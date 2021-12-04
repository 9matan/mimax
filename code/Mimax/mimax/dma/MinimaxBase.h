#pragma once

#include <limits>

namespace mimax {
namespace dma {

/*
TMovesContainer
    bool empty()
    iter begin()
    iter end()
*/

/*
TResolver
    float EvaluateState(TState const&)
    void GetPossibleMoves(TMovesContainer&, TState const&)
    void MakeMove(TState&, TMove)
*/

template<typename TState, typename TMove, typename TMovesContainer, typename TResolver>
class CMinimaxBase
{
public:
    CMinimaxBase(size_t const maxDepth, TResolver const& resolver)
        : CMinimaxBase(maxDepth, resolver, -1.0f, 1.0f)
    {}

    CMinimaxBase(size_t const maxDepth, TResolver const& resolver, float const minValue, float const maxValue)
        : m_maxDepth(maxDepth)
        , m_resolver(resolver)
        , m_minValue(minValue)
        , m_maxValue(maxValue)
    {}

    inline TMove Solve(TState const& state)
    {
        return VisitState(state, 0, m_minValue, m_maxValue).m_move;
    }

private:
    struct STraversalResult
    {
        float m_score = 0.0f;
        TMove m_move; 
    };

private:
    STraversalResult VisitState(TState const& state, size_t const depth, float alpha, float beta)
    {
        TMovesContainer moves;
        if(depth != m_maxDepth)
        {
            m_resolver.GetPossibleMoves(moves, state);
        }
        STraversalResult result;
        if(moves.empty())
        {
            int const colorMultiplier = (depth & 1) == 0 ? 1 : -1;
            result.m_score = m_resolver.EvaluateState(state) * colorMultiplier;
            return result; 
        }

        result.m_score = -std::numeric_limits<float>::max();

        for(auto const move: moves)
        {
            TState childState = state;
            m_resolver.MakeMove(childState, move);
            auto const childResult = VisitState(childState, depth + 1, -beta, -alpha);
            if(-childResult.m_score > result.m_score)
            {
                result.m_move = move;
                result.m_score = -childResult.m_score;
                alpha = (result.m_score > alpha) ? result.m_score : alpha;
                if (alpha >= beta) break;
            }
        }

        return result;
    }

private:
    TResolver m_resolver;
    size_t m_maxDepth;
    float m_minValue;
    float m_maxValue;
};

} // dma
} // mimax