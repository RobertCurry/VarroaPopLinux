#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <variant>
#include <vector>
#include <limits>

class CEvent;

struct ObservedHistoricalItem
{
    std::uint16_t PPT;
    std::int16_t TMAX;
    std::int16_t TMIN;
    std::int16_t WIND;
    std::int16_t SPH;
    std::int16_t SRAD;
    std::int16_t RMAX;
    std::int16_t RMIN;
};

struct ModeledHistoricalItem
{
    std::uint16_t PPT;
    std::int16_t TMAX;
    std::int16_t TMIN;
    std::int16_t WIND;
};

struct Rcp85
{
    std::uint16_t PPT;
    std::int16_t TMAX;
    std::int16_t TMIN;
    std::int16_t WIND;
};

// Rcp85 and Rcp45 format have the same binary layout
typedef Rcp85 Rcp45;

// Accessor to use to create proprietory data structure
template<typename GridDataType>
struct DataItemAccessor
{
    const GridDataType& m_dataItem;

    DataItemAccessor(const GridDataType& dataItem) : m_dataItem(dataItem) {}

    // Precipitations in mm
    double PPT()
    {
        const static double pptMultiplier = 1.0 / 40.0;
        return m_dataItem.PPT * pptMultiplier;
    }
    // Max temperature in Deg C
    double TMAX()
    {
        const static double tMultiplier = 1.0 / 100.0;
        return m_dataItem.TMAX * tMultiplier;
    }
    // Min temperature in Deg C
    double TMIN()
    {
        const static double tMultiplier = 1.0 / 100.0;
        return m_dataItem.TMIN * tMultiplier;
    }
    // Wind in m/s
    double WIND()
    {
        const static double wMultiplier = 1.0 / 100.0;
        return m_dataItem.WIND * wMultiplier;
    }
};

template<typename GridDataType>
class WeatherGridData
{
public:
    typedef std::vector<GridDataType> Data;

    WeatherGridData();

    void load(const std::string& filename);

    const Data& data() const;

    COleDateTime getStartTime() const;
    COleDateTime getEndTime() const;

protected:

	Data m_data;
};

namespace WeatherGridDataNs
{
	template<typename GridDataType>
	WeatherGridData<GridDataType> LoadGridData(const std::string & filename);

	struct DayLengthResult
	{
		float sunrise = -99.0;
		float sunset = -99.0;
		float daylength = 0.0;
	};

	// Computes the sunrise, sunset and daylength values for the given latitude and JDay of the year
	DayLengthResult DayLength(float latitude, int jDay);

    // Computes the day light hours
    double DayLightHours(float latitude, int JDay);

	// Computes the JDay given the date
	int ComputeJDay(const COleDateTime& date);

	// Computes the daylight hours (daylength) 
	// @param filename: needs to be under the form "prefix_latitude_suffix" where prefix and suffix can be anything but should not contain character '_'
	// @param date: used to compute the JDay
	double ComputeDaylightHours(const std::string& filename, const COleDateTime& date);

	// This method generates an std::exception is no latitude can be found in the filename
	// @param filename: needs to be under the form "prefix_latitude_suffix" where prefix and suffix can be anything but should not contain character '_'
	double GetLatitudeFromFilename(const std::string& filename);

	// Set all parameters to compute the estimated hourly temperatures
	// You can ommit:
	//   - prev_* properties if computing first day from the temperature file
	//   - next_* properties if computing last day from the temperature file
	struct HourlyTempraturesEstimator
	{
		double tmin = (std::numeric_limits<double>::max)();
		double tmax = (std::numeric_limits<double>::max)();
		double sunrise = (std::numeric_limits<double>::max)();
		double sunset = (std::numeric_limits<double>::max)();
		double daylength = (std::numeric_limits<double>::max)();
		double prev_tmin = (std::numeric_limits<double>::max)();
		double prev_tmax = (std::numeric_limits<double>::max)();
		double prev_sunset = (std::numeric_limits<double>::max)();
		double next_tmin = (std::numeric_limits<double>::max)();
		double next_sunrise = (std::numeric_limits<double>::max)();

		std::array<float, 24> hourly_temperatures;

		// compute hourly temperature estimations 
		void compute();

		// count daylight hours when temperatures are between temperatureMinThreshold and temperatureMaxThreshold
		int count_dayligth(double temperatureMinThreshold = 12.0, double temperatureMaxThreshold = 43.33);
	};
}
