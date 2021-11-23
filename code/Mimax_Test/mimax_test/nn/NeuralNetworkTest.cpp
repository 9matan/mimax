#include "gtest/gtest.h"

#include <memory>

#include "mimax/nn/Layer.h"
#include "mimax/nn/NeuralNetwork.h"

#include "mimax_mock/nn/LayerMock.h"

namespace mimax_test {
namespace nn {
namespace neural_network {

using mimax::common::CMatrix;

using namespace mimax_mock::nn;
using namespace mimax::nn;
using namespace std;

static unique_ptr<ILayer> CreateEmptyLayerCallActivateAtLeastOnce()
{
    auto layer = make_unique<CLayerNiceMock>();
    EXPECT_CALL(*layer, Activate(testing::_, testing::_))
        .Times(testing::AtLeast(1));
    return layer;
}

static unique_ptr<ILayer> CreateTransformationLayer(size_t const inputSize, size_t const outputSize, CMatrix::Scalar const transformationValue)
{
    auto layer = make_unique<CLayerNiceMock>();
    ON_CALL(*layer, Activate(testing::_, testing::_))
        .WillByDefault(
            [inputSize, outputSize, transformationValue](CMatrix const& input, CMatrix& output)
            {
                EXPECT_EQ(input.GetColsCount(), inputSize);
                output = input * CMatrix(inputSize, outputSize, transformationValue);
            });
    return layer;
}

inline static unique_ptr<ILayer> CreateZeroesTransformationLayer(size_t const inputSize, size_t const outputSize)
{
    return CreateTransformationLayer(inputSize, outputSize, CMatrix::ScalarZero);
}

inline static unique_ptr<ILayer> CreateOnesTransformationLayer(size_t const inputSize, size_t const outputSize)
{
    return CreateTransformationLayer(inputSize, outputSize, CMatrix::ScalarOne);
}

GTEST_TEST(NnCNeuralNetwork, PredictEmptyLayersExpectsEachLayerActivateCall)
{
    vector<unique_ptr<ILayer>> layers;
    layers.emplace_back(CreateEmptyLayerCallActivateAtLeastOnce());
    layers.emplace_back(CreateEmptyLayerCallActivateAtLeastOnce());
    layers.emplace_back(CreateEmptyLayerCallActivateAtLeastOnce());
    CNeuralNetwork network(move(layers));

    network.Predict(CMatrix());
}

GTEST_TEST(NnCNeuralNetwork, PredictZeroesTransformLayersReturnsMatrixWithExpectedDimension)
{
    vector<unique_ptr<ILayer>> layers;
    layers.emplace_back(CreateZeroesTransformationLayer(3, 4));
    layers.emplace_back(CreateZeroesTransformationLayer(4, 5));
    layers.emplace_back(CreateZeroesTransformationLayer(5, 2));
    CNeuralNetwork network(move(layers));
    CMatrix const input(5, 3, CMatrix::ScalarZero);

    CMatrix const output = network.Predict(input);

    EXPECT_EQ(output.GetRowsCount(), 5);
    EXPECT_EQ(output.GetColsCount(), 2);
}

GTEST_TEST(NnCNeuralNetwork, PredictOnesTransformLayersReturnsExpectedMatrix)
{
    vector<unique_ptr<ILayer>> layers;
    layers.emplace_back(CreateOnesTransformationLayer(4, 3));
    layers.emplace_back(CreateOnesTransformationLayer(3, 1));
    CNeuralNetwork network(move(layers));
    CMatrix const input(2, 4, CMatrix::ScalarOne);
    CMatrix const expectedOutput(2, 1, CMatrix::Scalar(12.0f));

    CMatrix const output = network.Predict(input);

    EXPECT_EQ(output, expectedOutput);
}

} // neural_network
} // nn
} // mimax_test