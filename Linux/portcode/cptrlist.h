#pragma once
#ifndef CPTRLIST_CUSTOM_H
#define CPTRLIST_CUSTOM_H

#include "cobject.h"

#include <cstddef>
#include <memory>
#include <list>

/**
 * TODO: Improve the way to replace the behavior the of MFC POSITION structure since
 * this implementation is not efficient (lots of allocations)
 */
namespace CPtrListNs { template<class TYPE> struct InnerPosition : public __POSITION {
	InnerPosition(const typename std::list<TYPE>::const_iterator& it) : m_it(it) {}
	InnerPosition* copy() { return new InnerPosition(m_it); }
    typename std::list<TYPE>::const_iterator m_it;
}; }

/**
 * Only supports the necessary interface for the good behavior of VarroaPop
 */
class CPtrList : public CObject
{
    public:
    
};

/**
 * Only supports the necessary interface for the good behavior of VarroaPop
 */
template<class BASE_CLASS, class TYPE>
class CTypedPtrList
{
public:

	CTypedPtrList();
	~CTypedPtrList();

	BOOL IsEmpty() const;
	INT_PTR GetCount() const;
	TYPE GetAt(POSITION position) const;
	POSITION AddTail(TYPE object);
	void RemoveAt(POSITION position);
	POSITION GetHeadPosition() const;
	POSITION GetTailPosition() const;
	TYPE GetHead() const;
	TYPE GetTail() const;
	TYPE GetNext(POSITION& position) const;
	TYPE RemoveHead();
	void RemoveAll();

protected:

    std::list<TYPE> m_data;
};

#include "cptrlist.inline.h"

#endif // CPTRLIST_CUSTOM_H
