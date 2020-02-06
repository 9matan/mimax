#pragma once

#include <assert.h>
#include <vector>
#include <utility>

namespace mimax
{

template<typename TObject>
class CObjectPool
{
public:
    void ResetPool()
    {
        m_objects.clear();
    }
    void ResetPool(size_t const poolSize)
    {
        m_objects.clear();
        m_objects.reserve(poolSize);
    }
    bool IsEmpty() { return m_objects.size() == m_objects.capacity(); }

    void FreeLastObject()
    {
        assert(!m_objects.empty());
        m_objects.pop_back();
    }

    template<typename ...TArgs>
    TObject* AllocateObject(TArgs&& ...args)
    {
        assert(m_objects.size() < m_objects.capacity());
        m_objects.emplace_back(std::forward<TArgs>(args)...);
        return &m_objects.back();
    }

private:
    std::vector<TObject> m_objects;
};

}