#pragma once

#include "Common/ObjectPool.h"
#include "Minimax/IMinimaxAlgorithm.h"
#include "Minimax/IMinimaxResolver.h"

namespace mimax
{

template<typename TState, typename TTransition>
class CMinimaxAlgorithm
    : public IMinimaxAlgorithm<TState, TTransition>
{
public:
    using IResolver = IMinimaxResolver<TState, TTransition>;
public:
    CMinimaxAlgorithm(IResolver* const resolver)
        : m_resolver(resolver) 
    {}

    virtual TTransition Solve(TState const& state, SMinimaxConfig const& config) override;
private:
    struct SNodeEntry
    {
        TTransition m_transition;
        struct SNode* m_node = nullptr;
    };

    struct SNode
    {
        SNode(TState const& state)
            : m_state(state)
        {}

        TState m_state;
        std::vector<SNodeEntry> m_children;
    };

    struct STraversalResult
    {
        int m_score = 0;
        TTransition m_transition; 
    };
private:
    STraversalResult VisitNode(SNode& node, size_t const depth);
    SNode* CreateNode(TState const& parent, TTransition const transition);
    bool IsBetter(int const newScore, int const currentScore, size_t const depth) const;
private:
    CObjectPool<SNode> m_nodesPool;
    IResolver* const m_resolver;
    SMinimaxConfig m_config;
};

#pragma region Definition

template<typename TState, typename TTransition>
TTransition CMinimaxAlgorithm<TState, TTransition>::Solve(TState const& state, SMinimaxConfig const& config)
{
    m_config = config;
    m_nodesPool.ResetPool(config.m_maxNodesCount);
    auto root = m_nodesPool.AllocateObject(state);

    size_t const depth = 0;
    auto const result = VisitNode(*root, depth);

    return result.m_transition;
}

template<typename TState, typename TTransition>
typename CMinimaxAlgorithm<TState, TTransition>::STraversalResult CMinimaxAlgorithm<TState, TTransition>::VisitNode(SNode& node, size_t const depth)
{
    STraversalResult result;
    if(depth == m_config.m_maxDepth || m_nodesPool.IsEmpty())
    {
        result.m_score = m_resolver->EvaluateState(node.m_state); 
        return result;
    }

    auto const transitions = m_resolver->CollectTransitions(node.m_state);
    if(transitions.empty())
    {
        result.m_score = m_resolver->EvaluateState(node.m_state); 
        return result;
    }

    node.m_children.reserve(transitions.size());
    for(auto const t: transitions)
    {
        SNodeEntry entry;
        entry.m_transition = t;
        node.m_children.push_back(entry);
    }

    bool firstChild = true;
    for(auto& child: node.m_children)
    {
        child.m_node = CreateNode(node.m_state, child.m_transition);
        auto const childResult = VisitNode(*child.m_node, depth + 1);
        if(firstChild)
        {
            firstChild = false;
            result.m_score = childResult.m_score;
        }
        else if(IsBetter(childResult.m_score, result.m_score, depth))
        {
            result.m_transition = child.m_transition;
            result.m_score = childResult.m_score;
        }
    }

    return result;
}

template<typename TState, typename TTransition>
typename CMinimaxAlgorithm<TState, TTransition>::SNode* CMinimaxAlgorithm<TState, TTransition>::CreateNode(TState const& parent, TTransition const transition)
{
    auto const nodeState = m_resolver->TakeTransition(parent, transition);
    auto node = m_nodesPool.AllocateObject(nodeState);
    return node;
}

template<typename TState, typename TTransition>
bool CMinimaxAlgorithm<TState, TTransition>::IsBetter(int const newScore, int const currentScore, size_t const depth) const
{
    return ((depth & 1) ? newScore < currentScore : newScore > currentScore);
}

#pragma endregion Definition

}