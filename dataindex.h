#if !defined(AFX_DATAINDEX_H__12DF2534_6AF6_11D4_AC53_444553546170__INCLUDED_)
#define AFX_DATAINDEX_H__12DF2534_6AF6_11D4_AC53_444553546170__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CDataIndex wrapper class

class CDataIndex : public COleDispatchDriver
{
public:
	CDataIndex() {}		// Calls COleDispatchDriver default constructor
	CDataIndex(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDataIndex(const CDataIndex& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetSeries();
	void SetSeries(long);
	long GetPoint();
	void SetPoint(long);
	long GetChartGroup();
	void SetChartGroup(long);
	LPDISPATCH GetParent();

// Operations
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAINDEX_H__12DF2534_6AF6_11D4_AC53_444553546170__INCLUDED_)
