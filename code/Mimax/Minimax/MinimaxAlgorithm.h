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

    virtual TTransition Solve(TState const& state, SMinimaxConfig const& config) override
    {
        m_config = config;
        m_nodesPool.ResetPool(config.m_staticTree ? config.m_maxNodesCount : config.m_maxDepth);
        auto root = m_nodesPool.AllocateObject(state);

        if(config.m_staticTree)
        {
            BuildStaticTree(*root);
            auto const result = VisitStaticNode(*root, EOperation::Max);
            return result.m_transition;
        }

        auto const result = VisitDynamicNode(*root, SDynamicNodeInfo(), EOperation::Max);
        return result.m_transition;
    }
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

    struct SDynamicNodeInfo
    {
        float m_alhpa = 0.0f;
        float m_beta = 1.0f;
        unsigned int m_depth = 0;
    };

    struct STraversalResult
    {
        float m_score = 0;
        TTransition m_transition; 
    };
private:
    void BuildStaticTree(SNode& node)
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
    STraversalResult VisitStaticNode(SNode& node, EOperation const operation)
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
            auto const childResult = VisitStaticNode(*child.m_node, nextOperation);
            if(firstChild || IsBetter(childResult.m_score, result.m_score, operation))
            {
                firstChild = false;
                result.m_transition = child.m_transition;
                result.m_score = childResult.m_score;
            }
        }

        return result;
    }
    STraversalResult VisitDynamicNode(SNode& node, SDynamicNodeInfo const nodeInfo, EOperation const operation)
    {   
        STraversalResult result;
        if(nodeInfo.m_depth == m_config.m_maxDepth)
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
        EOperation const nextOperation = static_cast<EOperation>((static_cast<int>(operation) + 1) % static_cast<int>(EOperation::Count));
        for(auto& child: node.m_children)
        {
            child.m_node = CreateNode(node.m_state, child.m_transition);
            SDynamicNodeInfo childNodeInfo = nodeInfo;
            ++childNodeInfo.m_depth;
            auto const childResult = VisitDynamicNode(*child.m_node, childNodeInfo, nextOperation);
            if(firstChild || IsBetter(childResult.m_score, result.m_score, operation))
            {
                firstChild = false;
                result.m_transition = child.m_transition;
                result.m_score = childResult.m_score;
            }
            m_nodesPool.FreeLastObject();
        }

        return result;
    }
    SNode* CreateNode(TState const& parent, TTransition const transition)
    {
        auto const nodeState = m_resolver->TakeTransition(parent, transition);
        auto node = m_nodesPool.AllocateObject(nodeState);
        return node;
    }
    bool IsBetter(float const newScore, float const currentScore, EOperation const operation) const
    {
        return (operation == EOperation::Min ? newScore < currentScore : newScore > currentScore);
    }
private:
    CObjectPool<SNode> m_nodesPool;
    IResolver* const m_resolver;
    SMinimaxConfig m_config;
};

}