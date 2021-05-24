#include <cassert>
#include <string>
#include <vector>

#include "mimax/minimax/MinimaxAlgorithmBase.h"

#include "mimax_test/games/TicTacToeGame.h"

namespace mimax_test {
namespace minimax {
namespace tic_tac_toe {

    using STicTacToeMove = mimax_test::games::tic_tac_toe::SMove;
    using STicTacToeState = mimax_test::games::tic_tac_toe::SGameState;
    using CTicTacToeMovesContainer = std::vector<STicTacToeMove>;
    using FindNextMoveFunc = mimax_test::games::tic_tac_toe::FindNextMoveFunc;

    static void PlayGame_ExpextedWinner_TestCase(FindNextMoveFunc func, STicTacToeState const& state, char const expectedWinner)
    {
        assert(mimax_test::games::tic_tac_toe::PlayGame(state, func) == expectedWinner);
    }

    static void FindNextMove_Expected_TestCase(FindNextMoveFunc func, STicTacToeState const& state, CTicTacToeMovesContainer const expectedMoves)
    {
        auto const move = func(state);
        for (auto const expected : expectedMoves)
        {
            if (move == expected) return;
        }
        assert(false);
    }

    static void FindNextMove_Unexpected_TestCase(FindNextMoveFunc func, STicTacToeState const& state, CTicTacToeMovesContainer const unexpectedMoves)
    {
        auto const move = func(state);
        for (auto const unexpected : unexpectedMoves)
        {
            assert(move != unexpected);
        }
    }
}
}
}