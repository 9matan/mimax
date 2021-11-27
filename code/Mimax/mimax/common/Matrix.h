#pragma once
#include <assert.h>
#include <initializer_list>
#include <vector>

namespace mimax {
namespace common {

class CMatrix
{
public:
    using Scalar = float;

    static constexpr Scalar ScalarZero = Scalar(0);
    static constexpr Scalar ScalarOne = Scalar(1);

public:
    static CMatrix Identity(size_t const size);

    static CMatrix CreateRowVector(size_t const size);
    static CMatrix CreateRowVector(std::initializer_list<Scalar> const& initList);
    inline static bool IsRowVector(CMatrix const& matrix) { return matrix.empty() || matrix.GetRowsCount() == 1; }

public:
    CMatrix();
    CMatrix(CMatrix const& other);
    CMatrix(size_t const rowsCnt, size_t const colsCnt);
    CMatrix(size_t const rowsCnt, size_t const colsCnt, Scalar const initValue);
    CMatrix(std::initializer_list<std::initializer_list<Scalar>> const& initList);
    ~CMatrix();

    CMatrix& operator=(CMatrix const& other);
    CMatrix operator+(CMatrix const& other) const;
    CMatrix operator+(Scalar const value) const;
    CMatrix operator-(CMatrix const& other) const;
    CMatrix operator-(Scalar const value) const;
    CMatrix operator*(CMatrix const& other) const;
    CMatrix operator*(Scalar const value) const;

    bool operator==(CMatrix const& other) const;
    inline bool operator!=(CMatrix const& other) const { return !(*this == other); }

    inline Scalar& operator()(size_t const rowIndex, size_t const colIndex) { return m_data[GetDataIndex(rowIndex, colIndex)]; }
    inline Scalar const& operator()(size_t const rowIndex, size_t const colIndex) const { return m_data[GetDataIndex(rowIndex, colIndex)]; }

    CMatrix AddToEachRow(CMatrix const& rowVectorMatrix) const;
    CMatrix& AddToEachRowInPlace(CMatrix const& rowVectorMatrix);
    CMatrix Transpose() const;

    inline size_t GetRowsCount() const { return m_sizes[0]; }
    inline size_t GetColsCount() const { return m_sizes[1]; }

    inline bool empty() const { return size() == 0; }
    inline size_t size() const { return GetRowsCount() * GetColsCount(); }
    inline Scalar* begin() { return m_data; }
    inline Scalar* begin() const { return m_data; }
    inline Scalar* end() { return m_data + size(); }
    inline Scalar* end() const { return m_data + size(); }

    inline bool IsValidIndices(size_t const rowIndex, size_t const colIndex) const
    {
        return rowIndex < GetRowsCount() && colIndex < GetColsCount();
    }

private:
    Scalar* m_data;
    size_t m_sizes[2];

private:
    void Resize(size_t const rowsCnt, size_t const colsCnt);
    void DeleteDataIfExists();
    void CreateData(size_t const rowsCnt, size_t const colsCnt);
    void ResetData();
    void CopyFrom(std::initializer_list<std::initializer_list<Scalar>> const& initList);
    void CopyFrom(CMatrix const& matrix);
    void Fill(Scalar const value);
    void SetSizes(size_t const rowsCnt, size_t const colsCnt);

    inline size_t GetDataIndex(size_t const rowIndex, size_t const colIndex) const
    {
        assert(IsValidIndices(rowIndex, colIndex));
        return rowIndex * GetColsCount() + colIndex;
    }
};

} // common
} // mimax