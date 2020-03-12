#pragma once

#include "Minimax/IMinimaxAlgorithm.h"
#include "Minimax/IMinimaxResolver.h"
#include "Minimax/MinimaxAlgorithmHelper.h"

namespace mimax
{

template<typename TState, typename TTransition>
class CMinimaxDFSAlgorithm
    : public IMinimaxAlgorithm<TState, TTransition>
{
public:
    struct SConfig
    {
        unsigned m_maxDepth;
    };
public:
    using IResolver = IMinimaxResolver<TState, TTransition>;
public:
    CMinimaxDFSAlgorithm(IResolver* const resolver, SConfig const& config)
        : m_resolver(resolver)
        , m_config(config)
    {}

    virtual TTransition Solve(TState const& state) override
    {
        unsigned int const depth = 0;
        auto const result = VisitState(state, depth, EMinimaxOperation::Max);
        return result.m_transition;
    }
private:
    struct STraversalResult
    {
        float m_score = 0;
        TTransition m_transition; 
    };
private:
    STraversalResult VisitState(TState const& state, unsigned int const depth, EMinimaxOperation const operation)
    {   
        STraversalResult result;
        if(depth == m_config.m_maxDepth)
        {
            result.m_score = m_resolver->EvaluateState(state);
            return result; 
        }
        auto const transitions = m_resolver->CollectTransitions(state);
        if(transitions.empty())
        {
            result.m_score = m_resolver->EvaluateState(state);
            return result; 
        }
        result.m_score = GetDefaultScore(operation);

        EMinimaxOperation const nextOperation = static_cast<EMinimaxOperation>((static_cast<int>(operation) + 1) % static_cast<int>(EMinimaxOperation::Count));
        for(auto const transition: transitions)
        {
            TState const childState = m_resolver->TakeTransition(state, transition);
            auto const childResult = VisitState(childState, depth + 1, nextOperation);
            if(CompareScore(childResult.m_score, result.m_score, operation))
            {
                result.m_transition = childResult.m_transition;
                result.m_score = childResult.m_score;
            }
        }

        return result;
    }
private:
    IResolver* const m_resolver;
    SConfig m_config;
};

}