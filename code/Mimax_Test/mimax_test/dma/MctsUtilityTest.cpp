#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "mimax/dma/MctsUtility.h"

namespace mimax_test {
namespace dma {
namespace mcts_utility {

using namespace mimax::dma;
using namespace mimax::dma::mcts_utility;

using SNodeEvalResult = SMctsNodeEvalResult<int>;

static void AddChildInfo(SNodeEvalResult& evalResult, int const move, unsigned int const simulationsCnt)
{
    SNodeEvalResult::SChildInfo childInfo;
    childInfo.m_move = move;
    childInfo.m_simulationsCount = simulationsCnt;

    evalResult.m_childrenInfo.push_back(childInfo);
}

GTEST_TEST(DmaMctsUtility, CombineEvalResultsEmptyEvalsExpectEmptyResult)
{
    SNodeEvalResult result;
    SNodeEvalResult resultToAdd;

    CombineEvalResults(result, resultToAdd);

    EXPECT_THAT(result.m_childrenInfo, testing::IsEmpty());
}

GTEST_TEST(DmaMctsUtility, CombineEvalResultsEvalsHaveSameMovesExpectResultWithSameMoves)
{
    SNodeEvalResult result;
    AddChildInfo(result, 2, 5);
    AddChildInfo(result, 4, 2);
    SNodeEvalResult resultToAdd;
    AddChildInfo(resultToAdd, 2, 3);
    AddChildInfo(resultToAdd, 4, 1);
    SNodeEvalResult expectedCombinedResult;
    AddChildInfo(expectedCombinedResult, 2, 8);
    AddChildInfo(expectedCombinedResult, 4, 3);

    CombineEvalResults(result, resultToAdd);

    EXPECT_THAT(result.m_childrenInfo, testing::SizeIs(2));
    EXPECT_THAT(result.m_childrenInfo,
        testing::UnorderedElementsAreArray(
            expectedCombinedResult.m_childrenInfo.begin(),
            expectedCombinedResult.m_childrenInfo.end()));
}

GTEST_TEST(DmaMctsUtility, CombineEvalResultsEvalsHaveDiffMovesExpectResultWithAllMoves)
{
    SNodeEvalResult result;
    AddChildInfo(result, 5, 2);
    AddChildInfo(result, 2, 3);
    SNodeEvalResult resultToAdd;
    AddChildInfo(resultToAdd, 2, 2);
    AddChildInfo(resultToAdd, 7, 3);
    SNodeEvalResult expectedCombinedResult;
    AddChildInfo(expectedCombinedResult, 2, 5);
    AddChildInfo(expectedCombinedResult, 5, 2);
    AddChildInfo(expectedCombinedResult, 7, 3);

    CombineEvalResults(result, resultToAdd);

    EXPECT_THAT(result.m_childrenInfo, testing::SizeIs(3));
    EXPECT_THAT(result.m_childrenInfo,
        testing::UnorderedElementsAreArray(
            expectedCombinedResult.m_childrenInfo.begin(),
            expectedCombinedResult.m_childrenInfo.end()));
}

GTEST_TEST(DmaMctsUtility, GetBestMoveValidEvalReturnsExpectedMove)
{
    SNodeEvalResult result;
    AddChildInfo(result, 1, 3);
    AddChildInfo(result, 3, 1);
    AddChildInfo(result, 2, 4);

    auto const bestMove = GetBestMove(result);

    EXPECT_EQ(bestMove, 2);
}

} // mcts_utility
} // dma
} // mimax_test