#include "stdafx.h"
#include "GlobalOptions.h"

GlobalOptions& GlobalOptions::Get()
{
	static GlobalOptions sGlobalOptions;
	return sGlobalOptions;
}

GlobalOptions::GlobalOptions()
	: ShouldForagerAgingBasedOnHourlyTemperatureEstimate(*this)
{
}

template<>
void GlobalOptions::AggregateOption<GlobalOptions::ForagerAgingBasedHourlyTemperatureEstimate>::Set(const GlobalOptions::ForagerAgingBasedHourlyTemperatureEstimate& value)
{
	m_value = value;
	m_options.ShouldForageDayElectionBasedOnTemperatures.Set(!m_value);
	m_options.ShouldComputeHourlyTemperatureEstimation.Set(m_value);
	m_options.ShouldForagersAlwaysAgeBasedOnForageInc.Set(m_value);
}

