#pragma once
#ifndef VARROAPOPCMDBRIDGE_H
#define VARROAPOPCMDBRIDGE_H

#include "varroapopsession.h"

class VarroaPopCmdBridge : public CVarroaPopSessionBridge
{
public:

	VarroaPopCmdBridge(CVarroaPopSession& session);
	~VarroaPopCmdBridge();

	void SetResultsFileName(const CString& resultsFileName) { m_ResultsFileName = resultsFileName; }

	//! method called when Simulation start time is present in the input file
	virtual void SimulationStartUpdated();

	//! method called when Simulation end time is present in the input file
	virtual void SimulationEndUpdated();

	//! method called before the main loop of the simulation starts
	virtual void StartSimulation(CVarroaPopSession& session);

	//! method called after the main loop of the simulation
	virtual void EndSimulation(CVarroaPopSession& session);

	//! method called after the immigration flag was read in the session
	virtual void ImmigrationEnabled(bool enabled);

	//! method called if a weather filename was missing in the session
	virtual void WeatherFileMissing();

	//! method called if a weather filename was specified in the session
	virtual void WeatherFileLoaded(bool loaded, const CString& filename);

	//! method called when the session file was fully loaded
	virtual void SessionFileLoaded(CArchive& ar);

	virtual CString GetDefaultPathName(CArchive& ar);

	//! method called when a variable defined in the input file is not handled
	virtual void InputFileUnknownVariable(const CString& name);

	//! method called when an exception occured trying to load the input file
	virtual void InputFileException(const CString& name);

	//! method called when an exception occured trying to write the output file
	virtual void OutputFileException(const CString& name);

	//! method called to get the version number to display in the result file
	virtual CString GetVersion();

	//! method called is the ShowWarning option is on and dates are incompatible
	//! if the method returns True the simulation will proceed 
	virtual BOOL CheckDateConsistencyFailed(const CString& warning);

protected:

	CVarroaPopSession& m_Session;
	CString m_ResultsFileName;
};

#endif // VARROAPOPCMD_H