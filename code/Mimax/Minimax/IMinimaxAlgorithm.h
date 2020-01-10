#pragma once

namespace mimax
{

struct SMinimaxConfig
{
    size_t m_maxNodesCount = 0;
    size_t m_maxDepth = 0;
};

template<typename TState, typename TTransition>
class IMinimaxAlgorithm
{
public:
    ~IMinimaxAlgorithm() = default;

    virtual TTransition Solve(TState const& state, SMinimaxConfig const& config) = 0;
};

}