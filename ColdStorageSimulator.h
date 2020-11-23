#pragma once

#include <afx.h>

#include <string>

class CEvent;
class CColony;

// There are 2 ways to activate cold storage:
//    - based on an exterior criteria then call Activate / DeActivate to force cold storage
//    - specify start and end date so that cold storage is activated during this period
class CColdStorageSimulator : public CObject
{
public:
	static double GetDefaultColdStorageTemperature()
	{
		const double temperature = ((40.0 - 32.0) / 1.8);
		return temperature;
	}

private:
	// Enables cold storage 
	bool m_Enabled = false;
	// start and end date for automatic cold storage
	COleDateTime m_StartDate;
	COleDateTime m_EndDate;
	// Cold storage is activated based on m_On state only
	bool m_On = false;
	// Temperature in cold storage
	double m_Temperature = GetDefaultColdStorageTemperature();

	// Optimization
	std::string m_StartDateStr; // used for faster comparison between current and start date
	std::string m_EndDateStr; // used for faster comparison between current and end date

	// State attributes
	bool m_IsActive = false;
	bool m_IsStarting = false;
	bool m_IsEnding = false;

public:

	static CColdStorageSimulator& Get()
	{
		static CColdStorageSimulator sInstance;
		return sInstance;
	}

	void SetEnabled(bool enabled) { m_Enabled = enabled; }
	bool IsEnabled() const { return m_Enabled; }

	void Activate() { m_On = true; }
	void DeActivate() { m_On = false; }
	bool IsOn() const { return m_On; }

	// Returns true if the start and end date are not specified
	bool IsAutomatic() const { return m_StartDateStr.empty() && m_EndDateStr.empty(); }

	double GetTemp(CEvent& p_Event) const;
	double GetMaxTemp(CEvent& p_Event) const;
	double GetMinTemp(CEvent& p_Event) const;
	double GetForageInc(CEvent& p_Event) const;
	bool IsForageDay(CEvent& p_Event) const;

	void SetStartDate(const COleDateTime& startDate);
	void SetEndDate(const COleDateTime& endDate);

	// Updates the state of the cold storage, call once a simulation day
	void Update(CEvent& p_Event, CColony& queen);

	// Reset default values
	void Reset();

	// Returns true if cold storage is currently activated
	bool IsActive() const;

	// Returns true if cold storage is currently activated
	bool IsStarting() const;

	// Returns true if cold storage is currently activated
	bool IsEnding() const;

protected:

	bool IsColdStoragePeriod(CEvent& p_Event) const;
};
