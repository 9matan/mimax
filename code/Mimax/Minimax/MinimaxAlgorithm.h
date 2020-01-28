#pragma once

#include <queue>

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
    enum class EOperation
    {
        Max = 0,
        Min,
        Count
    };
    
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
        float m_score = 0;
        TTransition m_transition; 
    };
private:
    void BuildTree(SNode& node);
    STraversalResult VisitNode(SNode& node, EOperation const operation);
    SNode* CreateNode(TState const& parent, TTransition const transition);
    bool IsBetter(float const newScore, float const currentScore, EOperation const operation) const;
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

    BuildTree(*root);
    auto const result = VisitNode(*root, EOperation::Max);

    return result.m_transition;
}

template<typename TState, typename TTransition>
void CMinimaxAlgorithm<TState, TTransition>::BuildTree(SNode& node)
{
    std::queue<SNode*> currentQueue;
    std::queue<SNode*> nextQueue;
    currentQueue.push(&node);
    
    for (unsigned int depth = 0; !m_nodesPool.IsEmpty() && depth != m_config.m_maxDepth; ++depth, std::swap(currentQueue, nextQueue))
    {
        while (!currentQueue.empty())
        {
            SNode& currentNode = *currentQueue.front();
            currentQueue.pop();
            auto const transitions = m_resolver->CollectTransitions(currentNode.m_state);
            if (transitions.empty())
            {
                continue;
            }

            currentNode.m_children.reserve(transitions.size());
            for (auto const t : transitions)
            {
                if (m_nodesPool.IsEmpty())
                {
                    break;
                }

                SNodeEntry entry;
                entry.m_transition = t;
                entry.m_node = CreateNode(currentNode.m_state, t);
                currentNode.m_children.push_back(entry);
                nextQueue.push(entry.m_node);
            }

            if (m_nodesPool.IsEmpty())
            {
                break;
            }
        }
    }
}

template<typename TState, typename TTransition>
typename CMinimaxAlgorithm<TState, TTransition>::STraversalResult CMinimaxAlgorithm<TState, TTransition>::VisitNode(SNode& node, EOperation const operation)
{
    STraversalResult result;
    if (node.m_children.empty())
    {
        result.m_score = m_resolver->EvaluateState(node.m_state);
        return result;
    }

    bool firstChild = true;
    EOperation const nextOperation = static_cast<EOperation>((static_cast<int>(operation) + 1) % static_cast<int>(EOperation::Count));
    for(auto& child: node.m_children)
    {
        auto const childResult = VisitNode(*child.m_node, nextOperation);
        if(firstChild || IsBetter(childResult.m_score, result.m_score, operation))
        {
            firstChild = false;
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
bool CMinimaxAlgorithm<TState, TTransition>::IsBetter(float const newScore, float const currentScore, EOperation const operation) const
{
    return (operation == EOperation::Min ? newScore < currentScore : newScore > currentScore);
}

#pragma endregion Definition

}