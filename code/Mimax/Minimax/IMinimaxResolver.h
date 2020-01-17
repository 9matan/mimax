#pragma once

#include <vector>

namespace mimax
{

template<typename TState, typename TTransition>
class IMinimaxResolver
{
public:
    virtual ~IMinimaxResolver() = default;

    virtual float EvaluateState(TState const& state) = 0;
    virtual std::vector<TTransition> CollectTransitions(TState const& state) = 0;
    virtual TState TakeTransition(TState const& state, TTransition const transition) = 0; 
};

}