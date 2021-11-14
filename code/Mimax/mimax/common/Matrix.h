#include <assert.h>
#include <initializer_list>

namespace mimax {
namespace common {

class CMatrix
{
public:
    using Scalar = float;

public:
    static CMatrix Identity(size_t const size);

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

    CMatrix Transpose() const;

    inline size_t GetRowsCount() const { return m_sizes[0]; }
    inline size_t GetColsCount() const { return m_sizes[1]; }

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