#pragma once

#include <queue>

#include "Common/ObjectPool.h"
#include "Minimax/IMinimaxAlgorithm.h"
#include "Minimax/IMinimaxResolver.h"
#include "Minimax/MinimaxAlgorithmHelper.h"

namespace mimax
{

template<typename TState, typename TTransition>
class CMinimaxBFSAlgorithm
    : public IMinimaxAlgorithm<TState, TTransition>
{
public:
    struct SConfig
    {
        unsigned int m_maxNodesCount = 1;
        unsigned int m_maxDepth = 1;
    };
public:
    using IResolver = IMinimaxResolver<TState, TTransition>;
public:
    CMinimaxBFSAlgorithm(IResolver* const resolver, SConfig const& config)
        : m_resolver(resolver)
        , m_config(config)
    {}

    virtual TTransition Solve(TState const& state) override
    {
        // m_config.m_maxDepth - max number of turns in depth
        // so number of levels is max depth + 1
        m_levels.reserve(m_config.m_maxDepth + 1);
        m_nodesPool.ResetPool(m_config.m_maxNodesCount);
        auto root = m_nodesPool.AllocateObject();
        root->m_state = state;
        auto const result = BFS(*root);
        return result.m_transition;
    }

    size_t GetNodesCount() const { return m_nodesPool.GetAllocatedSize(); }
private:
    struct STraversalResult
    {
        float m_score = 0;
        TTransition m_transition; 
    };

    struct SNode
    {
        TState m_state;
        SNode* m_parent = nullptr;
        TTransition m_parentTransition;
        STraversalResult m_result;
        bool m_handled = false;
    };
private:
    STraversalResult BFS(SNode& rootNode)
    {
        m_levels.emplace_back();
        m_levels[0].push_back(&rootNode);
        for (unsigned int depth = 0; !m_nodesPool.IsEmpty() && depth != m_config.m_maxDepth; ++depth)
        {
            m_levels.emplace_back();
            m_levels[depth + 1].reserve(m_levels[depth].size());
            for(auto node: m_levels[depth])
            {
                node->m_handled = true;
                auto const transitions = m_resolver->CollectTransitions(node->m_state);
                if (transitions.empty())
                {
                    node->m_result.m_score = m_resolver->EvaluateState(node->m_state);
                    continue;
                }
                node->m_result.m_score = GetDefaultScore(GetOperation(depth));

                for (auto const t : transitions)
                {
                    if (m_nodesPool.IsEmpty())
                    {
                        break;
                    }

                    auto& childNode = CreateNode(*node, t);
                    m_levels[depth + 1].push_back(&childNode);
                }

                if (m_nodesPool.IsEmpty())
                {
                    break;
                }
            }
        }

        // evaluate unhandled nodes of two last levels 
        for (int depth = m_levels.size() - 1; depth >= std::max((int)m_levels.size() - 2, 0); --depth)
        {
            for(auto& node: m_levels[depth])
            {
                if(!node->m_handled)
                {
                    node->m_result.m_score = m_resolver->EvaluateState(node->m_state);
                }
            }
        }
        
        for(unsigned int depth = m_levels.size() - 1; depth > 0; --depth)
        {
            for(auto& node: m_levels[depth])
            {
                auto& parent = *(node->m_parent);
                if(CompareScore(node->m_result.m_score, parent.m_result.m_score, GetOperation(depth - 1)))
                {
                    parent.m_result.m_score = node->m_result.m_score;
                    parent.m_result.m_transition = node->m_parentTransition;
                }
            }
        }

        return rootNode.m_result;
    }
    SNode& CreateNode(SNode& parent, TTransition const transition)
    {
        auto const nodeState = m_resolver->TakeTransition(parent.m_state, transition);
        SNode& node = *m_nodesPool.AllocateObject();
        node.m_state = nodeState;
        node.m_parent = &parent;
        node.m_parentTransition = transition;
        node.m_handled = false;
        return node;
    }
    EMinimaxOperation GetOperation(unsigned int const depth) 
    { 
        return static_cast<EMinimaxOperation>(depth % static_cast<int>(EMinimaxOperation::Count));
    }
private:
    std::vector<std::vector<SNode*>> m_levels;
    CObjectPool<SNode> m_nodesPool;
    IResolver* const m_resolver;
    SConfig m_config;
};

}