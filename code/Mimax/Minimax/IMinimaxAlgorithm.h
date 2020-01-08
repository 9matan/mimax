#pragma once

namespace mimax
{
template<typename TState, typename TTransition>
class IMinimaxAlgorithm
{
public:
    ~IMinimaxAlgorithm() = default;

    virtual void TTransition Solve(TState const& state, IResolver* resolver) = 0;
};

}