#pragma once

#include <limits>

namespace mimax {
namespace minimax {

// TResolver
// float EvaluateState(TState const&);
// void GetPossibleMoves(TMovesContainer&, TState const&);
// void MakeMove(TState&, TMove);

// TMovesContainer
// bool empty();
// begin(), end()

template<typename TState, typename TMove, typename TMovesContainer, typename TResolver>
class CMinimaxAlgorithmBase
{
public:
    CMinimaxAlgorithmBase(size_t const maxDepth, TResolver const& resolver)
        : m_maxDepth(maxDepth)
        , m_resolver(resolver)
    {}

    inline TMove Solve(TState const& state)
    {
        return VisitState(state, 0).m_move;
    }

private:
    struct STraversalResult
    {
        float m_score = 0.0f;
        TMove m_move; 
    };

private:
    STraversalResult VisitState(TState const& state, size_t const depth)
    {
        STraversalResult result;
        if(depth == m_maxDepth)
        {
            result.m_score = m_resolver.EvaluateState(state);
            return result; 
        }
        TMovesContainer moves;
        m_resolver.GetPossibleMoves(moves, state);
        if(moves.empty())
        {
            result.m_score = m_resolver.EvaluateState(state);
            return result; 
        }

        bool const minOp = (depth & 1);
        result.m_score = minOp
            ? std::numeric_limits<float>::max()
            : -std::numeric_limits<float>::max();

        for(auto const move: moves)
        {
            TState childState = state;
            m_resolver.MakeMove(childState, move);
            auto const childResult = VisitState(childState, depth + 1);
            if(minOp && (childResult.m_score < result.m_score) || !minOp && (childResult.m_score > result.m_score))
            {
                result.m_move = move;
                result.m_score = childResult.m_score;
            }
        }

        return result;
    }

private:
    TResolver m_resolver;
    size_t m_maxDepth;
};

}
}