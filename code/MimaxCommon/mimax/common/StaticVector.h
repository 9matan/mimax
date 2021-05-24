#pragma once

#include <algorithm>
#include <assert.h>

namespace mimax {
namespace common {

template<typename T, size_t TSize>
class CStaticVector
{
public:
    static const size_t MAX_SIZE = TSize;

public:
    CStaticVector() : m_nxtSlotIndex(0) {}
    CStaticVector(size_t const size) { resize(size); }
    CStaticVector(std::initializer_list<T> const& items) 
    { 
        resize(items.size()); 
        std::copy(items.begin(), items.end(), begin());
        m_nxtSlotIndex = items.size();
    }

    inline void push_back(T const& elem) { assert(m_nxtSlotIndex < TSize); m_data[m_nxtSlotIndex++] = elem; }
    template<typename ... TArgs>
    inline T& emplace_back(TArgs&& ... args) { m_data[m_nxtSlotIndex++] = std::move(T(std::forward<TArgs>(args)...)); return back(); }
    inline void pop_back() { m_nxtSlotIndex--; }
    inline T& back() { return m_data[m_nxtSlotIndex - 1]; }
    inline T& front() { return *m_data; }
    inline T const& back() const { return m_data[m_nxtSlotIndex - 1]; }
    inline T const& front() const { return *m_data; }
    inline size_t size() const { return m_nxtSlotIndex; }
    inline void resize(size_t const capacity) { m_nxtSlotIndex = capacity; }
    inline void reserve(size_t const capacity) { assert(capacity <= TSize); }
    inline size_t max_capacity() const { return TSize; }
    inline bool is_full() const { return m_nxtSlotIndex >= TSize; }
    void erase_swap(T* const iter)
    {
        size_t const index = std::distance(m_data, iter);
        std::swap(m_data[index], m_data[m_nxtSlotIndex - 1]);
        pop_back();
    }

    inline bool empty() const { return m_nxtSlotIndex == 0; }
    inline void clear() { m_nxtSlotIndex = 0; }

    inline T* begin() { return m_data; }
    inline T* end() { return m_data + m_nxtSlotIndex; }
    inline T const* begin() const { return m_data; }
    inline T const* end() const { return m_data + m_nxtSlotIndex; }

    inline T& operator[](size_t const index) { assert(index < m_nxtSlotIndex); return m_data[index]; }
    inline T const& operator[](size_t const index) const { assert(index < m_nxtSlotIndex); return m_data[index]; }

private:
    size_t m_nxtSlotIndex = 0;
    T m_data[TSize];
};

}
}