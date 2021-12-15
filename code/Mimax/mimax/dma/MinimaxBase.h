#pragma once

#include <limits>

#include "mimax/dma/MinimaxDebugInfo.h"

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
    struct SConfig
    {
        float m_minValue = -1.0f;
        float m_maxValue = 1.0f;
        float m_epsilon = std::numeric_limits<float>::epsilon();
        size_t m_maxDepth = 0;
    };

public:
    CMinimaxBase(TResolver const& resolver, SConfig const& config)
        : m_resolver(resolver)
        , m_config(config)
    {}

    inline TMove Solve(TState const& state)
    {
#if MIMAX_MINIMAX_DEBUG
        m_debugInfo.Reset();
#endif // MIMAX_MINIMAX_DEBUG
        return VisitState(state, 0, m_config.m_minValue, m_config.m_maxValue).m_move;
    }

#if MIMAX_MINIMAX_DEBUG
    inline SMinimaxDebugInfo const& GetDebugInfo() const { return m_debugInfo; }
#endif // MIMAX_MINIMAX_DEBUG

private:
    struct STraversalResult
    {
        float m_score = 0.0f;
        TMove m_move; 
    };

private:
    STraversalResult VisitState(TState const& state, size_t const depth, float alpha, float beta)
    {
#if MIMAX_MINIMAX_DEBUG
        m_debugInfo.VisitNode(depth);
#endif // MIMAX_MINIMAX_DEBUG
        TMovesContainer moves;
        if(depth != m_config.m_maxDepth)
        {
            m_resolver.GetPossibleMoves(moves, state);
        }
        STraversalResult result;
        if(moves.empty())
        {
#if MIMAX_MINIMAX_DEBUG
            m_debugInfo.EvaluateNode();
#endif // MIMAX_MINIMAX_DEBUG
            int const colorMultiplier = (depth & 1) == 0 ? 1 : -1;
            result.m_score = m_resolver.EvaluateState(state) * colorMultiplier;
            return result; 
        }

        result.m_score = -std::numeric_limits<float>::max();

        for (size_t i = 0; i < moves.size(); ++i)
        {
            auto const move = moves[i];
            TState childState = state;
            m_resolver.MakeMove(childState, move);
            auto const childResult = VisitState(childState, depth + 1, -beta, -alpha);
            if (-childResult.m_score > result.m_score)
            {
                result.m_move = move;
                result.m_score = -childResult.m_score;
                alpha = (result.m_score > alpha) ? result.m_score : alpha;
                if (alpha + m_config.m_epsilon >= beta)
                {
#if MIMAX_MINIMAX_DEBUG
                    m_debugInfo.PruneNodes(moves.size() - (i + 1), depth + 1);
#endif // MIMAX_MINIMAX_DEBUG
                    break;
                }
            }
        }

        return result;
    }

private:
    TResolver m_resolver;
    SConfig m_config;
#if MIMAX_MINIMAX_DEBUG
    SMinimaxDebugInfo m_debugInfo;
#endif // MIMAX_MINIMAX_DEBUG
};

} // dma
} // mimax