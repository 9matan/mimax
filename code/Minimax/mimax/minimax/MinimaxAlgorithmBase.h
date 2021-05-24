#pragma once

#include <limits>

// Define MINIMAX_ENABLE_ALPHA_BETA_PRUNING (1) to enabled pruning
#define MINIMAX_ENABLE_ALPHA_BETA_PRUNING (1)

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
#if MINIMAX_ENABLE_ALPHA_BETA_PRUNING
        return VisitState(state, 0, -std::numeric_limits<float>::max(), std::numeric_limits<float>::max()).m_move;
#else
        return VisitState(state, 0).m_move;
#endif // MINIMAX_ENABLE_ALPHA_BETA_PRUNING
    }

private:
    struct STraversalResult
    {
        float m_score = 0.0f;
        TMove m_move; 
    };

private:
#if MINIMAX_ENABLE_ALPHA_BETA_PRUNING
    STraversalResult VisitState(TState const& state, size_t const depth, float alpha, float beta)
#else
    STraversalResult VisitState(TState const& state, size_t const depth)
#endif // MINIMAX_ENABLE_ALPHA_BETA_PRUNING
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
#if MINIMAX_ENABLE_ALPHA_BETA_PRUNING
            auto const childResult = VisitState(childState, depth + 1, alpha, beta);
#else
            auto const childResult = VisitState(childState, depth + 1);
#endif // MINIMAX_ENABLE_ALPHA_BETA_PRUNING
            if(minOp && (childResult.m_score < result.m_score) || !minOp && (childResult.m_score > result.m_score))
            {
                result.m_move = move;
                result.m_score = childResult.m_score;
#if MINIMAX_ENABLE_ALPHA_BETA_PRUNING
                if (minOp)
                {
                    beta = (result.m_score < beta) ? result.m_score : beta;
                }
                else
                {
                    alpha = (result.m_score > alpha) ? result.m_score : alpha;
                }

                if (alpha >= beta) break;
#endif // MINIMAX_ENABLE_ALPHA_BETA_PRUNING
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