#pragma once
// VarroaPopSession.h : header file
//


#include "Colony.h"
#include "IEDItem.h"
#include "Matrix.h"

#include "stdafx.h"

class CVarroaPopSession;

class CVarroaPopSessionBridge
{
public:

	//! method called when Simulation start time is present in the input file
	virtual void SimulationStartUpdated() = 0;

	//! method called when Simulation end time is present in the input file
	virtual void SimulationEndUpdated() = 0;

	//! method called before the main loop of the simulation starts
	virtual void StartSimulation(CVarroaPopSession& session) = 0;

	//! method called after the main loop of the simulation
	virtual void EndSimulation(CVarroaPopSession& session) = 0;

	//! method called after the immigration flag was read in the session
	virtual void ImmigrationEnabled(bool enabled) = 0;

	//! method called if a weather filename was missing in the session
	virtual void WeatherFileMissing() = 0;

	//! method called if a weather filename was specified in the session
	virtual void WeatherFileLoaded(bool loaded, const CString& filename) = 0;

	//! method called when the session file was fully loaded
	virtual void SessionFileLoaded(CArchive& ar) = 0;

	virtual CString GetDefaultPathName(CArchive& ar) = 0;

	//! method called when a variable defined in the input file is not handled
	virtual void InputFileUnknownVariable(const CString& name) = 0;

	//! method called when an exception occured trying to load the input file
	virtual void InputFileException(const CString& name) = 0;

	//! method called when an exception occured trying to write the output file
	virtual void OutputFileException(const CString& name) = 0;

	//! method called to get the version number to display in the result file
	virtual CString GetVersion() = 0;

	//! method called is the ShowWarning option is on and dates are incompatible
	//! if the method returns True the simulation will proceed 
	virtual BOOL CheckDateConsistencyFailed(const CString& warning) = 0;
};

class CVarroaPopSession
{
public:
	CVarroaPopSession();
	virtual ~CVarroaPopSession();

	// Attributes
public:
	CStringList availableFilesList;
	CStringList selectedFilesList;
	int selectedFileIndex;

	//  Output/Plotting Attributes;
	CMatrix m_SimResults;		// The simulation results to be plotted
	CStringList m_SimLabels;	// Contains the labels of the plotted data series
	CStringList m_ResultsHeader;// Header Strings for ListView Control
	CStringList m_ResultsFileHeader; // Header Strings for File
	CStringList m_ResultsText;  // Results of Simulation 
	CStringList m_ResultsFileText; // Results of Simulation for file storage
	bool m_DispWeeklyData;		// Defines whether numeric results are generated
								//    Weekly or Daily

	// Graph Selection variables
	BOOL	m_AD;	// Adult Drones
	BOOL	m_AW;	// Adult Workers
	BOOL	m_CS;	// Colony Size
	BOOL	m_DB;	// Drone Brood
	BOOL	m_DE;	// Drone Eggs
	BOOL	m_DL;	// Drone Larvae
	BOOL	m_F;	// Foragers
	BOOL    m_TM;	// Total Mites in colony
	BOOL	m_MDB;	// Mites in Drone Brood
	BOOL	m_MWB;	// Mites in Worker Brood
	BOOL	m_PDB;	// Proportion of Drone Brood Infested
	BOOL	m_PWB;	// Proportion of Worker Brood Infested
	BOOL	m_PRM;	// Proportion of Running Mites resistant to miticide
	BOOL	m_RM;	// Free Running Mites
	BOOL	m_WB;	// Worker Brood
	BOOL	m_WE;	// Worker Eggs
	BOOL	m_WL;	// Worker Larvae
	BOOL    m_IM;	// Immigrating Mites
	BOOL	m_MD;	// Mites Dying this day or week
	BOOL	m_PMD;	// Proportion of total mite population dying this day or week
	BOOL	m_NS;	// Colony Nectar Stores
	BOOL	m_PS;	// COlony Pollen Stores
	BOOL	m_NPC;	// Nectar Persticide Concentration
	BOOL	m_PPC;	// Pollen Pesticide Concentration
	BOOL	m_DDL;	// Dead Drone Larvae from Pesticide
	BOOL	m_DWL;	// Dead Worker Larvae from Pesticide
	BOOL	m_DDA;	// Dead Drone Adults from Pesticide
	BOOL	m_DWA;	// Dead Worker Adults from Pesticide
	BOOL	m_DFG;	// Dead Foragers from Pesticide
	BOOL	m_TE;	// Total Eggs
	BOOL	m_DD;	// Daily DD
	BOOL	m_L;	// Daily L
	BOOL	m_N;	// Daily N
	BOOL	m_P;	// Daily P
	BOOL	m_dd;	// Daily dd
	BOOL	m_l;	// Daily l
	BOOL	m_n;	// Daily n
	int		m_AutoScaleChart; //0 if autoscale, 1 if manual scale
	double	m_YAxisMax; // If not autoscaled, the Y-axis maximum
	double  m_YAxisMin; // If not autoscaled, the Y-axis minimum




	// Options Selection from COptions Dialog
	BOOL m_ColTitles;
	BOOL m_InitConds;
	BOOL m_Version;
	BOOL m_WeatherColony;
	int m_FieldDelimiter;
	int m_DispFrequency;



protected:
	CColony theColony;
	CWeatherEvents* m_pWeather;
	bool m_FirstResultEntry;
	CString m_DefaultPathName;
	CString m_WeatherFileName;
	bool m_SessionLoaded;
	bool m_WeatherLoaded;
	bool m_ShowWarnings;


	// Simulation Data
	COleDateTime m_SimStartTime;
	COleDateTime m_SimEndTime;
	bool m_SimulationComplete;
	bool m_ResultsReady;

public:
	// Immigration Data
	CString m_ImmigrationType;
	CMite m_TotImmigratingMites;
	CMite m_IncImmigratingMites;
	CMite m_CumImmigratingMites;
	double m_ImmMitePctResistant;
	COleDateTime m_ImmigrationStartDate;
	COleDateTime m_ImmigrationEndDate;
	bool m_ImmigrationEnabled;

	// Re-Queening Data
	UINT	m_RQEggLayingDelay;
	double	m_RQWkrDrnRatio;
	BOOL	m_RQEnableReQueen;
	int		m_RQScheduled;
	double	m_RQQueenStrength;
	int		m_RQOnce;
	COleDateTime	m_RQReQueenDate;

	// Varroa Miticide Treatment Data;
	// NOTE:  Need to change this to a list to support multiple treatments
	CMiteTreatments m_MiteTreatments;

	COleDateTime	m_VTTreatmentStart;
	UINT	m_VTTreatmentDuration;
	UINT	m_VTMortality;
	double	m_InitMitePctResistant;
	BOOL	m_VTEnable;

	// Varroa Spore Treatment Data
	BOOL	m_SPEnable;
	COleDateTime	m_SPTreatmentStart;
	int		m_SPInitial;
	double	m_Mort10;
	double	m_Mort25;
	double	m_Mort50;
	double	m_Mort75;
	double	m_Mort90;

	// Comb Removal
	COleDateTime	m_CombRemoveDate;
	BOOL	m_CombRemoveEnable;
	double  m_CombRemovePct;

	// EPA Mortality
	CIEDItem m_IEDItem;  //Note:  This variable should be removed but it will change the session file format
	BOOL m_IEDEnable;

	CString m_ResultsFileFormatStg;

protected:

	CVarroaPopSessionBridge* m_Bridge;

	// Operations
public:

	void SetBridge(CVarroaPopSessionBridge* bridge) { m_Bridge = bridge; }
	CVarroaPopSessionBridge* GetBridge() const { return m_Bridge ; }

	CStringList* getAvailableFilesList();
	CStringList* getSelectedFilesList();
	int* getSelectedFileIndex();
	CWeatherEvents* GetWeather() { return m_pWeather; }
	CColony* GetColony() { return &theColony; }
	CColony* FindColony(CString ColonyName);
	void DeleteColony(CString ColonyName);
	void AddColony(CColony* pColony);
	CString GetWeatherFileName() { return m_pWeather->GetFileName(); }
	CString GetColonyFileName() { return theColony.GetName(); }
	int GetDocumentLength();
	CString GetDefaultPathName() { return m_DefaultPathName; }
	void SetDefaultPathName(CString stg) { m_DefaultPathName = stg; }
	bool DateInRange(COleDateTime StartRange, COleDateTime StopRange, COleDateTime theTime);
	bool CheckDateConsistency(bool ShowWarning);
	bool IsSessionLoaded() { return m_SessionLoaded; }
	bool IsWeatherLoaded() { return m_WeatherLoaded; }
	bool IsShowWarnings() { return m_ShowWarnings; }
	void SetShowWarnings(bool Warn) { m_ShowWarnings = Warn; }
	void SetWeatherFileName(CString fn) { m_WeatherFileName = fn; }
	bool LoadWeatherFile(CString WeatherFileName);


	// Simulation Operations
	COleDateTime GetSimStart() { return m_SimStartTime; }
	COleDateTime GetSimEnd() { return m_SimEndTime; }
	void SetSimStart(COleDateTime start);
	void SetSimEnd(COleDateTime end);
	int GetSimDays();
	int GetSimDayNumber(COleDateTime theDate);
	COleDateTime GetSimDate(int DayNum);
	int GetNumSeries();
	bool ReadyToSimulate();
	bool IsSimulationComplete() { return m_SimulationComplete; }
	bool AreResultsReady() { return m_ResultsReady; }
	int GetResultsLength() { return m_ResultsText.GetCount(); }
	void Simulate();
	void UpdateResults(int Day, CEvent* pEvent = NULL);

	// Immigration Operations
	void SetImmigrationType(CString ImType) { m_ImmigrationType = ImType; }
	CString GetImmigrationType() { return m_ImmigrationType; }
	void SetNumImmigrationMites(int mites)
	{
		m_TotImmigratingMites.SetResistant(int(mites * m_ImmMitePctResistant / 100));
		m_TotImmigratingMites.SetNonResistant(mites - m_TotImmigratingMites.GetResistant());
	}
	int GetNumImmigrationMites() { return m_TotImmigratingMites.GetTotal(); }
	void SetImmigrationStart(COleDateTime start) { m_ImmigrationStartDate = start; }
	COleDateTime GetImmigrationStart() { return m_ImmigrationStartDate; }
	void SetImmigrationEnd(COleDateTime end) { m_ImmigrationEndDate = end; }
	COleDateTime GetImmigrationEnd() { return m_ImmigrationEndDate; }
	void SetImmigrationEnabled(bool enabled) { m_ImmigrationEnabled = enabled; }
	bool IsImmigrationEnabled() { return m_ImmigrationEnabled; }
	bool IsImmigrationWindow(CEvent* pEvent);
	double GetImmPctResistant() { return m_ImmMitePctResistant; }
	void SetImmPctResistant(double pctres) { m_ImmMitePctResistant = pctres; }
	//	CMite GetImmigrationMites(COleDateTime theDate);
	CMite GetImmigrationMites(CEvent* pEvent);

	// Property Page Operations
	//void LoadMiteTreatments(CMiteTreatments* theTreatments);

	// Implementation
public:
	void ProcessInputFile(CString FileName);
	void ProcessInputFileA(CString FileName);
	WORD InputParameterToWord(CString ParameterName);
	void StoreResultsFile(CString PathName);
	void InitializeSimulation();

	void Serialize(CArchive& ar);
};

/////////////////////////////////////////////////////////////////////////////
