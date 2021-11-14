#include <assert.h>
#include <vector>

#include "gtest/gtest.h"

#include "mimax/common/Matrix.h"

namespace mimax_test {
namespace common {
namespace matrix {

    using namespace std;
    using namespace mimax::common;

    static void IsValidIndices_MatrixWithSpecifiedSize_ReturnsExpectedValue(size_t const rowsCnt, size_t const colsCnt, size_t const rIndex, size_t const cIndex, bool const expectedResult)
    {
        CMatrix matrix(rowsCnt, colsCnt);

        bool const isValidIndices = matrix.IsValidIndices(rIndex, cIndex);

        EXPECT_EQ(isValidIndices, expectedResult);
    }

    static void GetRowsCount_MatrixWithSpecifiedSize_ReturnsRowsCount(size_t const rowsCnt, size_t const colsCnt)
    {
        CMatrix matrix(rowsCnt, colsCnt);

        size_t const returnedRowsCount = matrix.GetRowsCount();

        EXPECT_EQ(returnedRowsCount, rowsCnt);
    }

    static void GetColsCount_MatrixWithSpecifiedSize_ReturnsColsCount(size_t const rowsCnt, size_t const colsCnt)
    {
        CMatrix matrix(rowsCnt, colsCnt);

        size_t const returnedColsCount = matrix.GetColsCount();

        EXPECT_EQ(returnedColsCount, colsCnt);
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

#pragma region IsValidIndices
    GTEST_TEST(CommonCMatrix, IsValidIndicesEmptyMatrixReturnsFalse)
    {
        IsValidIndices_MatrixWithSpecifiedSize_ReturnsExpectedValue(0, 0, 0, 0, false);
        IsValidIndices_MatrixWithSpecifiedSize_ReturnsExpectedValue(0, 0, 1, 2, false);
    }

    GTEST_TEST(CommonCMatrix, IsValidIndicesEmptyRowsCountReturnsFalse)
    {
        IsValidIndices_MatrixWithSpecifiedSize_ReturnsExpectedValue(0, 5, 0, 0, false);
        IsValidIndices_MatrixWithSpecifiedSize_ReturnsExpectedValue(0, 5, 0, 1, false);
    }

    GTEST_TEST(CommonCMatrix, IsValidIndicesEmptyColumnsCountReturnsFalse)
    {
        IsValidIndices_MatrixWithSpecifiedSize_ReturnsExpectedValue(5, 0, 0, 0, false);
        IsValidIndices_MatrixWithSpecifiedSize_ReturnsExpectedValue(5, 0, 1, 0, false);
    }

    GTEST_TEST(CommonCMatrix, IsValidIndicesNonEmptyMatrixInvalidIndicesReturnsFalse)
    {
        IsValidIndices_MatrixWithSpecifiedSize_ReturnsExpectedValue(4, 5, 0, 5, false);
        IsValidIndices_MatrixWithSpecifiedSize_ReturnsExpectedValue(4, 5, 4, 0, false);
        IsValidIndices_MatrixWithSpecifiedSize_ReturnsExpectedValue(4, 5, 4, 5, false);
    }

    GTEST_TEST(CommonCMatrix, IsValidIndicesNonEmptyMatrixIndicesOnEdgeReturnsTrue)
    {
        IsValidIndices_MatrixWithSpecifiedSize_ReturnsExpectedValue(4, 5, 0, 0, true);
        IsValidIndices_MatrixWithSpecifiedSize_ReturnsExpectedValue(4, 5, 0, 4, true);
        IsValidIndices_MatrixWithSpecifiedSize_ReturnsExpectedValue(4, 5, 3, 0, true);
        IsValidIndices_MatrixWithSpecifiedSize_ReturnsExpectedValue(4, 5, 3, 4, true);
    }
#pragma endregion IsValidIndices

#pragma region GetRowsCount
    GTEST_TEST(CommonCMatrix, GetRowsCountDefaultConstructorReturnsZero)
    {
        CMatrix matrix;

        size_t const rowsCount = matrix.GetRowsCount();

        EXPECT_EQ(rowsCount, 0);
    }

    GTEST_TEST(CommonCMatrix, GetRowsCountMatrixWithSpecifiedSizeReturnsRowsCount)
    {
        GetRowsCount_MatrixWithSpecifiedSize_ReturnsRowsCount(0, 0);
        GetRowsCount_MatrixWithSpecifiedSize_ReturnsRowsCount(0, 5);
        GetRowsCount_MatrixWithSpecifiedSize_ReturnsRowsCount(4, 5);
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
        GetColsCount_MatrixWithSpecifiedSize_ReturnsColsCount(0, 0);
        GetColsCount_MatrixWithSpecifiedSize_ReturnsColsCount(5, 0);
        GetColsCount_MatrixWithSpecifiedSize_ReturnsColsCount(5, 4);
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

#pragma region AssignmentOperator
    GTEST_TEST(CommonCMatrix, AssignmentOperatorAssignEmptyMatrixMatrixIsEmpty)
    {
        CMatrix matrix(2, 3, 10.0f);
        CMatrix const assignmentMatrix;

        matrix = assignmentMatrix;

        EXPECT_EQ(matrix.GetRowsCount(), 0);
        EXPECT_EQ(matrix.GetColsCount(), 0);
    }

    GTEST_TEST(CommonCMatrix, AssignmentOperatorAssignNonEmptyMatrixMatrixIsSame)
    {
        CMatrix matrix(2, 3, 10.0f);
        CMatrix const assignmentMatrix(5, 4, 3.0f);

        matrix = assignmentMatrix;

        bool const isMatrixSame = matrix == assignmentMatrix;
        EXPECT_TRUE(isMatrixSame);
    }

    GTEST_TEST(CommonCMatrix, AssignmentOperatorAssignNonEmptyMatrixMatrixHasDifferentDataPointer)
    {
        CMatrix matrix(2, 3, 10.0f);
        CMatrix const assignmentMatrix(5, 4, 3.0f);

        matrix = assignmentMatrix;

        bool const isDataDifferent = matrix.begin() != assignmentMatrix.begin();
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

} // matrix
} // common
} // mimax_test