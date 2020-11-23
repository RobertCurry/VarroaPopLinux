#include "varroapopcmdbridge.h"

#include <iostream>

VarroaPopCmdBridge::VarroaPopCmdBridge(CVarroaPopSession& session)
: m_Session(session)
{
}

VarroaPopCmdBridge::~VarroaPopCmdBridge()
{
}
	
void VarroaPopCmdBridge::SimulationStartUpdated()
{
	// do nothing
}

void VarroaPopCmdBridge::SimulationEndUpdated()
{
	m_Session.StoreResultsFile(m_ResultsFileName);
}

void VarroaPopCmdBridge::StartSimulation(CVarroaPopSession& session)
{
	// TODO: start timer
}

void VarroaPopCmdBridge::EndSimulation(CVarroaPopSession& session)
{
	// TODO: end timer
	m_Session.StoreResultsFile(m_ResultsFileName);
}

void VarroaPopCmdBridge::ImmigrationEnabled(bool enabled)
{
	// do nothing
}

void VarroaPopCmdBridge::WeatherFileMissing()
{
	std::cerr << "Weather file missing" << std::endl;
}

void VarroaPopCmdBridge::WeatherFileLoaded(bool loaded, const CString& filename)
{
	if (loaded)
	{
		std::cout << "Weather file: " << filename.ToString() << " loaded" << std::endl;
	}
	else 
	{
		std::cerr << "Weather file: [ERROR] when loading " << filename.ToString() << std::endl;
	}
}

void VarroaPopCmdBridge::SessionFileLoaded(CArchive& ar)
{

}

CString VarroaPopCmdBridge::GetDefaultPathName(CArchive& ar)
{
	return "./";
}

void VarroaPopCmdBridge::InputFileUnknownVariable(const CString& name)
{
	std::cerr << name.ToString() << std::endl;
}

void VarroaPopCmdBridge::InputFileException(const CString& name)
{
	std::cerr << name.ToString() << std::endl;
}

void VarroaPopCmdBridge::OutputFileException(const CString& name)
{
	std::cerr << name.ToString() << std::endl;
}

CString VarroaPopCmdBridge::GetVersion()
{
	return "CMD";
}

BOOL VarroaPopCmdBridge::CheckDateConsistencyFailed(const CString& warning)
{
	std::cerr << warning.ToString() << std::endl;
	return true;
}
