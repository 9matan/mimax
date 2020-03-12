#pragma once

namespace mimax
{

template<typename TState, typename TTransition>
class IMinimaxAlgorithm
{
public:
    ~IMinimaxAlgorithm() = default;

    virtual TTransition Solve(TState const& state) = 0;
};

}