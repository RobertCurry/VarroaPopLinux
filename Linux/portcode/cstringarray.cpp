#include "cstringarray.h"

INT_PTR CStringArray::GetSize() const
{
    return m_data.size();
}

INT_PTR CStringArray::GetCount() const
{
    return m_data.size();
}

BOOL CStringArray::IsEmpty() const
{
    return m_data.empty();
}

INT_PTR CStringArray::GetUpperBound() const
{
    INT_PTR upperBound = -1;
    if (m_data.size() > 0)
    {
        upperBound = m_data.size() - 1;
    }
    return upperBound;
}

void CStringArray::SetSize(INT_PTR nNewSize, INT_PTR /*nGrowBy*/)
{
    m_data.resize(nNewSize);
}

CString CStringArray::GetAt(INT_PTR position)
{
    return m_data.at(position);
}

void CStringArray::Add(const CString& str)
{
    m_data.push_back(str);
}

const CString& CStringArray::operator[](INT_PTR index) const
{
    return m_data[index];
}

CString& CStringArray::operator[](INT_PTR index)
{
    return m_data[index];
}

void CStringArray::RemoveAll()
{
    m_data.clear();
}