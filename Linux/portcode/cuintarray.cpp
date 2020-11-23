#include "cuintarray.h"

INT_PTR CUIntArray::GetSize() const
{
    return m_data.size();
}

UINT CUIntArray::GetAt(INT_PTR index) const
{
    return m_data.at(index);
}

void CUIntArray::Add(UINT eventId)
{
    m_data.push_back(eventId);
}

void CUIntArray::RemoveAt(UINT index)
{
    auto it = m_data.begin();
    std::advance(it, index);
    m_data.erase(it);
}

void CUIntArray::RemoveAll()
{
    m_data.clear();
}
