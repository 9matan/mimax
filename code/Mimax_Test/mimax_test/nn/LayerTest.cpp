#include <algorithm>

#include "gtest/gtest.h"

#include "mimax/nn/PerceptronLayer.h"

#include "mimax_mock/nn/ActivationMock.h"

namespace mimax_test {
namespace nn {
namespace layer {

using mimax::common::CMatrix;

using namespace mimax_mock::nn;
using namespace mimax::nn;
using namespace std;

static unique_ptr<IActivation> CreateDoublingActivation()
{
    auto activation = make_unique<CActivationNiceMock>();
    ON_CALL(*activation, Activate(testing::_))
        .WillByDefault([](CMatrix& matrix) 
            {
                transform(matrix.begin(), matrix.end(), matrix.begin(),
                    [](CMatrix::Scalar const value)
                    {
                        return 2.0f * value;
                    });
            });
    return activation;
}

static unique_ptr<IActivation> CreateEmptyActivation()
{
    auto activation = make_unique<CActivationNiceMock>();
    ON_CALL(*activation, Activate(testing::_))
        .WillByDefault([](CMatrix& ){});
    return activation;
}

static CPerceptronLayer CreatePerceptronLayerWithParamsValue(unique_ptr<IActivation> activation, size_t const inputsCount, size_t const outputsCount, CMatrix::Scalar const paramsValue)
{
    CMatrix const weights(inputsCount, outputsCount, paramsValue);
    CMatrix const biases(1, outputsCount, paramsValue);
    return CPerceptronLayer(move(activation), weights, biases);
}

#pragma region PerceptronLayer
GTEST_TEST(NnCPerceptronLayer, ActivateZeroParametersExpectsActivationActivateCall)
{
    auto activation = make_unique<CActivationNiceMock>();
    EXPECT_CALL(*activation, Activate(testing::_))
        .Times(testing::AtLeast(1));
    auto layer = CreatePerceptronLayerWithParamsValue(move(activation), 2, 3, CMatrix::ScalarZero);
    CMatrix const input(1, 2, CMatrix::ScalarZero);

    CMatrix output;
    layer.Activate(input, output);
}

GTEST_TEST(NnCPerceptronLayer, ActivateZeroParametersReturnsMatrixWithExpectedDimension)
{
    auto layer = CreatePerceptronLayerWithParamsValue(
        CreateEmptyActivation(), 5, 4, CMatrix::ScalarZero);
    CMatrix const input(3, 5, CMatrix::ScalarZero);

    CMatrix output;
    layer.Activate(input, output);

    EXPECT_EQ(output.GetRowsCount(), 3);
    EXPECT_EQ(output.GetColsCount(), 4);
}

GTEST_TEST(NnCPerceptronLayer, ActivateDoublingActivationReturnsExpectedMatrix)
{
    auto layer = CreatePerceptronLayerWithParamsValue(
        CreateDoublingActivation(), 3, 2, CMatrix::ScalarOne);
    CMatrix const input(2, 3, CMatrix::ScalarOne);
    CMatrix const expectedOutput(2, 2, CMatrix::Scalar(8.0f));

    CMatrix output;
    layer.Activate(input, output);

    EXPECT_EQ(output, expectedOutput);
}
#pragma endregion PerceptronLayer

} // layer
} // nn
} // mimax_test