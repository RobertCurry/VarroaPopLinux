#pragma once
#ifndef CDIALOG_CUSTOM_H
#define CDIALOG_CUSTOM_H

#define IDOK 1
#define IDNO 7
#define IDD_CREATE_HEADER 134
#define IDD_PROGRESS 0
#define IDC_PROGRESS 0

#define IDC_MINTEMP 0
#define IDC_MAXTEMP 0
#define IDC_W_BEG_TIME 0
#define IDC_W_END_TIME 0
#define IDC_W_BEG_DATE 0
#define IDC_W_END_DATE 0
#define IDC_SIM_TIME 0
#define IDC_SIM_DATE 0
#define IDC_SOLAR_RAD 0

#define ON_BN_CLICKED(_1, _2) // void

#define _T(x) x

#include "ccmdtarget.h"
#include "cstring.h"

#include <cstddef>

class CDataExchange;

/**
 * Empty class to be able to compile the VarroaPop application
 */
class CPoint
{
public:
	CPoint(uint32_t x, uint32_t y);
};

/**
 * Empty class to be able to compile the VarroaPop application
 */
class CWnd : public CCmdTarget
{
public:
	virtual size_t DoModal();
	virtual void DoDataExchange(CDataExchange*);
	CWnd* GetDlgItem(int nID) const;
	bool EnableWindow(bool bEnable = true);
	bool UpdateData(bool bSaveAndValidate = true);
	void DestroyWindow();
};

/**
 * Empty class to be able to compile the VarroaPop application
 */
class CDialog : public CWnd
{
public:
	CDialog();
	CDialog(int32_t id, CWnd* pParent);
	virtual void Create(int32_t nIDTemplate);
	virtual void Create(int32_t nIDTemplate, CWnd* pParentWnd);
	bool SetWindowText(const CString& text);
	void OnOK();
	void OnCancel();
};

class CProgressCtrl : public CWnd
{
public:
	void StepIt();
};

#endif // CDIALOG_CUSTOM_H
