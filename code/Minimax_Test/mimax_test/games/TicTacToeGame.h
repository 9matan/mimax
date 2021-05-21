#pragma once

#include <vector>
#include <functional>

namespace mimax_test {
namespace games {
namespace tic_tac_toe {

    using SMove = std::pair<int, int>;
    struct SGameState
    {
        // '-' empty, 'X' x, 'O' o, 'D' draw
        char m_map[3][4];
        char m_player;
    };

    void MakeMove(SGameState& state, SMove const move);
    char GetWinner(SGameState const& state);

    template<typename TMovesContainer>
    void GetPossibleMoves(TMovesContainer& moves, SGameState const& state)
    {
        moves.clear();
        if (GetWinner(state) != '-') return;

        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                if (state.m_map[i][j] == '-')
                    moves.push_back(std::make_pair(i, j));
            }
        }
    }

    char PlayGame(SGameState state, std::function<SMove(SGameState const&)> nextMoveFunc);
}
}
}