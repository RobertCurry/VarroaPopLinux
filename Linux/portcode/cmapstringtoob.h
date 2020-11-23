#pragma once
#ifndef CMAPSTRINGTOOB_CUSTOM_H
#define CMAPSTRINGTOOB_CUSTOM_H

#include "cobject.h"
#include "cstring.h"

#include "stdafx.h"

#include <cstddef>
#include <map>

/**
 * TODO: Improve the way to replace the behavior the of MFC POSITION structure since
 * this implementation is not efficient (lots of allocations)
 */
namespace CMapStringToObNs { struct InnerPosition : public __POSITION {
	InnerPosition(const typename std::map<CString, CObject*>::const_iterator& it) : m_it(it) {}
	InnerPosition* copy() { return new InnerPosition(m_it); }
    std::map<CString, CObject*>::const_iterator m_it;
}; }

/**
 * Only supports the necessary interface for the good behavior of VarroaPop
 */
class CMapStringToOb : public CObject
{
public:

	BOOL IsEmpty() const;
	POSITION GetStartPosition() const;
	void SetAt(LPCTSTR string, CObject* value);
	void GetNextAssoc(POSITION& position, CString& string, CObject*& value) const;
	BOOL Lookup(LPCTSTR string, CObject*& value) const;
	BOOL RemoveKey(LPCTSTR string);
	void RemoveAll();

protected:

    std::map<CString, CObject*> m_map;
};

#endif // CMAPSTRINGTOOB_CUSTOM_H
