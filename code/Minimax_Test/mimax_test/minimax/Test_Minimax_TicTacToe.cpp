#include <cassert>
#include <string>
#include <vector>

#include "mimax/minimax/MinimaxAlgorithm.h"

#include "mimax_test/games/TicTacToeGame.h"

namespace
{
    using STicTacToeMove = mimax_test::games::tic_tac_toe::SMove;
    using STicTacToeState = mimax_test::games::tic_tac_toe::SGameState;
    using CTicTacToeMovesContainer = std::vector<STicTacToeMove>;

    class CTicTacToeResolver
    {
    public:
        CTicTacToeResolver(char const myPlayer)
            : m_myPlayer(myPlayer)
        {}

        float EvaluateState(STicTacToeState const& state)
        {
            char const winner = mimax_test::games::tic_tac_toe::GetWinner(state);
            if (winner == '-' || winner == 'D') return 0.0f;
            return winner == m_myPlayer ? 1.0f : -1.0f;
        }

        void GetPossibleMoves(CTicTacToeMovesContainer& movesOut, STicTacToeState const& state)
        {
            mimax_test::games::tic_tac_toe::GetPossibleMoves(movesOut, state);
        }

        void MakeMove(STicTacToeState& state, STicTacToeMove const move)
        {
            mimax_test::games::tic_tac_toe::MakeMove(state, move);
        }
    private:
        char m_myPlayer;
    };

    using CTicTacToeMinimax = mimax::minimax::CMinimaxAlgorithmBase<STicTacToeState, STicTacToeMove, CTicTacToeMovesContainer, CTicTacToeResolver>;

    STicTacToeMove FindNextMove(STicTacToeState const& state)
    {
        CTicTacToeMinimax minimax(9, CTicTacToeResolver(state.m_player));
        return minimax.Solve(state);
    }

    char PlayGame(STicTacToeState state)
    {
        return mimax_test::games::tic_tac_toe::PlayGame(state, FindNextMove);
    }

    void PlayGame_ExpextedWinner_TestCase(STicTacToeState const state, char const expectedWinner)
    {
        assert(PlayGame(state) == expectedWinner);
    }

    void FindNextMove_Expected_TestCase(STicTacToeState const state, CTicTacToeMovesContainer const expectedMoves)
    {
        auto const move = FindNextMove(state);
        for (auto const expected : expectedMoves)
        {
            if (move == expected) return;
        }
        assert(false);
    }

    void FindNextMove_NonExpected_TestCase(STicTacToeState const state, CTicTacToeMovesContainer const nonExpectedMoves)
    {
        auto const move = FindNextMove(state);
        for (auto const nonExpected : nonExpectedMoves)
        {
            assert(move != nonExpected);
        }
    }
}

namespace mimax_test {
namespace minimax {
namespace tic_tac_toe {
    void RunMinimaxTests()
    {
        FindNextMove_Expected_TestCase(
            {
                {"XXO", 
                 "OXX", 
                 "OO-"}, 'X'
            },
            { { 2, 2 } }
        );
        
        FindNextMove_Expected_TestCase(
            {
                {"XXO",
                 "-X-",
                 "OO-"}, 'X'
            },
            { { 2, 2 } }
        );

        FindNextMove_NonExpected_TestCase(
            {
                {"X--",
                 "-O-",
                 "--X"}, 'O'
            },
            { { 0, 2 }, {2, 0} }
        );

        PlayGame_ExpextedWinner_TestCase(
            {
                {"X--",
                 "-O-",
                 "O-X"}, 'X'
            },
            'X'
        );

        PlayGame_ExpextedWinner_TestCase(
            {
                {"---",
                 "---",
                 "---"}, 'X'
            },
            'D'
        );

        PlayGame_ExpextedWinner_TestCase(
            {
                {"---",
                 "-X-",
                 "---"}, 'O'
            },
            'D'
        );

        PlayGame_ExpextedWinner_TestCase(
            {
                {"O--",
                 "XOX",
                 "--X"}, 'O'
            },
            'O'
        );
    }
}
}
}