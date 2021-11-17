#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "mimax/minimax/MinimaxAlgorithmBase.h"

#include "mimax_test/games/TicTacToeGame.h"

namespace mimax_test {
namespace minimax {
namespace tic_tac_toe {

    using STicTacToeMove = mimax_test::games::tic_tac_toe::SMove;
    using STicTacToeState = mimax_test::games::tic_tac_toe::SGameState;
    using CTicTacToeMovesContainer = std::vector<STicTacToeMove>;
    using FindNextMoveFunc = mimax_test::games::tic_tac_toe::FindNextMoveFunc;

    class CMinimaxResolver
    {
    public:
        CMinimaxResolver(char const myPlayer)
            : m_myPlayer(myPlayer)
        {}

        CMinimaxResolver(char const myPlayer, std::vector<STicTacToeState> const& unexpectedStates)
            : m_myPlayer(myPlayer)
            , m_unexpectedStates(unexpectedStates)
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
            EXPECT_THAT(m_unexpectedStates, testing::Not(testing::Contains(state)));
        }
    };

    using CTicTacToeMinimax = mimax::minimax::CMinimaxAlgorithmBase<STicTacToeState, STicTacToeMove, CTicTacToeMovesContainer, CMinimaxResolver>;

    static FindNextMoveFunc CreateFindNextMoveFunc(std::vector<STicTacToeState> const& unexpectedStates = std::vector<STicTacToeState>())
    {
        return [&unexpectedStates](STicTacToeState const& state) {
            CTicTacToeMinimax minimax(9, CMinimaxResolver(state.m_player, unexpectedStates));
            return minimax.Solve(state);
        };
    }

    static void FindNextMove_SpecifiedState_ReturnsExpectedMove(STicTacToeState const& state, CTicTacToeMovesContainer const& expectedMoves)
    {
        auto findNextMoveFunc = CreateFindNextMoveFunc();

        auto move = findNextMoveFunc(state);

        EXPECT_THAT(expectedMoves, testing::Contains(move));
    }

    static void FindNextMove_SpecifiedState_NotReturnsUnexpectedMove(STicTacToeState const& state, CTicTacToeMovesContainer const& unexpectedMoves)
    {
        auto findNextMoveFunc = CreateFindNextMoveFunc();

        auto move = findNextMoveFunc(state);

        EXPECT_THAT(unexpectedMoves, testing::Not(testing::Contains(move)));
    }

    static void PlayGame_SpecifiedState_ReturnsExpectedWinner(STicTacToeState const& state, char const expectedWinner)
    {
        auto findNextMoveFunc = CreateFindNextMoveFunc();

        auto const winner = mimax_test::games::tic_tac_toe::PlayGame(state, findNextMoveFunc);

        EXPECT_EQ(winner, expectedWinner);
    }

#if MINIMAX_ENABLE_ALPHA_BETA_PRUNING
    static void PlayGame_SpecifiedState_NotEntersUnexpectedStates(STicTacToeState const& state, std::vector<STicTacToeState> const& unexpectedStates)
    {
        auto findNextMoveFunc = CreateFindNextMoveFunc(unexpectedStates);

        mimax_test::games::tic_tac_toe::PlayGame(state, findNextMoveFunc);
    }
#endif // MINIMAX_ENABLE_ALPHA_BETA_PRUNING

    GTEST_TEST(CMinimaxAlgorithmBase_TicTacToe, FindNextMove_SpecifiedState_ReturnsExpectedMove_1)
    {
        FindNextMove_SpecifiedState_ReturnsExpectedMove(
            {
                {"XXO",
                 "OXX",
                 "OO-"}, 'X'
            },
            { { 2, 2 } }
        );
    }

    GTEST_TEST(CMinimaxAlgorithmBase_TicTacToe, FindNextMove_SpecifiedState_ReturnsExpectedMove_2)
    {
        FindNextMove_SpecifiedState_ReturnsExpectedMove(
            {
                {"XXO",
                 "-X-",
                 "OO-"}, 'X'
            },
            { { 2, 2 } }
        );
    }

    GTEST_TEST(CMinimaxAlgorithmBase_TicTacToe, FindNextMove_SpecifiedState_NotReturnsUnexpectedMove_1)
    {
        FindNextMove_SpecifiedState_NotReturnsUnexpectedMove(
            {
                {"X--",
                 "-O-",
                 "--X"}, 'O'
            },
            { { 0, 2 }, {2, 0} }
        );
    }
    
    GTEST_TEST(CMinimaxAlgorithmBase_TicTacToe, PlayGame_SpecifiedState_ReturnsExpectedWinner_1)
    {
        PlayGame_SpecifiedState_ReturnsExpectedWinner(
            {
                {"X--",
                 "-O-",
                 "O-X"}, 'X'
            },
            'X'
        );
    }

    GTEST_TEST(CMinimaxAlgorithmBase_TicTacToe, PlayGame_SpecifiedState_ReturnsExpectedWinner_2)
    {
        PlayGame_SpecifiedState_ReturnsExpectedWinner(
            {
                {"---",
                 "---",
                 "---"}, 'X'
            },
            'D'
        );
    }

    GTEST_TEST(CMinimaxAlgorithmBase_TicTacToe, PlayGame_SpecifiedState_ReturnsExpectedWinner_3)
    {
        PlayGame_SpecifiedState_ReturnsExpectedWinner(
            {
                {"---",
                 "-X-",
                 "---"}, 'O'
            },
            'D'
        );
    }

    GTEST_TEST(CMinimaxAlgorithmBase_TicTacToe, PlayGame_SpecifiedState_ReturnsExpectedWinner_4)
    {
        PlayGame_SpecifiedState_ReturnsExpectedWinner(
            {
                {"O--",
                 "XOX",
                 "--X"}, 'O'
            },
            'O'
        );
    }

    GTEST_TEST(CMinimaxAlgorithmBase_TicTacToe, PlayGame_SpecifiedState_ReturnsExpectedWinner_5)
    {
        PlayGame_SpecifiedState_ReturnsExpectedWinner(
            {
                {"-OO",
                 "XXO",
                 "--X"}, 'X'
            },
            'X'
        );
    }

#if MINIMAX_ENABLE_ALPHA_BETA_PRUNING
    GTEST_TEST(CMinimaxAlgorithmBase_TicTacToe, PlayGame_SpecifiedState_NotEntersUnexpectedStates_1)
    {
        PlayGame_SpecifiedState_NotEntersUnexpectedStates(
            {
                {"-OO",
                 "XXO",
                 "--X"}, 'X'
            },
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
        );
    }
#endif // MINIMAX_ENABLE_ALPHA_BETA_PRUNING
    
} // tic_tac_toe
} // minimax
} // mimax_test