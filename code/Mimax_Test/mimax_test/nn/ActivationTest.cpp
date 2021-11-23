#include "gtest/gtest.h"

#include "mimax/nn/ActivationReLU.h"

namespace mimax_test {
namespace nn {
namespace activation {

using namespace mimax::nn;

using CMatrix = mimax::common::CMatrix;

#pragma region CActivationReLU
GTEST_TEST(NnCActivationReLU, ActivateZoreMatrixReturnsZeroMatrix)
{
    CMatrix matrix(4, 4, CMatrix::ScalarZero);
    CMatrix const expectedResult(4, 4, CMatrix::ScalarZero);
    CActivationReLU activation;

    activation.Activate(matrix);

    EXPECT_EQ(matrix, expectedResult);
}

GTEST_TEST(NnCActivationReLU, ActivateNonPositiveMatrixReturnsZeroMatrix)
{
    CMatrix matrix({
        {-1.5f, -0.5f, 0.0f, -0.25f},
        {0.0f, -0.5f, 0.0f, -0.25f},
        {0.0f, -0.57f, -1.0f, -0.45f}
        });
    CMatrix const expectedResult(3, 4, CMatrix::ScalarZero);
    CActivationReLU activation;

    activation.Activate(matrix);

    EXPECT_EQ(matrix, expectedResult);
}

GTEST_TEST(NnCActivationReLU, ActivateMatrixReturnsZeroInsteadOfNegativeValuesMatrix)
{
    CMatrix matrix({
        {1.5f, -0.5f, 0.0f, -0.25f},
        {0.15f, -0.5f, 0.0f, 0.25f},
        {0.0f, -0.57f, 1.0f, -0.45f}
        });
    CMatrix const expectedResult({
        {1.5f, 0.0f, 0.0f, 0.0f},
        {0.15f, 0.0f, 0.0f, 0.25f},
        {0.0f, 0.0f, 1.0f, 0.0f}
        });
    CActivationReLU activation;

    activation.Activate(matrix);

    EXPECT_EQ(matrix, expectedResult);
}
#pragma endregion CActivationReLU

} // activation
} // nn
} // mimax_test