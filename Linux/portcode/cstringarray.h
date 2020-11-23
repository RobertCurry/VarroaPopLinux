#pragma once
#ifndef CSTRINGARRAY_CUSTOM_H
#define CSTRINGARRAY_CUSTOM_H

#include "cobject.h"
#include "cstring.h"

#include <cstdint>
#include <vector>

/**
 * Only supports the necessary interface for the good behavior of VarroaPop
 */
class CStringArray : public CObject
{
public:

	INT_PTR GetSize() const;
	INT_PTR GetCount() const;
	BOOL IsEmpty() const;
	INT_PTR GetUpperBound() const;
	void SetSize(INT_PTR nNewSize, INT_PTR nGrowBy = -1);

	CString GetAt(INT_PTR position);
	void Add(const CString& str);

	const CString& operator[](INT_PTR index) const;
	CString& operator[](INT_PTR index);

	void RemoveAll();

protected:

    std::vector<CString> m_data;
}; 


#endif // CSTRINGARRAY_CUSTOM_H
