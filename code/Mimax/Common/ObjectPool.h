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
    void ResetPool();
    void ResetPool(size_t const poolSize);
    bool IsEmpty() { return m_objects.size() == m_objects.capacity(); }

    template<typename ...TArgs>
    TObject* AllocateObject(TArgs&& ...args);

private:
    std::vector<TObject> m_objects;
};

template<typename TObject>
void CObjectPool<TObject>::ResetPool()
{
    m_objects.clear();
}

template<typename TObject>
void CObjectPool<TObject>::ResetPool(size_t const poolSize)
{
    m_objects.clear();
    m_objects.reserve(poolSize);
}

template<typename TObject>
template<typename ...TArgs>
TObject* CObjectPool<TObject>::AllocateObject(TArgs&& ...args)
{
    assert(m_objects.size() < m_objects.capacity());
    m_objects.emplace_back(std::forward<TArgs>(args)...);
    return &m_objects.back();
}

}