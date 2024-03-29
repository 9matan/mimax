#include <assert.h>
#include <vector>

#include "gtest/gtest.h"

#include "mimax/common/Matrix.h"

namespace mimax_test {
namespace common {
namespace matrix {

    using namespace std;
    using namespace mimax::common;

    static void AreIndicesValid_MatrixWithSpecifiedSize_ReturnsExpectedValue(size_t const rowsCnt, size_t const colsCnt, size_t const rIndex, size_t const cIndex, bool const expectedResult)
    {
        CMatrix matrix(rowsCnt, colsCnt);

        bool const AreIndicesValid = matrix.AreIndicesValid(rIndex, cIndex);

        EXPECT_EQ(AreIndicesValid, expectedResult);
    }

    static void GetRowsCount_MatrixWithSpecifiedSize_ReturnsExpectedValue(size_t const rowsCnt, size_t const colsCnt, size_t const expectedRowsCnt)
    {
        CMatrix matrix(rowsCnt, colsCnt);

        size_t const returnedRowsCount = matrix.GetRowsCount();

        EXPECT_EQ(returnedRowsCount, expectedRowsCnt);
    }

    static void GetColsCount_MatrixWithSpecifiedSize_ReturnsExpectedValue(size_t const rowsCnt, size_t const colsCnt, size_t const expetcedColsCnt)
    {
        CMatrix matrix(rowsCnt, colsCnt);

        size_t const returnedColsCount = matrix.GetColsCount();

        EXPECT_EQ(returnedColsCount, expetcedColsCnt);
    }

    static void Get_Maxtrix10x10Set_ReturnsSameValue(size_t const rIndex, size_t const cIndex, CMatrix::Scalar const value)
    {
        CMatrix matrix(10, 10);
        matrix(rIndex, cIndex) = value;

        auto const gotValue = matrix(rIndex, cIndex);

        EXPECT_EQ(gotValue, value);
    }

    static void EqualityOperator_SpecifiedMatrices_ReturnsExpectedResult(CMatrix const& lhs, CMatrix const& rhs, bool const expectedResult)
    {
        bool const areEqual = lhs == rhs;

        EXPECT_EQ(areEqual, expectedResult);
    }

    static void AdditionOperator_SpecifiedMatrices_ReturnsExpectedMatrix(CMatrix const& a, CMatrix const& b, CMatrix const& expectedMatrix)
    {
        CMatrix const result = a + b;

        EXPECT_EQ(result, expectedMatrix);
    }

#pragma region AreIndicesValid
    GTEST_TEST(CommonCMatrix, AreIndicesValidEmptyMatrixReturnsFalse)
    {
        AreIndicesValid_MatrixWithSpecifiedSize_ReturnsExpectedValue(0, 0, 0, 0, false);
        AreIndicesValid_MatrixWithSpecifiedSize_ReturnsExpectedValue(0, 0, 1, 2, false);
    }

    GTEST_TEST(CommonCMatrix, AreIndicesValidEmptyRowsCountReturnsFalse)
    {
        AreIndicesValid_MatrixWithSpecifiedSize_ReturnsExpectedValue(0, 5, 0, 0, false);
        AreIndicesValid_MatrixWithSpecifiedSize_ReturnsExpectedValue(0, 5, 0, 1, false);
    }

    GTEST_TEST(CommonCMatrix, AreIndicesValidEmptyColumnsCountReturnsFalse)
    {
        AreIndicesValid_MatrixWithSpecifiedSize_ReturnsExpectedValue(5, 0, 0, 0, false);
        AreIndicesValid_MatrixWithSpecifiedSize_ReturnsExpectedValue(5, 0, 1, 0, false);
    }

    GTEST_TEST(CommonCMatrix, AreIndicesValidNonEmptyMatrixInvalidIndicesReturnsFalse)
    {
        AreIndicesValid_MatrixWithSpecifiedSize_ReturnsExpectedValue(4, 5, 0, 5, false);
        AreIndicesValid_MatrixWithSpecifiedSize_ReturnsExpectedValue(4, 5, 4, 0, false);
        AreIndicesValid_MatrixWithSpecifiedSize_ReturnsExpectedValue(4, 5, 4, 5, false);
    }

    GTEST_TEST(CommonCMatrix, AreIndicesValidNonEmptyMatrixIndicesOnEdgeReturnsTrue)
    {
        AreIndicesValid_MatrixWithSpecifiedSize_ReturnsExpectedValue(4, 5, 0, 0, true);
        AreIndicesValid_MatrixWithSpecifiedSize_ReturnsExpectedValue(4, 5, 0, 4, true);
        AreIndicesValid_MatrixWithSpecifiedSize_ReturnsExpectedValue(4, 5, 3, 0, true);
        AreIndicesValid_MatrixWithSpecifiedSize_ReturnsExpectedValue(4, 5, 3, 4, true);
    }
#pragma endregion AreIndicesValid

#pragma region GetRowsCount
    GTEST_TEST(CommonCMatrix, GetRowsCountDefaultConstructorReturnsZero)
    {
        CMatrix matrix;

        size_t const rowsCount = matrix.GetRowsCount();

        EXPECT_EQ(rowsCount, 0);
    }

    GTEST_TEST(CommonCMatrix, GetRowsCountMatrixWithSpecifiedSizeReturnsRowsCount)
    {
        GetRowsCount_MatrixWithSpecifiedSize_ReturnsExpectedValue(0, 0, 0);
        GetRowsCount_MatrixWithSpecifiedSize_ReturnsExpectedValue(0, 5, 0);
        GetRowsCount_MatrixWithSpecifiedSize_ReturnsExpectedValue(5, 0, 0);
        GetRowsCount_MatrixWithSpecifiedSize_ReturnsExpectedValue(4, 5, 4);
    }
#pragma endregion GetRowsCount

#pragma region GetColsCount
    GTEST_TEST(CommonCMatrix, GetColsCountDefaultConstructorReturnsZero)
    {
        CMatrix matrix;

        size_t const colsCount = matrix.GetColsCount();

        EXPECT_EQ(colsCount, 0);
    }

    GTEST_TEST(CommonCMatrix, GetColsCountMatrixWithSpecifiedSizeReturnsColsCount)
    {
        GetColsCount_MatrixWithSpecifiedSize_ReturnsExpectedValue(0, 0, 0);
        GetColsCount_MatrixWithSpecifiedSize_ReturnsExpectedValue(0, 5, 0);
        GetColsCount_MatrixWithSpecifiedSize_ReturnsExpectedValue(5, 0, 0);
        GetColsCount_MatrixWithSpecifiedSize_ReturnsExpectedValue(5, 4, 4);
    }
#pragma endregion GetColsCount

#pragma region EqualityOperator
    GTEST_TEST(CommonCMatrix, EqualityOperatorEmptyMatricesReturnsTrue)
    {
        CMatrix const lhs;
        CMatrix const rhs;

        EqualityOperator_SpecifiedMatrices_ReturnsExpectedResult(lhs, rhs, true);
    }

    GTEST_TEST(CommonCMatrix, EqualityOperatorEmptyAndNonEmptyMatricesReturnsFalse)
    {
        CMatrix const lhs;
        CMatrix const rhs(4, 5);

        EqualityOperator_SpecifiedMatrices_ReturnsExpectedResult(lhs, rhs, false);
        EqualityOperator_SpecifiedMatrices_ReturnsExpectedResult(rhs, lhs, false);
    }

    GTEST_TEST(CommonCMatrix, EqualityOperatorDifferentSizesMatricesReturnsFalse)
    {
        CMatrix const lhs(4, 6);
        CMatrix const rhs(4, 5);

        EqualityOperator_SpecifiedMatrices_ReturnsExpectedResult(lhs, rhs, false);
        EqualityOperator_SpecifiedMatrices_ReturnsExpectedResult(rhs, lhs, false);
    }

    GTEST_TEST(CommonCMatrix, EqualityOperatorSameSizeMatricesDifferentValuesReturnsFalse)
    {
        CMatrix const lhs({
            {1.0f, 2.0f, 3.0f},
            {4.0f, 5.0f, 6.0f}
            });
        CMatrix const rhs({
            {1.0f, 2.0f, -3.0f},
            {4.0f, 5.0f, 6.0f}
            });

        EqualityOperator_SpecifiedMatrices_ReturnsExpectedResult(lhs, rhs, false);
        EqualityOperator_SpecifiedMatrices_ReturnsExpectedResult(rhs, lhs, false);
    }

    GTEST_TEST(CommonCMatrix, EqualityOperatorSameMatricesReturnsTrue)
    {
        CMatrix const lhs({
            {10.0f, 20.0f, 30.0f},
            {40.0f, 50.0f, 60.0f}
            });
        CMatrix const rhs({
            {10.0f, 20.0f, 30.0f},
            {40.0f, 50.0f, 60.0f}
            });

        EqualityOperator_SpecifiedMatrices_ReturnsExpectedResult(lhs, rhs, true);
        EqualityOperator_SpecifiedMatrices_ReturnsExpectedResult(rhs, lhs, true);
    }
#pragma endregion EqualityOperator

#pragma region MoveSemantics
    GTEST_TEST(CommonCMatrix, MoveConstructorMoveNonEmptyMatrixExpectNewMatrixHasSameDataPointer)
    {
        CMatrix matrixToMove(2, 3, CMatrix::ScalarOne);
        auto const matrixData = matrixToMove.data();

        CMatrix const matrix(move(matrixToMove));

        EXPECT_EQ(matrix.data(), matrixData);
        EXPECT_EQ(matrixToMove.data(), nullptr);
    }

    GTEST_TEST(CommonCMatrix, MoveConstructorMoveNonEmptyMatrixExpectNewMatrixIsEqualToMovedOne)
    {
        CMatrix matrixToMove(2, 3, CMatrix::ScalarOne);
        CMatrix const expectedMatrix(2, 3, CMatrix::ScalarOne);

        CMatrix const matrix(move(matrixToMove));

        EXPECT_EQ(matrix, expectedMatrix);
        EXPECT_EQ(matrixToMove, CMatrix());
    }
#pragma endregion MoveSemantics

#pragma region GetElement
    GTEST_TEST(CommonCMatrix, GetElementFillConstructorReturnsAllSameValues)
    {
        CMatrix const matrix(2, 3, 10.0f);

        EXPECT_EQ(matrix(0, 0), 10.0f);
        EXPECT_EQ(matrix(0, 1), 10.0f);
        EXPECT_EQ(matrix(0, 2), 10.0f);
        EXPECT_EQ(matrix(1, 0), 10.0f);
        EXPECT_EQ(matrix(1, 1), 10.0f);
        EXPECT_EQ(matrix(1, 2), 10.0f);
    }

    GTEST_TEST(CommonCMatrix, GetElementSetEdgeElementReturnsSameValue)
    {
        Get_Maxtrix10x10Set_ReturnsSameValue(0, 0, 10.0f);
        Get_Maxtrix10x10Set_ReturnsSameValue(9, 9, -11.0f);
        Get_Maxtrix10x10Set_ReturnsSameValue(9, 0, 12.0f);
        Get_Maxtrix10x10Set_ReturnsSameValue(0, 9, -13.0f);
    }

    GTEST_TEST(CommonCMatrix, GetElementSetNonEdgeElementReturnsSameValue)
    {
        Get_Maxtrix10x10Set_ReturnsSameValue(3, 5, 14.0f);
        Get_Maxtrix10x10Set_ReturnsSameValue(7, 4, -13.0f);
    }
#pragma endregion GetElement

#pragma region Reserve
    GTEST_TEST(CommonCMatrix, ReserveRequestMoreCapacityExpectMatrixIsSame)
    {
        CMatrix matrix({
            {1.0f, 1.5f, -0.5f},
            {0.25f, -0.75f, 0.0f}
            });
        CMatrix const expectedMatrix({
            {1.0f, 1.5f, -0.5f},
            {0.25f, -0.75f, 0.0f}
            });

        matrix.reserve(matrix.capacity() + 1);

        EXPECT_EQ(matrix, expectedMatrix);
    }

    GTEST_TEST(CommonCMatrix, ReserveRequestLessCapacityExpectMatrixIsSame)
    {
        CMatrix matrix({
            {1.0f, 1.5f, -0.5f},
            {0.25f, -0.75f, 0.0f}
            });
        CMatrix const expectedMatrix({
            {1.0f, 1.5f, -0.5f},
            {0.25f, -0.75f, 0.0f}
            });

        matrix.reserve(matrix.capacity() - 1);

        EXPECT_EQ(matrix, expectedMatrix);
    }

    GTEST_TEST(CommonCMatrix, ReserveRequestLessCapacityExpectMatrixHasSameDataPointer)
    {
        CMatrix matrix({
            {1.0f, 1.5f, -0.5f},
            {0.25f, -0.75f, 0.0f}
            });
        auto const dataBeforeReservation = matrix.data();

        matrix.reserve(matrix.capacity() - 1);

        EXPECT_EQ(matrix.data(), dataBeforeReservation);
    }
#pragma endregion Reserve

#pragma region AssignmentOperator
    GTEST_TEST(CommonCMatrix, AssignmentOperatorAssignEmptyMatrixExpectMatrixIsEmpty)
    {
        CMatrix matrix(2, 3, 10.0f);
        CMatrix const assignmentMatrix;

        matrix = assignmentMatrix;

        EXPECT_EQ(matrix.GetRowsCount(), 0);
        EXPECT_EQ(matrix.GetColsCount(), 0);
    }

    GTEST_TEST(CommonCMatrix, AssignmentOperatorAssignNonEmptyMatrixExpectMatrixIsEqualToAssignedOne)
    {
        CMatrix matrix(2, 3, 10.0f);
        CMatrix const assignmentMatrix(5, 4, 3.0f);

        matrix = assignmentMatrix;

        bool const isMatrixSame = matrix == assignmentMatrix;
        EXPECT_TRUE(isMatrixSame);
    }

    GTEST_TEST(CommonCMatrix, AssignmentOperatorAssignNonEmptyMatrixExpectMaxtrixHasDifferentDataPointer)
    {
        CMatrix matrix(2, 3, 10.0f);
        CMatrix const assignmentMatrix(5, 4, 3.0f);

        matrix = assignmentMatrix;

        bool const isDataDifferent = matrix.data() != assignmentMatrix.data();
        EXPECT_TRUE(isDataDifferent);
    }
#pragma endregion AssignmentOperator

#pragma region AdditionOperator
    GTEST_TEST(CommonCMatrix, AdditionOperatorAddEmptyMatricesMatrixIsEmpty)
    {
        CMatrix const a;
        CMatrix const b;

        CMatrix const result = a + b;

        EXPECT_EQ(result.GetRowsCount(), 0);
        EXPECT_EQ(result.GetColsCount(), 0);
    }

    GTEST_TEST(CommonCMatrix, AdditionOperatorAddNonEmptyMatricesReturnsExpectedMatrix)
    {
        CMatrix const a({
            {9.0f, 16.0f, -5.0f},
            {-10.0f, 21.0f, 3.0f}
            });
        CMatrix const b({
            {1.0f, 11.0f, 9.0f},
            {5.0f, -21.0f, 4.0f}
            });
        CMatrix const expectedResult({
            {10.0f, 27.0f, 4.0f},
            {-5.0f, 0.0f, 7.0f}
            });

        AdditionOperator_SpecifiedMatrices_ReturnsExpectedMatrix(a, b, expectedResult);
        AdditionOperator_SpecifiedMatrices_ReturnsExpectedMatrix(b, a, expectedResult);
    }

    GTEST_TEST(CommonCMatrix, AdditionOperatorAddValueReturnsExpectedMatrix)
    {
        CMatrix const a({
            {-2.0f, 6.0f, -8.0f},
            {13.0f, -11.0f, 17.0f}
            });
        CMatrix::Scalar const b = 5.0f;
        CMatrix const expectedResult({
            {3.0f, 11.0f, -3.0f},
            {18.0f, -6.0f, 22.0f}
            });

        CMatrix const result = a + b;

        EXPECT_EQ(result, expectedResult);
    }
#pragma endregion AdditionOperator

#pragma region SubtractionOperator
    GTEST_TEST(CommonCMatrix, SubtractionOperatorSubtractEmptyMatriciesMatrixIsEmpty)
    {
        CMatrix const a;
        CMatrix const b;

        CMatrix const result = a - b;

        EXPECT_EQ(result.GetRowsCount(), 0);
        EXPECT_EQ(result.GetColsCount(), 0);
    }

    GTEST_TEST(CommonCMatrix, SubtractionOperatorSubtractNonEmptyMatricesReturnsExpectedMatrix)
    {
        CMatrix const a({
            {15.0f, -11.0f},
            {8.0f, -13.0f},
            {-1.0f, 5.0f}
            });
        CMatrix const b({
            {-5.0f, 13.0f},
            {8.0f, -7.0f},
            {3.0f, -10.0f}
            });
        CMatrix const expectedResultAB({
            {20.0f, -24.0f},
            {0.0f, -6.0f},
            {-4.0f, 15.0f}
            });
        CMatrix const expectedResultBA({
            {-20.0f, 24.0f},
            {0.0f, 6.0f},
            {4.0f, -15.0f}
            });

        CMatrix const resultAB = a - b;
        CMatrix const resultBA = b - a;

        EXPECT_EQ(resultAB, expectedResultAB);
        EXPECT_EQ(resultBA, expectedResultBA);
    }

    GTEST_TEST(CommonCMatrix, SubtractionOperatorSubtractValueReturnsExpectedMatrix)
    {
        CMatrix const a({
            {15.0f, -11.0f},
            {8.0f, -13.0f},
            {-1.0f, 5.0f}
            });
        CMatrix::Scalar const b = 7.0f;
        CMatrix const expectedResult({
            {8.0f, -18.0f},
            {1.0f, -20.0f},
            {-8.0f, -2.0f}
            });

        CMatrix const result = a - b;

        EXPECT_EQ(result, expectedResult);
    }
#pragma endregion SubtractionOperator

#pragma region MultiplicationOperator
    GTEST_TEST(CommonCMatrix, MultiplicationOperatorMultiplyEmptyMatriciesMatrixIsEmpty)
    {
        CMatrix const a;
        CMatrix const b;

        CMatrix const result = a * b;

        EXPECT_EQ(result.GetRowsCount(), 0);
        EXPECT_EQ(result.GetColsCount(), 0);
    }

    GTEST_TEST(CommonCMatrix, MultiplicationOperatorMultiplyNonEmptyMatricesReturnsExpectedMatrix)
    {
        CMatrix const a({
            {2.0f, -3.0f, 4.0f},
            {-5.0f, 5.0f, 6.0f}
            });
        CMatrix const b({
            {2.0f, -3.0f},
            {4.0f, -5.0f},
            {5.0f, 6.0f}
            });
        CMatrix const expectedResult({
            {12.0f, 33.0f},
            {40.0f, 26.0f}
            });

        CMatrix const result = a * b;

        EXPECT_EQ(result, expectedResult);
    }

    GTEST_TEST(CommonCMatrix, MultiplicationOperatorMultiplyByValueReturnsExpectedMatrix)
    {
        CMatrix const a({
            {2.0f, -3.0f, 4.0f},
            {-5.0f, 5.0f, 6.0f}
            });
        CMatrix::Scalar const b = -2.0f;
        CMatrix const expectedResult({
            {-4.0f, 6.0f, -8.0f},
            {10.0f, -10.0f, -12.0f}
            });

        CMatrix const result = a * b;

        EXPECT_EQ(result, expectedResult);
    }
#pragma endregion MultiplicationOperator

#pragma region Transpose
    GTEST_TEST(CommonCMatrix, TransposeEmptyMatrixReturnsEmptyMatrix)
    {
        CMatrix const matrix;
        CMatrix const expectedResult;

        auto const transposedMatrix = matrix.Transpose();

        EXPECT_EQ(transposedMatrix, expectedResult);
    }

    GTEST_TEST(CommonCMatrix, TransposeOneRowMatrixReturnsOneColumnMatrix)
    {
        CMatrix const matrix({
            {1.0f, -2.0f, 3.0f, -4.0f, 5.0f}
            });
        CMatrix const expectedResult({
            {1.0f},
            {-2.0f},
            {3.0f},
            {-4.0f},
            {5.0f}
            });

        auto const transposedMatrix = matrix.Transpose();

        EXPECT_EQ(transposedMatrix, expectedResult);
    }

    GTEST_TEST(CommonCMatrix, TransposeOneColumnMatrixReturnsOneRowMatrix)
    {
        CMatrix const matrix({
            {-1.0f},
            {2.0f},
            {-3.0f},
            {4.0f},
            {-5.0f}
            });
        CMatrix const expectedResult({
            {-1.0f, 2.0f, -3.0f, 4.0f, -5.0f}
            });

        auto const transposedMatrix = matrix.Transpose();

        EXPECT_EQ(transposedMatrix, expectedResult);
    }

    GTEST_TEST(CommonCMatrix, TransposeMatrixReturnsTransposedMatrix)
    {
        CMatrix const matrix({
            {1.0f, 4.0f, 9.0f},
            {16.0f, 25.0f, 36.0f}
            });
        CMatrix const expectedResult({
            {1.0f, 16.0f},
            {4.0f, 25.0f},
            {9.0f, 36.0f}
            });

        auto const transposedMatrix = matrix.Transpose();

        EXPECT_EQ(transposedMatrix, expectedResult);
    }
#pragma endregion Transpose

#pragma region AddToEachRowInPlace
    GTEST_TEST(CommonCMatrix, AddToEachRowInPlaceAddEmptyVectorToEmptyMatrixReturnsEmptyMatrix)
    {
        CMatrix matrix;
        CMatrix const vectorMatrix = CMatrix::CreateRowVector(0);
        CMatrix const expectedResult;

        matrix.AddToEachRowInPlace(vectorMatrix);

        EXPECT_EQ(matrix, expectedResult);
    }

    GTEST_TEST(CommonCMatrix, AddToEachRowInPlaceAddNonEmptyVectorAndMatrixReturnsExpectedMatrix)
    {
        CMatrix matrix({
            {1.0f, 2.0f, -1.0f},
            {0.5f, -1.5f, -0.5f}
            });
        CMatrix const vectorMatrix = CMatrix::CreateRowVector(
            {-1.0f, 0.5f, 1.0f}
            );
        CMatrix const expectedResult({
            {0.0f, 2.5f, 0.0f},
            {-0.5f, -1.0f, 0.5f}
            });

        matrix.AddToEachRowInPlace(vectorMatrix);

        EXPECT_EQ(matrix, expectedResult);
    }
#pragma endregion AddToEachRowInPlace

} // matrix
} // common
} // mimax_test