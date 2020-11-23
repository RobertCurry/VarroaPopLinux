#include "stdafx.h"

#include "ColdStorageSimulator.h"
#include "Colony.h"
#include "Egg.h"
#include "Queen.h"
#include "WeatherEvents.h"


//////////////////////////////////////////////////////////////////////
// CColdStorageSimulator Member Functions
//////////////////////////////////////////////////////////////////////
double CColdStorageSimulator::GetTemp(CEvent& p_Event) const
{
	double temperature = p_Event.GetTemp();
	if (IsActive())
		temperature = m_Temperature;
	return temperature;
}
double CColdStorageSimulator::GetMaxTemp(CEvent& p_Event) const
{
	double temperature = p_Event.GetMaxTemp();
	if (IsActive())
		temperature = m_Temperature;
	return temperature;
}
double CColdStorageSimulator::GetMinTemp(CEvent& p_Event) const
{
	double temperature = p_Event.GetMinTemp();
	if (IsActive())
		temperature = m_Temperature;
	return temperature;
}
double CColdStorageSimulator::GetForageInc(CEvent& p_Event) const
{
	double forageInc = p_Event.GetForageInc();
	if (IsActive())
		forageInc = 0.0;
	return forageInc;
}
bool CColdStorageSimulator::IsForageDay(CEvent& p_Event) const
{
	bool forageDay = p_Event.IsForageDay();
	if (IsActive())
		forageDay = false;
	return forageDay;
}
void CColdStorageSimulator::SetStartDate(const COleDateTime& startDate)
{
	m_StartDate = startDate;
	m_StartDateStr = m_StartDate.Format("%m%d");
}
void CColdStorageSimulator::SetEndDate(const COleDateTime& endDate)
{
	m_EndDate = endDate;
	m_EndDateStr = m_EndDate.Format("%m%d");
}
void CColdStorageSimulator::Update(CEvent& p_Event, CColony& p_Colony)
{
	bool isActive = IsEnabled() && (IsOn() || IsColdStoragePeriod(p_Event));
	if (!m_IsActive && isActive)
	{
		m_IsStarting = true;
	}
	// Starting phase is when the bees are placed in cold storage 
	// and there are still brood that needs to become Adult
	if (m_IsStarting)
	{
		const bool starting = p_Colony.queen.GetTeggs() > 0 || p_Colony.CapDrn.GetQuantity() > 0 || p_Colony.CapWkr.GetQuantity() > 0;
		m_IsStarting = starting;
	}
	// Ending phase is when the queen starts to lay eggs again while in cold storage
	if (isActive && !m_IsStarting && p_Colony.queen.GetTeggs() > 0)
	{
		m_IsEnding = true;
	}
	if (!isActive)
	{
		m_IsStarting = false;
		m_IsEnding = false;
	}
	m_IsActive = isActive;
}
void CColdStorageSimulator::Reset()
{
	m_Enabled = false;
	m_StartDate = COleDateTime();
	m_EndDate = COleDateTime();
	m_On = false;
	m_Temperature = GetDefaultColdStorageTemperature();
	m_StartDateStr = "";
	m_EndDateStr = "";
	m_IsActive = false;
	m_IsStarting = false;
	m_IsEnding = false;
}
bool CColdStorageSimulator::IsActive() const
{
	return m_IsActive;
}
bool CColdStorageSimulator::IsStarting() const
{
	return IsActive() && m_IsStarting;
}
bool CColdStorageSimulator::IsEnding() const
{
	return IsActive() && m_IsEnding;
}
bool CColdStorageSimulator::IsColdStoragePeriod(CEvent& p_Event) const
{
	// check that SetStartDate and SetEndDate were called
	if (m_StartDateStr.empty() || m_EndDateStr.empty())
	{
		return false;
	}

	const std::string currentDateStr = (const char*)p_Event.GetTime().Format("%m%d");
	return (m_StartDateStr >= m_EndDateStr && (currentDateStr >= m_StartDateStr || currentDateStr <= m_EndDateStr))
		|| (m_StartDateStr <= m_EndDateStr && currentDateStr >= m_StartDateStr && currentDateStr <= m_EndDateStr);
}
