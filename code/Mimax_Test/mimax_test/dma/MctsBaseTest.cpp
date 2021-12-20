#include <tuple>
#include <vector>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "mimax/dma/MCTSBase.h"

namespace mimax_test {
namespace dma {
namespace mcts {

using namespace std;
using mimax::dma::CMctsBase;

using STestMove = int;
using CTestMovesContainer = vector<STestMove>;

struct STestState
{
public:
    STestState() : STestState(0.0f) {}
    STestState(float const playoutScore)
        : m_isVisited(false)
        , m_playoutScore(playoutScore)
    {}

    STestState& AddState(float const playoutScore)
    {
        m_children.emplace_back(playoutScore);
        return m_children.back();
    }

    STestState& AddState(STestState const& state)
    {
        m_children.push_back(state);
        return m_children.back();
    }

    inline STestState& AddLostState() { return AddState(0.0f); }
    inline STestState& AddWonState() { return AddState(1.0f); }

public:
    mutable bool m_isVisited;
    float m_playoutScore;
    vector<STestState> m_children;
};

class CTestResolver
{
public:
    void GetPossibleMoves(STestState const* state, CTestMovesContainer& moves)
    {
        moves.resize(state->m_children.size());
        for (int i = 0; i < moves.size(); ++i)
        {
            moves[i] = i;
        }
    }

    void MakeMove(STestState*& state, STestMove const move)
    {
        state = &(state->m_children[move]);
    }

    float Playout(STestState const* state)
    {
        state->m_isVisited = true;
        return state->m_playoutScore;
    }
};

using CTestMcts = CMctsBase<STestState*, STestMove, CTestMovesContainer, CTestResolver>;

static CTestMcts CreateTestMCTS(STestState* state)
{
    unsigned long long const randomSeed = 1234567890ULL;
    float const explorationParam = 1.4142f;
    return CTestMcts(state, CTestResolver(), randomSeed, explorationParam);
}

static void EvaluateNTimes(CTestMcts& mcts, size_t const iterationsCnt)
{
    for (size_t i = 0; i < iterationsCnt; ++i)
    {
        mcts.Evaluate();
    }
}

static auto TestStateIsVisitedMatcher(bool const isVisited)
{
    return testing::Field(&STestState::m_isVisited, isVisited);
}

static STestState CreateTestStateWithSubstates(int const wonSubstatesCnt, int const lostSubstatesCnt)
{
    float const playoutScore = wonSubstatesCnt > lostSubstatesCnt
        ? 1.0f
        : 0.0f;
    STestState state(playoutScore);
    for (int i = 0; i < wonSubstatesCnt; ++i) state.AddWonState();
    for (int i = 0; i < lostSubstatesCnt; ++i) state.AddLostState();
    return state;
}

GTEST_TEST(DmaCMCTSBase, EvaluateTwoLevelsTreeExpectOnlyRootChildrenAreVisited)
{
    vector<STestState*> leafs;
    STestState rootState;
    leafs.push_back(&rootState.AddWonState().AddWonState());
    leafs.push_back(&rootState.AddWonState().AddLostState());
    leafs.push_back(&rootState.AddLostState().AddLostState());
    CTestMcts mcts = CreateTestMCTS(&rootState);

    EvaluateNTimes(mcts, 3);

    EXPECT_THAT(rootState.m_children,
        testing::Each(TestStateIsVisitedMatcher(true)));
    EXPECT_THAT(leafs,
        testing::Each(TestStateIsVisitedMatcher(false)));
}

GTEST_TEST(DmaCMCTSBase, GetCurrentResultEvaluateTwoLevelsTree100TimesReturnsBestMove)
{
    STestState rootState;
    rootState.AddState(CreateTestStateWithSubstates(5, 0));
    rootState.AddState(CreateTestStateWithSubstates(4, 1));
    rootState.AddState(CreateTestStateWithSubstates(3, 2));
    rootState.AddState(CreateTestStateWithSubstates(2, 3));
    rootState.AddState(CreateTestStateWithSubstates(1, 4));
    rootState.AddState(CreateTestStateWithSubstates(0, 5));
    CTestMcts mcts = CreateTestMCTS(&rootState);
    EvaluateNTimes(mcts, 100);

    STestMove const bestMove = mcts.GetCurrentResult();

    EXPECT_EQ(bestMove, 0);
}

} // mcts
} // dma
} // mimax_test