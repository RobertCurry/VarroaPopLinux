#include "stdafx.h"
#include "WeatherGridData.h"
#include "WeatherEvents.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <array>
#include <cmath>
#include <fstream>
#include <numeric>
#include <regex>

/**
 * Define accessors for Binary data structures access
 */

template<>
struct DataItemAccessor<ObservedHistoricalItem>
{
    const ObservedHistoricalItem& m_dataItem;

    DataItemAccessor(const ObservedHistoricalItem& dataItem) : m_dataItem(dataItem) {}

    double PPT()
    {
        const static double pptMultiplier = 1.0 / 40.0;
        return m_dataItem.PPT * pptMultiplier;
    }
    double TMAX()
    {
        const static double tMultiplier = 1.0 / 100.0;
        return m_dataItem.TMAX * tMultiplier;
    }
    double TMIN()
    {
        const static double tMultiplier = 1.0 / 100.0;
        return m_dataItem.TMIN * tMultiplier;
    }
    double WIND()
    {
        const static double wMultiplier = 1.0 / 100.0;
        return m_dataItem.WIND * wMultiplier;
    }
    double SPH()
    {
        const static double sphMultiplier = 1.0 / 1000.0;
        return m_dataItem.SPH * sphMultiplier;
    }
    double SRAD()
    {
        const static double sradMultiplier = 1.0 / 40.0;
        return m_dataItem.SRAD * sradMultiplier;
    }
    double RMAX()
    {
        const static double rMultiplier = 1.0 / 100.0;
        return m_dataItem.RMAX * rMultiplier;
    }
    double RMIN()
    {
        const static double rMultiplier = 1.0 / 100.0;
        return m_dataItem.RMIN * rMultiplier;
    }
};

/**
 * WeatherGridData Implementation
 */

template<typename GridDataType>
WeatherGridData<GridDataType>::WeatherGridData()
{

}

template<typename GridDataType>
const typename WeatherGridData<GridDataType>::Data& WeatherGridData<GridDataType>::data() const
{
    return m_data;
}

template<typename GridDataType>
void WeatherGridData<GridDataType>::load(const std::string& filename)
{
    m_data.clear();

    std::ifstream istream(filename, std::ifstream::in | std::ifstream::binary);
    if (istream.good())
    {
        GridDataType data;
        while (!istream.eof())
        {
            istream.read(reinterpret_cast<char*>(&data), sizeof(data));
            if (istream.good())
            {
                m_data.push_back(data);
            }
        }
    }
}

template<>
COleDateTime WeatherGridData<ObservedHistoricalItem>::getStartTime() const
{
    return COleDateTime(1979, 1, 1, 0, 0, 0);
}

template<>
COleDateTime WeatherGridData<ModeledHistoricalItem>::getStartTime() const
{
    return COleDateTime(1950, 1, 1, 0, 0, 0);
}

template<>
COleDateTime WeatherGridData<Rcp85>::getStartTime() const
{
    return COleDateTime(2006, 1, 1, 0, 0, 0);
}

template<typename GridDataType>
COleDateTime WeatherGridData<GridDataType>::getEndTime() const
{
    COleDateTimeSpan span(m_data.size() - 1, 0, 0, 0); // remove start day 
    return getStartTime() + span;
}

namespace WeatherGridDataNs
{
    template<typename GridDataType>
    WeatherGridData<GridDataType> LoadGridData(const std::string& filename)
    {
        WeatherGridData<GridDataType> data;
        data.load(filename);
        return data;
    }

    DayLengthResult DayLength(float latitude, int jDay)
    {
        const double gamma = 2.0 * M_PI / 365.0 * (jDay - 1.0);
        const double delta = 180.0 / M_PI * (0.006918 - 0.399912 * std::cos(gamma) + 0.070257 * std::sin(gamma) - 0.006758 * std::cos(gamma) + 0.000907 * std::sin(gamma) - 0.002697 * std::cos(3.0 * (gamma)) + 0.00148 * std::sin(3.0 * (gamma)));
        const double cosWo = (sin(-0.8333 / 360.0 * 2.0 * M_PI) - sin(latitude / 360.0 * 2.0 * M_PI) *
            sin(delta / 360.0 * 2.0 * M_PI)) / (cos(latitude / 360.0 * 2.0 * M_PI) * cos(delta / 360.0 * 2.0 * M_PI));

        DayLengthResult result;
        if (cosWo >= -1.0 && cosWo <= 1.0)
        {
            result.sunrise = 12.0 - std::acos(cosWo) / (15.0 / 360.0 * 2.0 * M_PI);
            result.sunset = 12.0 + std::acos(cosWo) / (15.0 / 360.0 * 2.0 * M_PI);
            result.daylength = result.sunset - result.sunrise;
        }
        else if (cosWo > 1.0)
        {
            result.daylength = 0;
        }
        else if (cosWo < -1.0)
        {
            result.daylength = 24.0;
        }
        if (result.daylength == 24.0)
        {
            result.sunrise = 99.0;
            result.sunset = 99.0;
        }
        return result;
    }

    double DayLightHours(float latitude, int JDay)
    {
        int J = JDay;
        double P = std::asin(0.39795 * std::cos(0.2163108 + 2 * std::atan(0.9671396 * std::tan(0.00860 * (J - 186)))));
        double D = 24 - (24 / M_PI) * std::acos((std::sin(0.833 * M_PI / 180) + (std::sin(latitude * M_PI / 180) * std::sin(P))) / (std::cos(latitude * M_PI / 180) * std::cos(P)));
        return D;
    }

    int ComputeJDay(const COleDateTime& date)
    {
        const bool leapYear = (date.GetYear() % 4) == 0;
        std::vector<int> months = { 31, (!leapYear)?28:29, 31, 30, 31 , 30, 31 , 31, 30 , 31, 30, 31 };
        const int month = date.GetMonth();
        const int jDay = std::accumulate(months.begin(), months.begin() + month - 1, date.GetDay());
        return jDay;
    }

    double ComputeDaylightHours(const std::string& filename, const COleDateTime& date)
    {
        const double latitude = GetLatitudeFromFilename(filename);
        const double jDay = ComputeJDay(date);
        const double daylightHours = DayLength(latitude, jDay).daylength;
        return daylightHours;
    }

    class malformated_latitude_exception : public std::exception
    {
    public:
        malformated_latitude_exception(const std::string& filename) 
            : m_filename(filename) 
        {
        }
        virtual ~malformated_latitude_exception()
        {
        }
        virtual const char* what() const noexcept 
        { 
            std::string message = "can't find latitude in file named: " + m_filename; 
            return message.c_str(); 
        }
    private:
        std::string m_filename;
    };

    double GetLatitudeFromFilename(const std::string& filename)
    {
        const std::regex latitudeSearch("_([+-]?[0-9]*[.][0-9]+)_[+-]?[0-9]*[.][0-9]+");
        std::smatch latutudeMatch;
        if (std::regex_search(filename, latutudeMatch, latitudeSearch))
        {
            if (latutudeMatch.size() == 2)
            {
                return std::atof(latutudeMatch[1].str().c_str());
            }
            else
            {
                throw malformated_latitude_exception(filename);
            }
        }
        else
        {
            throw malformated_latitude_exception(filename);
        }
    }
    
    void HourlyTempraturesEstimator::compute()
    {
        std::double_t tsunset = tmin + ((tmax)-tmin) * std::sin(M_PI * (daylength / ((daylength)+4)));

        std::double_t prev_tsunset = 0.0;
        if (prev_tmin != (std::numeric_limits<double>::max)())
        {
            prev_tsunset = prev_tmin + ((prev_tmax)-prev_tmin) * std::sin((M_PI * (daylength) / ((daylength)+4)));
        }

        for (int h = 0; h < hourly_temperatures.size(); h++)
        {
            if (h <= sunrise)
            {
                if (prev_tmin != (std::numeric_limits<double>::max)())
                {
                    hourly_temperatures[h] = prev_tsunset - ((prev_tsunset - tmin) / std::log((std::max)(1.0, 24.0 - ((prev_sunset)-sunrise))) * std::log(h + 24.0 - prev_sunset + 1));
                }
                else
                {
                    hourly_temperatures[h] = tmin;
                }
            }
            else if (h > sunrise && h <= sunset)
            {
                hourly_temperatures[h] = tmin + (((tmax)-tmin) * std::sin(M_PI * ((h)-sunrise) / ((daylength)+4)));
            }
            else if (h > sunset)
            {
                if (next_tmin != (std::numeric_limits<double>::max)())
                {
                    hourly_temperatures[h] = tsunset - ((tsunset - next_tmin) / std::log(24.0 - ((sunset)-next_sunrise) + 1) * std::log(h - sunset + 1));
                }
                else
                {
                    hourly_temperatures[h] = tmin;
                }
            }
        }
    }

    int HourlyTempraturesEstimator::count_dayligth(double temperatureMinThreshold /*= 12.0*/, double temperatureMaxThreshold /*= 43.33*/)
    {
        int count = 0;
        for (size_t h = 0; h < hourly_temperatures.size(); h++)
        {
            if (h > sunrise && h <= sunset)
            {
                if (hourly_temperatures[h] > temperatureMinThreshold && hourly_temperatures[h] < temperatureMaxThreshold)
                    count++;
            }
        }
        return count;
    }
}


// Explicit instanciations of template classes

#define INSTANCIATE_WEATHER_GRID_DATA_TEMPLATES(TypeName)\
    template struct DataItemAccessor<TypeName>;\
    template class WeatherGridData<TypeName>;\
    template WeatherGridData<TypeName> WeatherGridDataNs::LoadGridData(const std::string& filename);

INSTANCIATE_WEATHER_GRID_DATA_TEMPLATES(ObservedHistoricalItem);
INSTANCIATE_WEATHER_GRID_DATA_TEMPLATES(ModeledHistoricalItem);
INSTANCIATE_WEATHER_GRID_DATA_TEMPLATES(Rcp85);
