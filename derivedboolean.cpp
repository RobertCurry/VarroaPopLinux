// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "derivedboolean.h"


/////////////////////////////////////////////////////////////////////////////
// CDerivedBoolean properties

BOOL CDerivedBoolean::GetValue()
{
	BOOL result;
	GetProperty(0x1, VT_BOOL, (void*)&result);
	return result;
}

void CDerivedBoolean::SetValue(BOOL propVal)
{
	SetProperty(0x1, VT_BOOL, propVal);
}

BOOL CDerivedBoolean::GetIsDefault()
{
	BOOL result;
	GetProperty(0x2, VT_BOOL, (void*)&result);
	return result;
}

void CDerivedBoolean::SetIsDefault(BOOL propVal)
{
	SetProperty(0x2, VT_BOOL, propVal);
}

LPDISPATCH CDerivedBoolean::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x3, VT_DISPATCH, (void*)&result);
	return result;
}

/////////////////////////////////////////////////////////////////////////////
// CDerivedBoolean operations
