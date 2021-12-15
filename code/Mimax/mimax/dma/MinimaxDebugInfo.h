#pragma once

#define MIMAX_MINIMAX_DEBUG (1)

#if MIMAX_MINIMAX_DEBUG
#include <iostream>

namespace mimax {
namespace dma {

struct SMinimaxDebugInfo
{
    static constexpr size_t MAX_DEPTH = 64;

    size_t m_evaluatedNodesCnt;
    size_t m_visitedNodesCnt;
    size_t m_prunedNodesCnt[MAX_DEPTH + 1];
    size_t m_maxDepth;

    SMinimaxDebugInfo()
    {
        Reset();
    }

    inline void EvaluateNode()
    {
        ++m_evaluatedNodesCnt;
    }

    inline void VisitNode(size_t const depth)
    {
        ++m_visitedNodesCnt;
        m_maxDepth = (m_maxDepth < depth) ? depth : m_maxDepth;
    }

    inline void PruneNodes(size_t const nodesCnt, size_t const depth)
    {
        if (depth < MAX_DEPTH)
            m_prunedNodesCnt[depth] += nodesCnt;
    }

    inline void Reset()
    {
        m_evaluatedNodesCnt = 0;
        m_visitedNodesCnt = 0;
        m_maxDepth = 0;
        memset(m_prunedNodesCnt, 0, sizeof(m_prunedNodesCnt));
    }
};

std::ostream& operator<<(std::ostream& o, SMinimaxDebugInfo const& debugInfo);

} // dma
} // mimax
#endif // MIMAX_MINIMAX_DEBUG