// VarroaPopDoc.h : interface of the CVarroaPopDoc class
//
/////////////////////////////////////////////////////////////////////////////


#if !defined(AFX_VARROAPOPDOC_H__8C6C418D_7899_11D2_8D9A_0020AF233A70__INCLUDED_)
#define AFX_VARROAPOPDOC_H__8C6C418D_7899_11D2_8D9A_0020AF233A70__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Colony.h"
#include "WeatherEvents.h"
#include "MyPropSheet.h"
#include "Mite.h"
#include "MiteTreatments.h"
#include "Matrix.h"
#include "IEDItem.h"
#include "VarroaPopSession.h"

class CVarroaPopDoc : public CDocument, public CVarroaPopSession, public CVarroaPopSessionBridge
{
protected: // create from serialization only
	CVarroaPopDoc();
	DECLARE_DYNCREATE(CVarroaPopDoc)

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVarroaPopDoc)
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void SetTitle(LPCTSTR lpszTitle);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CVarroaPopDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CVarroaPopDoc)
	afx_msg void OnFileNew();
	afx_msg void OnSelectGraph();
	afx_msg void OnViewPlotdata();
	afx_msg void OnFileSaveResults();
	afx_msg void OnFileSaveSession();
	afx_msg void OnViewCombremovaldate();
	afx_msg void OnUpdateToggleDataFreq(CCmdUI* pCmdUI);
	afx_msg void OnToggleDataFreq();
	afx_msg void OnViewOptions();
	afx_msg void OnWeatherCreatenewweatherfile();
	afx_msg void OnWeatherEditcurrentweatherfile();
	afx_msg void OnWeatherEditweatherfilefromdisk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	afx_msg void OnUpdateViewShowwarnings(CCmdUI *pCmdUI);
	afx_msg void OnToggleShowwarnings();

public:
	// CVarroaPopSessionBridge Implementation
	void SimulationStartUpdated();
	void SimulationEndUpdated();
	void StartSimulation(CVarroaPopSession& session);
	void EndSimulation(CVarroaPopSession& session);
	void ImmigrationEnabled(bool enabled);
	void WeatherFileMissing();
	void WeatherFileLoaded(bool loaded, const CString& filename);
	void SessionFileLoaded(CArchive& ar);
	CString GetDefaultPathName(CArchive& ar);
	void InputFileUnknownVariable(const CString& name);
	void InputFileException(const CString& name);
	void OutputFileException(const CString& name);
	CString GetVersion();
	BOOL CheckDateConsistencyFailed(const CString& warning);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VARROAPOPDOC_H__8C6C418D_7899_11D2_8D9A_0020AF233A70__INCLUDED_)
