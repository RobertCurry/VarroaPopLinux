#if !defined(AFX_CHART2DCHECKNEWPOINTSRESULT_H__12DF2548_6AF6_11D4_AC53_444553546170__INCLUDED_)
#define AFX_CHART2DCHECKNEWPOINTSRESULT_H__12DF2548_6AF6_11D4_AC53_444553546170__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CChart2DCheckNewPointsResult wrapper class

class CChart2DCheckNewPointsResult : public COleDispatchDriver
{
public:
	CChart2DCheckNewPointsResult() {}		// Calls COleDispatchDriver default constructor
	CChart2DCheckNewPointsResult(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CChart2DCheckNewPointsResult(const CChart2DCheckNewPointsResult& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	BOOL GetIsInside();
	void SetIsInside(BOOL);
	BOOL GetIsOutsideXMin();
	void SetIsOutsideXMin(BOOL);
	BOOL GetIsOutsideXMax();
	void SetIsOutsideXMax(BOOL);
	BOOL GetIsOutsideYMin();
	void SetIsOutsideYMin(BOOL);
	BOOL GetIsOutsideYMax();
	void SetIsOutsideYMax(BOOL);

// Operations
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHART2DCHECKNEWPOINTSRESULT_H__12DF2548_6AF6_11D4_AC53_444553546170__INCLUDED_)
