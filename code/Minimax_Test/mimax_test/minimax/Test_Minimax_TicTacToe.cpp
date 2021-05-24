#include <cassert>
#include <string>
#include <vector>

#include "mimax/minimax/MinimaxAlgorithmBase.h"

#include "mimax_test/minimax/Helper_Minimax_TicTacToe.h"

namespace mimax_test {
namespace minimax {
namespace tic_tac_toe {

    class CMinimaxResolver
    {
    public:
        CMinimaxResolver(char const myPlayer)
            : m_myPlayer(myPlayer)
        {}

        CMinimaxResolver(char const myPlayer, std::vector<STicTacToeState> const& unexpectedState)
            : m_myPlayer(myPlayer)
            , m_unexpectedStates(unexpectedState)
        {}

        float EvaluateState(STicTacToeState const& state)
        {
            CheckUnexpectedStates(state);
            char const winner = mimax_test::games::tic_tac_toe::GetWinner(state);
            if (winner == '-' || winner == 'D') return 0.0f;
            return winner == m_myPlayer ? 1.0f : -1.0f;
        }

        void GetPossibleMoves(CTicTacToeMovesContainer& movesOut, STicTacToeState const& state)
        {
            CheckUnexpectedStates(state);
            mimax_test::games::tic_tac_toe::GetPossibleMoves(movesOut, state);
        }

        void MakeMove(STicTacToeState& state, STicTacToeMove const move)
        {
            CheckUnexpectedStates(state);
            mimax_test::games::tic_tac_toe::MakeMove(state, move);
        }

    private:
        std::vector<STicTacToeState> m_unexpectedStates;
        char m_myPlayer;

    private:
        void CheckUnexpectedStates(STicTacToeState const& state)
        {
            for (auto const& unexpectedState : m_unexpectedStates)
            {
                assert(unexpectedState != state);
            }
        }
    };

    using CTicTacToeMinimax = mimax::minimax::CMinimaxAlgorithmBase<STicTacToeState, STicTacToeMove, CTicTacToeMovesContainer, CMinimaxResolver>;
    static STicTacToeMove FindNextMove(STicTacToeState const& state)
    {
        CTicTacToeMinimax minimax(9, CMinimaxResolver(state.m_player));
        return minimax.Solve(state);
    }

    static FindNextMoveFunc Create_FindNextMove_UnexpectedStates(std::vector<STicTacToeState> const& unexpectedStates)
    {
        return [&unexpectedStates](STicTacToeState const& state) {
            CTicTacToeMinimax minimax(9, CMinimaxResolver(state.m_player, unexpectedStates));
            return minimax.Solve(state);
        };
    }

    void RunMinimaxTests()
    {
        FindNextMove_Expected_TestCase(FindNextMove,
            {
                {"XXO", 
                 "OXX", 
                 "OO-"}, 'X'
            },
            { { 2, 2 } }
        );
        
        FindNextMove_Expected_TestCase(FindNextMove,
            {
                {"XXO",
                 "-X-",
                 "OO-"}, 'X'
            },
            { { 2, 2 } }
        );

        FindNextMove_Unexpected_TestCase(FindNextMove,
            {
                {"X--",
                 "-O-",
                 "--X"}, 'O'
            },
            { { 0, 2 }, {2, 0} }
        );

        PlayGame_ExpextedWinner_TestCase(FindNextMove,
            {
                {"X--",
                 "-O-",
                 "O-X"}, 'X'
            },
            'X'
        );

        PlayGame_ExpextedWinner_TestCase(FindNextMove,
            {
                {"---",
                 "---",
                 "---"}, 'X'
            },
            'D'
        );

        PlayGame_ExpextedWinner_TestCase(FindNextMove,
            {
                {"---",
                 "-X-",
                 "---"}, 'O'
            },
            'D'
        );

        PlayGame_ExpextedWinner_TestCase(FindNextMove,
            {
                {"O--",
                 "XOX",
                 "--X"}, 'O'
            },
            'O'
        );

#if MINIMAX_ENABLE_ALPHA_BETA_PRUNING
        // alpha beta test
        PlayGame_ExpextedWinner_TestCase(
            Create_FindNextMove_UnexpectedStates(
                {
                    {
                        {"-OO",
                         "XXO",
                         "XOX"}, 'X'
                    },
                    {
                        {"-OO",
                         "XXO",
                         "OXX"}, 'X'
                    }
                }
            ),
            {
                {"-OO",
                 "XXO",
                 "--X"}, 'X'
            },
            'X'
        );
#endif // MINIMAX_ENABLE_ALPHA_BETA_PRUNING
    }
}
}
}