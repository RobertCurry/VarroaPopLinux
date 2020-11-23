#include "coblist.h"

CObList::CObList()
{
}

CObList::~CObList()
{
}

INT_PTR CObList::GetCount() const
{
    return m_data.size();   
}

BOOL CObList::IsEmpty() const
{
    return m_data.empty();
}

POSITION CObList::FindIndex(INT_PTR index) const
{
    auto it = m_data.begin();
    std::advance(it, index);
    
    POSITION position; 
    position = std::make_unique<CObListNs::InnerPosition>(it);
    return position;
}

CObject* CObList::GetAt(POSITION position) const
{
    auto it = ext::dynamic_unique_cast<CObListNs::InnerPosition>(position.get());
    return *it->m_it;
}

CObject* CObList::GetHead() const
{
    return m_data.front();
}

CObject* CObList::GetTail() const
{
    return m_data.back();
}

CObject* CObList::GetPrev(POSITION& position) const
{
    auto it = ext::dynamic_unique_cast<CObListNs::InnerPosition>(position.get());
    CObject* prev = *it->m_it;
    if (it->m_it == m_data.begin())
    {
        position = nullptr;
    }
    else 
    {
        it->m_it--;
    }
    return prev;
}

CObject* CObList::GetNext(POSITION& position) const
{
    auto it = ext::dynamic_unique_cast<CObListNs::InnerPosition>(position.get());
    CObject* next = *it->m_it;
    it->m_it++;
    if (it->m_it == m_data.end())
    {
        position = nullptr;
    }
    return next;
}

POSITION CObList::GetHeadPosition() const
{
    POSITION position = nullptr;
    if (GetCount() > 0)
    {
        position = std::make_unique<CObListNs::InnerPosition>(m_data.begin());
    }
    return position;
}

POSITION CObList::GetTailPosition() const
{
    POSITION position = nullptr;
    if (GetCount() > 0)
    {
        auto it = m_data.begin();
        std::advance(it, m_data.size() - 1);
        position = std::make_unique<CObListNs::InnerPosition>(it);
    }
    return position;
}

POSITION CObList::AddHead (CObject* object)
{
    m_data.push_front(object);
    return GetHeadPosition();
}

POSITION CObList::AddTail (CObject* object)
{
    m_data.push_back(object);
    return GetTailPosition();
}

void CObList::RemoveAt(POSITION position)
{
    auto it = ext::dynamic_unique_cast<CObListNs::InnerPosition>(position.get());
    it->m_it = m_data.erase(it->m_it);
}

CObject* CObList::RemoveHead()
{
    CObject* head = GetHead();
    m_data.pop_front();
    return head;
}

CObject* CObList::RemoveTail ()
{
    CObject* tail = GetTail();
    m_data.pop_back();
    return tail;
}

void CObList::RemoveAll()
{
    m_data.clear();
}

CStringList::CStringList()
{
}

CStringList::~CStringList()
{
}

INT_PTR CStringList::GetCount() const
{
    return m_data.size();   
}

BOOL CStringList::IsEmpty() const
{
    return m_data.empty();
}

const CString& CStringList::GetNext(POSITION& position) const 
{
    auto it = ext::dynamic_unique_cast<CStringListNs::InnerPosition>(position.get());
    auto next = std::ref(*it->m_it);
    it->m_it++;
    if (it->m_it == m_data.end())
    {
        position = nullptr;
    }
    return next.get();
}

POSITION CStringList::GetHeadPosition() const
{
    POSITION position = nullptr;
    if (GetCount() > 0)
    {
        position = std::make_unique<CStringListNs::InnerPosition>(m_data.begin());
    }
    return position;
}	

void CStringList::AddTail(const CString& string)
{
    m_data.push_back(string);
}
	
void CStringList::RemoveAll()
{
    m_data.clear();
}

