#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "mimax/dma/MinimaxBase.h"

#include "mimax_test/games/TicTacToeGame.h"

namespace mimax_test {
namespace dma {
namespace minimax {

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

    using CTicTacToeMinimax = mimax::dma::CMinimaxBase<STicTacToeState, STicTacToeMove, CTicTacToeMovesContainer, CMinimaxResolver>;

    static FindNextMoveFunc CreateFindNextMoveFunc(std::vector<STicTacToeState> const& unexpectedStates = std::vector<STicTacToeState>())
    {
        return [&unexpectedStates](STicTacToeState const& state) {
            CTicTacToeMinimax::SConfig config;
            config.m_maxValue = 1.0f;
            config.m_minValue = -1.0f;
            config.m_maxDepth = 9;
            config.m_epsilon = 0.1f;
            CMinimaxResolver const resolver(state.m_player, unexpectedStates);

            CTicTacToeMinimax minimax(resolver, config);
            return minimax.FindSolution(state).value();
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

    static void PlayGame_SpecifiedState_NotEntersUnexpectedStates(STicTacToeState const& state, std::vector<STicTacToeState> const& unexpectedStates)
    {
        auto findNextMoveFunc = CreateFindNextMoveFunc(unexpectedStates);

        mimax_test::games::tic_tac_toe::PlayGame(state, findNextMoveFunc);
    }

    GTEST_TEST(DmaCMinimaxBaseTicTacToe, FindNextMoveSpecifiedStateReturnsExpectedMove1)
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

    GTEST_TEST(DmaCMinimaxBaseTicTacToe, FindNextMoveSpecifiedStateReturnsExpectedMove2)
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

    GTEST_TEST(DmaCMinimaxBaseTicTacToe, FindNextMoveSpecifiedStateNotReturnsUnexpectedMove)
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
    
    GTEST_TEST(DmaCMinimaxBaseTicTacToe, PlayGameSpecifiedStateReturnsWinnerX1)
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

    GTEST_TEST(DmaCMinimaxBaseTicTacToe, PlayGameSpecifiedStateReturnsWinnerX2)
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

    GTEST_TEST(DmaCMinimaxBaseTicTacToe, PlayGameSpecifiedStateReturnsDraw1)
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

    GTEST_TEST(DmaCMinimaxBaseTicTacToe, PlayGameSpecifiedStateReturnsDraw2)
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

    GTEST_TEST(DmaCMinimaxBaseTicTacToe, PlayGameSpecifiedStateReturnsWinnerO)
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

    GTEST_TEST(DmaCMinimaxBaseTicTacToe, PlayGameSpecifiedStateNotEntersUnexpectedStates)
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
    
} // minimax
} // dma
} // mimax_test