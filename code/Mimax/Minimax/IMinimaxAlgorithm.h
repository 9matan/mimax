#pragma once

namespace mimax
{

struct SMinimaxConfig
{
    unsigned int m_maxNodesCount = 0;
    unsigned int m_maxDepth = 0;
    bool m_staticTree = true;

};

template<typename TState, typename TTransition>
class IMinimaxAlgorithm
{
public:
    ~IMinimaxAlgorithm() = default;

    virtual TTransition Solve(TState const& state, SMinimaxConfig const& config) = 0;
};

}