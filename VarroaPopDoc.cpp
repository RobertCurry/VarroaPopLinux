// VarroaPopDoc.cpp : implementation of the CVarroaPopDoc class
//

#include "stdafx.h"
#include "VarroaPop.h"

#include "VarroaPopDoc.h"
#include "VarroaPopView.h"
#include "MainFrm.h"
#include "SelGraph.h"
#include "CombRemDlg.h"
#include "Options.h"
#include "WeatherEditor.h"
#include "afxcoll.h"
#include  <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVarroaPopDoc

IMPLEMENT_DYNCREATE(CVarroaPopDoc, CDocument)

BEGIN_MESSAGE_MAP(CVarroaPopDoc, CDocument)
	//{{AFX_MSG_MAP(CVarroaPopDoc)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_SELECT_GRAPH, OnSelectGraph)
	ON_COMMAND(ID_VIEW_PLOTDATA, OnViewPlotdata)
	ON_COMMAND(ID_FILE_SAVE_RESULTS, OnFileSaveResults)
	ON_COMMAND(ID_FILE_SAVE_SESSION, OnFileSaveSession)
	ON_COMMAND(ID_VIEW_COMBREMOVALDATE, OnViewCombremovaldate)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DATA_FREQ, OnUpdateToggleDataFreq)
	ON_COMMAND(ID_VIEW_DATA_FREQ, OnToggleDataFreq)
	ON_COMMAND(ID_VIEW_OPTIONS, OnViewOptions)
	ON_COMMAND(ID_WEATHER_CREATENEWWEATHERFILE, OnWeatherCreatenewweatherfile)
	ON_COMMAND(ID_WEATHER_EDITCURRENTWEATHERFILE, OnWeatherEditcurrentweatherfile)
	ON_COMMAND(ID_WEATHER_EDITWEATHERFILEFROMDISK, OnWeatherEditweatherfilefromdisk)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWWARNINGS, &CVarroaPopDoc::OnUpdateViewShowwarnings)
	ON_COMMAND(ID_VIEW_SHOWWARNINGS, &CVarroaPopDoc::OnToggleShowwarnings)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVarroaPopDoc construction/destruction

CVarroaPopDoc::CVarroaPopDoc()
{
	CVarroaPopApp* theApp = (CVarroaPopApp*)AfxGetApp();

	SetBridge(this);

	// Set Default Path name = App path
	CString modulename = CString(((CVarroaPopApp*)AfxGetApp())->GetModuleFileName());
	SetDefaultPathName(SplitPath(modulename,DRV) + SplitPath(modulename,DIR));

	// Find all colony description files and build the list
	CFileFind finder;
	CString name;
	int ok = finder.FindFile("*.col");  // colony files
	int i;
	bool nullify;
	while(ok) {
		ok = finder.FindNextFile();
		name = finder.GetFileName();
		// strip the extension and add the name to the list
		nullify = false;
		for(i=0; i<name.GetLength(); i++) {
			if((nullify) || (name.GetAt(i) == '.')) {
				name.SetAt(i, ' ');
				nullify = true;
			}
			name.TrimRight();
		}
		getAvailableFilesList()->AddTail(name);
	}
}

CVarroaPopDoc::~CVarroaPopDoc()
{
}



BOOL CVarroaPopDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	SetModifiedFlag(FALSE);
	SetTitle("Untitled");
	GetColony()->Clear();
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CVarroaPopDoc serialization

void CVarroaPopDoc::Serialize(CArchive& ar)
{
	CVarroaPopSession::Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CVarroaPopDoc diagnostics

#ifdef _DEBUG
void CVarroaPopDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CVarroaPopDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVarroaPopDoc commands

void CVarroaPopDoc::SetTitle(LPCTSTR lpszTitle) 
{
	
	CDocument::SetTitle(lpszTitle);
}


void CVarroaPopDoc::OnFileNew() 
{
	POSITION pos = GetFirstViewPosition();
	CVarroaPopView* pFirstView = (CVarroaPopView*)GetNextView( pos );

	if (((GetWeather()->IsInitialized())||(GetColony()->IsInitialized()))&&IsModified())
	{
		//  Give opportunity to save old file

		if (MyMessageBox("Save Changes to "+GetPathName()+"?\n",MB_YESNO) == IDYES)
		{
			CFileDialog MyFileDialog(	FALSE,
									".vrp",
									GetPathName(),
									OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
									_T("VarroaPop Files (*.vrp)|*.vrp|All Files|*.*||"));

			if (MyFileDialog.DoModal() == IDOK) OnFileSaveSession();
		}
	}
	OnNewDocument();

	pFirstView->ShowPropsheet();
	
}


void CVarroaPopDoc::OnSelectGraph() 
{
	// We get here from the toolbar selection
	CSelGraph SelectGraph(this);
	SelectGraph.DoModal();
	int i = 0;

	
}

void CVarroaPopDoc::OnViewPlotdata() 
{
	//  We get here from the menu selection
	OnSelectGraph();
	
}


void CVarroaPopDoc::OnFileSaveResults() 
{
	
	CFileDialog MyFileDialog(	FALSE,
								".res",
								NULL,
								OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
								_T("Simulation Results Files (*.res)|*.res|All Files|*.*||"));

	if (MyFileDialog.DoModal() == IDOK)
	{
		StoreResultsFile(MyFileDialog.GetPathName());
	}
}



void CVarroaPopDoc::OnFileSaveSession() 
{
	OnSaveDocument(GetPathName());
}

void CVarroaPopDoc::OnViewCombremovaldate() 
{
	CCombRemDlg MyDialog;
	MyDialog.m_RemovalDate = m_CombRemoveDate;
	MyDialog.m_IsEnabled = m_CombRemoveEnable;
	MyDialog.m_Percent = m_CombRemovePct;

	if (MyDialog.DoModal() == IDOK)
	{
		m_CombRemoveDate = MyDialog.m_RemovalDate;
		m_CombRemoveEnable = MyDialog.m_IsEnabled;
		m_CombRemovePct = MyDialog.m_Percent;
	}

	
}


void CVarroaPopDoc::OnUpdateToggleDataFreq(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	//if (!m_DispWeeklyData) pCmdUI->SetText("Weekly Data");
	//else pCmdUI->SetText("Daily Data");
	if (m_DispWeeklyData) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}

void CVarroaPopDoc::OnToggleDataFreq() 
{
	m_DispWeeklyData = !m_DispWeeklyData;
}



void CVarroaPopDoc::OnViewOptions() 
{
	COptions OptDlg(this);

	OptDlg.DoModal();

	
}

void CVarroaPopDoc::OnWeatherCreatenewweatherfile() 
{
	// Creates a new CWeatherEvents and sends it to the weather editor
	// Upon completion of editing, if user closed the editor with OK and 
	// has not yet saved the weather file, prompt for saving and prompt to make
	// this the current weather for the session

	CWeatherEvents* pWeather = new CWeatherEvents;
	CWeatherEditor WeatherEditor(pWeather);
	if (WeatherEditor.DoModal() == IDOK)
	{
		if(WeatherEditor.IsModified())
		{
			// Prompt to set this as the active weather file and prompt to save
			if(AfxMessageBox("Save Weather File ?",MB_YESNO) == IDYES)
			{
				// Save weather
				// TODO - Add file save dialog
				if (AfxMessageBox("Replace current weather with this one ?",MB_YESNO) == IDYES)
				{
					if (AfxIsValidAddress(m_pWeather,sizeof(CWeatherEvents))) delete m_pWeather;
					m_pWeather = pWeather;
				}
				else delete pWeather;
			}
		}
	}
	
}

void CVarroaPopDoc::OnWeatherEditcurrentweatherfile() 
{
	CWeatherEditor WeatherEditor(m_pWeather);
	if (WeatherEditor.DoModal() == IDOK)
	{
		if(WeatherEditor.IsModified())
		{
			// Prompt to save
			// TODO - Add file save dialog
		}
	}
	
}

void CVarroaPopDoc::OnWeatherEditweatherfilefromdisk() 
{
	// Loads a weather file from disk and edits.  After editing, prompts to
	// save and to make current
	CFileDialog fd(	TRUE,
					".wth",
					NULL,
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					_T("Weather Files (*.wth)|*.wth|All Files|*.*||"));

	if (fd.DoModal() == IDOK)
	{
		CString FileName = fd.GetPathName();
		CWeatherEvents* pWeather = new CWeatherEvents;
		if (pWeather->LoadWeatherFile(FileName))
		{
			CWeatherEditor WeatherEditor(pWeather);
			if (WeatherEditor.DoModal() == IDOK)
			{
				if (WeatherEditor.IsModified())
				{
					// Prompt to save file
					// TODO - Add File Save dialog
					if (AfxMessageBox("Replace current weather with this one ?",MB_YESNO) == IDYES)
					{
						if (AfxIsValidAddress(m_pWeather,sizeof(CWeatherEvents))) delete m_pWeather;
						m_pWeather = pWeather;
					}
					else delete pWeather;
				}
			}
		}
	}
}



BOOL CVarroaPopDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  Add your specialized creation code here

	return TRUE;
}



void CVarroaPopDoc::OnUpdateViewShowwarnings(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	if (IsShowWarnings()) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}


void CVarroaPopDoc::OnToggleShowwarnings()
{
	SetShowWarnings(!IsShowWarnings()); // Toggle
}

/////////////////////////////////////////////////////////////////////////////
// CVarroaPopSessionBridge Implementation

void CVarroaPopDoc::SimulationStartUpdated()
{
	if (gl_RunGUI)
	{
		if (IsWeatherLoaded()) ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->InitializeDateCtrls();
	}
}

void CVarroaPopDoc::SimulationEndUpdated() 
{
	if (gl_RunGUI)
	{
		if (IsWeatherLoaded()) ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->InitializeDateCtrls();
	}
}


void CVarroaPopDoc::StartSimulation(CVarroaPopSession& session)
{
	BeginWaitCursor();
}

void CVarroaPopDoc::EndSimulation(CVarroaPopSession& session)
{
	if (gl_RunGUI) UpdateAllViews(NULL);
	else
	{
		// This code generates an execption on Julien's computer we need to figure out what it does and why
		POSITION pos = GetFirstViewPosition();
		CVarroaPopView* pView = (CVarroaPopView*)GetNextView(pos);
		pView->ChartData(this, gl_RunGUI);
	}

	// If command line switch has /or then save results file
	CString ResultsFileName = ((CVarroaPopApp*)AfxGetApp())->m_OutputResultsFileName;
	if (ResultsFileName.GetLength() > 0) StoreResultsFile(ResultsFileName);

	EndWaitCursor();
}

void CVarroaPopDoc::ImmigrationEnabled(bool enabled)
{
	if ((enabled) && (gl_RunGUI))
		((CMainFrame*)(AfxGetApp()->m_pMainWnd))->SetImmigration(true);
	else if (gl_RunGUI)
		((CMainFrame*)(AfxGetApp()->m_pMainWnd))->SetImmigration(false);
}

void CVarroaPopDoc::WeatherFileMissing()
{
	if (gl_RunGUI)
	{
		CString msg = "  This Session has no associated Weather File\n";
		msg += "You will have to specify one before you run a simulation";
		MyMessageBox(msg);
		((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_WeatherFileName = "";
	}
	else
	{
		// Weather file might be specified in the input file
	}
}

void CVarroaPopDoc::WeatherFileLoaded(bool loaded, const CString& filename)
{
	if (loaded)
	{
		if (gl_RunGUI)
			((CMainFrame*)(AfxGetApp()->m_pMainWnd))->InitializeDateCtrls();
	}
	else
	{
		CString msg = "  Reading Session File: The Specified Weather file  ";
		msg += filename + " was not found\n";
		msg += "You will have to specify one before you run a simulation";
		MyMessageBox(msg);
	}
}

void CVarroaPopDoc::SessionFileLoaded(CArchive& ar)
{
	// After reading .vrp file, update any variables which were changed
	// by the command line input file
	CString InputFileName = ((CVarroaPopApp*)AfxGetApp())->m_InputFileName;
	if ((!ar.IsStoring()) & (InputFileName.GetLength() > 0))
	{
		ProcessInputFile(InputFileName);
	}

	if (gl_RunGUI)
		((CMainFrame*)(AfxGetApp()->m_pMainWnd))->EnableDialogBar(ReadyToSimulate());
}

CString CVarroaPopDoc::GetDefaultPathName(CArchive& ar)
{
	// Set Default Path name = SessionFile Path
	CString pathname = ar.GetFile()->GetFilePath();
	return (SplitPath(pathname, DRV) + SplitPath(pathname, DIR));
}

void CVarroaPopDoc::InputFileUnknownVariable(const CString& name)
{
	MyMessageBox("Input File Variable Name " + name + " not Recognized");
}

void CVarroaPopDoc::InputFileException(const CString& message)
{
	MyMessageBox(CString("Error Pocessing Input File: ") + CString(message));
}

void CVarroaPopDoc::OutputFileException(const CString& message)
{
	MyMessageBox(CString("Error Storing Results: ") + CString(message));
}

CString CVarroaPopDoc::GetVersion()
{
	CString verstg;
	verstg.LoadString(IDS_VPVERSION);
	return verstg;
}

BOOL CVarroaPopDoc::CheckDateConsistencyFailed(const CString& warning)
{
	CString DispString =
		"Warning - The Following Events are Outside the Simulation Period:\n\n";
	DispString += warning;
	DispString += "\nPress OK to Continue the Simulation\nPress Cancel to Stop the Simulation";

	return (MyMessageBox(DispString, MB_OKCANCEL) == IDOK);
}
