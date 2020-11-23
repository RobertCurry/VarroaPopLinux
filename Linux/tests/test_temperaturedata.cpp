#include "catch2/catch.hpp"

//! include explicitely since we are not using the pre-compiled header for tests
//! may change this in the future
#include "stdafx.h" 

#include "helpers/common.h" 

#define _USE_MATH_DEFINES
#include <cmath>

#include <cstdint>
#include <optional>
#include <regex>
#include <variant>
#include <vector>

#include <boost/filesystem.hpp>
namespace bfs = boost::filesystem;

#include <boost/date_time.hpp>
namespace bd = boost::gregorian;

#include <boost/algorithm/string.hpp>
namespace ba = boost::algorithm;

#include "colony.h"
#include "coldstoragesimulator.h"
#include "queen.h"
#include "weatherevents.h"
#include "weathergriddata.h"

using namespace WeatherGridDataNs;

// The matcher class
class DayLengthResultMatcher : public Catch::MatcherBase<DayLengthResult> {
    DayLengthResult m_result;
    std::float_t m_epsilon = 1.e-5f;
public:
    DayLengthResultMatcher(const DayLengthResult& result) : m_result(result){}

    // Performs the test for this matcher
    bool match(const DayLengthResult& testing) const override {
        return Catch::Floating::WithinRelMatcher(m_result.sunrise, m_epsilon).match(testing.sunrise)
            && Catch::Floating::WithinRelMatcher(m_result.sunset, m_epsilon).match(testing.sunset)
            && Catch::Floating::WithinRelMatcher(m_result.daylength, m_epsilon).match(testing.daylength);
    }

    // Produces a string describing what this matcher does. It should
    // include any provided data (the begin/ end in this case) and
    // be written as if it were stating a fact (in the output it will be
    // preceded by the value under test).
    virtual std::string describe() const override {
        std::ostringstream ss;
        ss << "is matching sunrise:" << m_result.sunrise << " sunset:" << m_result.sunset << " daylength:" << m_result.daylength;
        return ss.str();
    }
};

// The builder function
inline DayLengthResultMatcher DayLengthResultEq(const DayLengthResult& result) {
    return DayLengthResultMatcher(result);
}

template<typename Container, typename Compare>
struct CompareMatcher : Catch::MatcherBase<Container> {

    CompareMatcher(const Container& comparator, const Compare& compare)
        : m_comparator(comparator),
        m_compare(compare) {}

    bool match(const Container& v) const override {
        if (m_comparator.size() != v.size()) {
            return false;
        }
        for (size_t i = 0; i < v.size(); ++i) {
            if (!m_compare(m_comparator[i], v[i])) {
                return false;
            }
        }
        return true;
    }

    virtual std::string describe() const override {
        return "Equals: " + Catch::Detail::stringify(m_comparator);
    }

    const Container& m_comparator;
    const Compare& m_compare;
};

template<typename Container, typename C>
CompareMatcher<Container, C> Compare(const Container& comparator, const C& compare) {
    return CompareMatcher<Container, C>(comparator, compare);
}

template<typename Type>
auto EqualsApprox(const std::array<Type, 24>& comparator) {
    return Compare(comparator, [=](Type actual, Type expected) {
        return actual == Approx(expected);
    });
}

TEST_CASE("Temperature Data", "[input]") {
    
    SECTION("Load") {

        bfs::path weatherFile (bfs::path(GetSimulationsDir()) / "ObservedHistoricalBinary" / "data_46.03125_-118.34375");

        CHECK(bfs::exists(weatherFile));

        auto data = LoadGridData<ObservedHistoricalItem>(weatherFile.string());
        bd::date startDate(1979, bd::Jan, 1);
        bd::date endDate = startDate + bd::days(data.data().size() - 1 /* first day*/);
        CHECK(endDate.year() == 2016);
        CHECK(endDate.month() == 12);
        CHECK(endDate.day() == 31);
    }

    SECTION("Load Rcp85Binary") {

        bfs::path weatherFile(bfs::path(GetSimulationsDir()) / "Rcp85Binary" / "data_46.03125_-118.34375");

        CHECK(bfs::exists(weatherFile));

        auto data = LoadGridData<Rcp85>(weatherFile.string());
        bd::date startDate(2006, bd::Jan, 1);
        bd::date endDate = startDate + bd::days(data.data().size() - 1 /* first day*/);
        CHECK(endDate.year() == 2099);
        CHECK(endDate.month() == 12);
        CHECK(endDate.day() == 31);
    }

    SECTION("JDay") {

        CHECK(ComputeJDay(COleDateTime(2019, 1, 1, 0, 0, 0)) == 1);
        CHECK(ComputeJDay(COleDateTime(2019, 2, 28, 0, 0, 0)) == 59);
        CHECK(ComputeJDay(COleDateTime(2019, 2, 29, 0, 0, 0)) == 60);
        CHECK(ComputeJDay(COleDateTime(2019, 3, 1, 0, 0, 0)) == 60);
        CHECK(ComputeJDay(COleDateTime(2019, 12, 31, 0, 0, 0)) == 365);
        CHECK(ComputeJDay(COleDateTime(2020, 1, 1, 0, 0, 0)) == 1);
        CHECK(ComputeJDay(COleDateTime(2020, 2, 28, 0, 0, 0)) == 59);
        CHECK(ComputeJDay(COleDateTime(2020, 2, 29, 0, 0, 0)) == 60);
        CHECK(ComputeJDay(COleDateTime(2020, 12, 31, 0, 0, 0)) == 366);
    }

    SECTION("GetLatitudeFromFileName"){

        const std::map<std::string, double> filenames = {
            {"Omak_bcc-csm1-1_rcp45_48.40625_-119.53125", 48.40625},
            {"Omak_inmcm4_rcp85_48.40625_-119.53125", 48.40625},
            {"Omak_inmcm4_rcp85_-48.40625_-119.53125", -48.40625},
            {"Richland_GFDL-ESM2G_rcp85_46.28125_-119.34375", 46.28125},
            {"Omak_inmcm4_rcp85_-48.40625_-119.53125.wth", -48.40625},
            {"Richland_GFDL-ESM2G_rcp85_46.28125_-119.34375.wth", 46.28125},
            {"data_46.28125_-119.34375", 46.28125},
            {"data_47.40625_-120.34375", 47.40625},
            {"data_-47.40625_-120.34375.txt", -47.40625},
            {"data_46.28125_-119.34375.txt", 46.28125},
            {"data_47.40625_-120.34375.wth", 47.40625},
            {"data_-47.40625_-120.34375.wth", -47.40625}
        };
        for (auto it = filenames.begin(); it != filenames.end(); it++)
        {
            std::stringstream info;
            info << it->first;
            info << ": ";
            info << it->second;
            INFO(info.str());
            CHECK(GetLatitudeFromFilename(it->first) == Catch::Detail::Approx(it->second));
        }
    }

    SECTION("daylength") {

        DayLengthResult northPole1 = { -99.0f, -99.0f, 0.0f };
        DayLengthResult northPole180 = { 99.0f, 99.0f, 24.0f };

        CHECK_THAT(DayLength(90.0f, 1), DayLengthResultEq(northPole1));
        CHECK_THAT(DayLength(90.0f, 180), DayLengthResultEq(northPole180));

        DayLengthResult pullman1 = { 7.694178f, 16.30582f, 8.611644f };
        DayLengthResult pullman180 = { 4.010171f, 19.98983f, 15.97966f };

        CHECK_THAT(DayLength(46.7298f, 1), DayLengthResultEq(pullman1));
        CHECK_THAT(DayLength(46.7298f, 180), DayLengthResultEq(pullman180));

        DayLengthResult rio1 = { 6.624304f, 17.3757f, 10.75139f };
        DayLengthResult rio180 = { 5.207381f, 18.79262f, 13.58524f };

        CHECK_THAT(DayLength(22.9068f, 1), DayLengthResultEq(rio1));
        CHECK_THAT(DayLength(22.9068f, 180), DayLengthResultEq(rio180));
    }

    SECTION("hourly temperature") {
        // JDay 1
        {
            auto current_daylength = DayLength(46.7298f, 1);
            auto next_daylength = DayLength(46.7298f, 2);

            HourlyTempraturesEstimator input;
            input.tmin = -2.55f;
            input.tmax = -0.28f;
            input.sunrise = current_daylength.sunrise;
            input.sunset = current_daylength.sunset;
            input.daylength = current_daylength.daylength;

            input.next_tmin = -6.71f;
            input.next_sunrise = next_daylength.sunrise;

            input.compute();

            std::array<std::float_t, 24> expectedResult = { -2.550000f,-2.550000f,-2.550000f,-2.550000f,-2.550000f,-2.550000f,-2.550000f,-2.550000f,-2.377236f,-1.824559f,-1.316665f,-0.8849064f,-0.5559374f,-0.3500657f,-0.28f,-0.3500657f,-0.5559374f,-1.787940f,-2.794280f,-3.479060f,-3.998755f,-4.417698f,-4.768682f, -5.070708f };
            CHECK_THAT(input.hourly_temperatures, EqualsApprox(expectedResult));
        }
    }
    SECTION("Cold Storage")
    {
        CEvent event;
        event.SetTemp(25.0);

        CColony colony;
        CQueen& queen = colony.queen;
        queen.SetMaxEggs(2000.0);

        CColdStorageSimulator simulator;

        CHECK(simulator.GetTemp(event) == Approx(25.0));

        simulator.SetEnabled(true);

        CHECK(simulator.GetTemp(event) == Approx(25.0));

        simulator.Activate();
        simulator.Update(event, colony);

        CHECK(simulator.IsActive());
        CHECK_FALSE(simulator.IsStarting());
        CHECK_FALSE(simulator.IsEnding());
        CHECK(simulator.GetTemp(event) == Approx(CColdStorageSimulator::GetDefaultColdStorageTemperature()));

        // Pick a date where the queen is laying eggs
        queen.LayEggs(90, 14.0, 12.0, 2000, 1);
        REQUIRE(queen.GetTeggs() > 0);

        simulator.DeActivate();
        simulator.Update(event, colony);

        CHECK_FALSE(simulator.IsActive());
        CHECK_FALSE(simulator.IsStarting());
        CHECK_FALSE(simulator.IsEnding());
        CHECK(simulator.GetTemp(event) == Approx(25.0));

        // cold storage period in a year        
        simulator.SetStartDate(COleDateTime(2020, 10, 1, 0, 0, 0));
        simulator.SetEndDate(COleDateTime(2020, 11, 30, 0, 0, 0));

        // event date is before cold storage period
        event.SetTime(COleDateTime(2020, 7, 1, 0, 0, 0));
        simulator.Update(event, colony);

        CHECK_FALSE(simulator.IsActive());
        CHECK_FALSE(simulator.IsStarting());
        CHECK_FALSE(simulator.IsEnding());
        CHECK(simulator.GetTemp(event) == Approx(25.0));

        // event date is on cold storage period start date
        event.SetTime(COleDateTime(2020, 10, 1, 0, 0, 0));
        simulator.Update(event, colony);

        CHECK(simulator.IsActive());
        CHECK(simulator.IsStarting());
        CHECK_FALSE(simulator.IsEnding());
        CHECK(simulator.GetTemp(event) == Approx(CColdStorageSimulator::GetDefaultColdStorageTemperature()));

        // Pick a date where the queen is not laying eggs (change daylight hours)
        queen.LayEggs(90, 14.0, 9.0, 2000, 1);
        REQUIRE(queen.GetTeggs() == 0);

        // event date is during cold storage period 
        event.SetTime(COleDateTime(2020, 11, 1, 0, 0, 0));
        simulator.Update(event, colony);

        CHECK(simulator.IsActive());
        CHECK_FALSE(simulator.IsStarting());
        CHECK_FALSE(simulator.IsEnding());
        CHECK(simulator.GetTemp(event) == Approx(CColdStorageSimulator::GetDefaultColdStorageTemperature()));

        // Pick a date where the queen is laying eggs (change daylight hours)
        queen.LayEggs(90, 14.0, 12.0, 2000, 1);
        REQUIRE(queen.GetTeggs() > 0);

        // event date is on cold storage period end date
        event.SetTime(COleDateTime(2020, 11, 30, 0, 0, 0));
        simulator.Update(event, colony);

        CHECK(simulator.IsActive());
        CHECK_FALSE(simulator.IsStarting());
        CHECK(simulator.IsEnding());
        CHECK(simulator.GetTemp(event) == Approx(CColdStorageSimulator::GetDefaultColdStorageTemperature()));

        // event date is after cold storage period 
        event.SetTime(COleDateTime(2020, 12, 1, 0, 0, 0));
        simulator.Update(event, colony);

        CHECK_FALSE(simulator.IsActive());
        CHECK_FALSE(simulator.IsStarting());
        CHECK_FALSE(simulator.IsEnding());
        CHECK(simulator.GetTemp(event) == Approx(25.0));

        // cold storage period on year transition
        simulator.SetStartDate(COleDateTime(2021, 11, 1, 0, 0, 0));
        simulator.SetEndDate(COleDateTime(2022, 2, 29, 0, 0, 0));

        // event date is before cold storage period
        event.SetTime(COleDateTime(2021, 7, 1, 0, 0, 0));
        simulator.Update(event, colony);

        CHECK(simulator.GetTemp(event) == Approx(25.0));

        // event date is on cold storage period start date
        event.SetTime(COleDateTime(2021, 11, 1, 0, 0, 0));
        simulator.Update(event, colony);

        CHECK(simulator.GetTemp(event) == Approx(CColdStorageSimulator::GetDefaultColdStorageTemperature()));

        // event date is during cold storage period 
        event.SetTime(COleDateTime(2021, 12, 1, 0, 0, 0));
        simulator.Update(event, colony);

        CHECK(simulator.GetTemp(event) == Approx(CColdStorageSimulator::GetDefaultColdStorageTemperature()));

        // event date is during cold storage period 
        event.SetTime(COleDateTime(2022, 1, 1, 0, 0, 0));
        simulator.Update(event, colony);

        CHECK(simulator.GetTemp(event) == Approx(CColdStorageSimulator::GetDefaultColdStorageTemperature()));

        // event date is on cold storage period end date
        event.SetTime(COleDateTime(2022, 3, 1, 0, 0, 0));
        simulator.Update(event, colony);

        CHECK(simulator.GetTemp(event) == Approx(CColdStorageSimulator::GetDefaultColdStorageTemperature()));

        // event date is after cold storage period 
        event.SetTime(COleDateTime(2022, 3, 2, 0, 0, 0));
        simulator.Update(event, colony);

        CHECK(simulator.GetTemp(event) == Approx(25.0));

        simulator.SetEnabled(false);

        // event date is on cold storage period end date
        event.SetTime(COleDateTime(2022, 3, 1, 0, 0, 0));
        simulator.Update(event, colony);

        CHECK(simulator.GetTemp(event) == Approx(25.0));

        simulator.Reset();
    }
}

TEST_CASE("Temperature Data Failing", "[hide]") 
{
    SECTION("Hourly Temperature")
    {
        // JDay 2
        {
            auto previous_daylength = DayLength(46.7298f, 1);
            auto current_daylength = DayLength(46.7298f, 2);
            auto next_daylength = DayLength(46.7298f, 3);

            HourlyTempraturesEstimator input;
            input.tmin = -6.71f;
            input.tmax = 1.46f;
            input.sunrise = current_daylength.sunrise;
            input.sunset = current_daylength.sunset;
            input.daylength = current_daylength.daylength;

            input.prev_tmin = -2.55f;
            input.prev_tmax = -0.28f;
            input.prev_sunset = previous_daylength.sunset;

            input.next_tmin = -6.71f;
            input.next_sunrise = next_daylength.sunrise;

            input.compute();

            std::array<std::float_t, 24> expectedResult = { -5.444190f,-5.685756f,-5.903585f,-6.101928f,-6.283987f,-6.452233f,-6.608616f,-6.754699f,-6.074356f,-4.088088f,-2.263309f,-0.7124124f,0.4690795f,1.2083956f,1.46f,1.2083956f,0.4690795f,-1.082260f,-2.173649f,-2.915478f,-3.478154f,-3.931593f,-4.311390f,-4.638157f };
            CHECK_THAT(input.hourly_temperatures, EqualsApprox(expectedResult));
        }
        // JDay 148
        {
            auto previous_daylength = DayLength(46.7298f, 147);
            auto current_daylength = DayLength(46.7298f, 148);
            auto next_daylength = DayLength(46.7298f, 149);

            HourlyTempraturesEstimator input;
            input.tmin = 8.83f;
            input.tmax = 18.02f;
            input.sunrise = current_daylength.sunrise;
            input.sunset = current_daylength.sunset;
            input.daylength = current_daylength.daylength;

            input.prev_tmin = 8.33f;
            input.prev_tmax = 17.42f;
            input.prev_sunset = previous_daylength.sunset;

            input.next_tmin = 5.62f;
            input.next_sunrise = next_daylength.sunrise;

            input.compute();

            std::array<std::float_t, 24> expectedResult = { 10.779638f,10.463412f,10.194304f,9.960077f,9.752718f,9.976793f,11.420404f,12.79732f,14.07209f,15.21189f,16.18737f,16.97343f,17.54982f,17.90169f,18.02f,17.90169f,17.54982f,16.97343f,16.18737f,15.21189f,13.56064f,11.23618f,9.791873f,8.741350f };
            CHECK_THAT(input.hourly_temperatures, EqualsApprox(expectedResult));
        }
        // JDay last
        {
            auto previous_daylength = DayLength(46.7298f, 150);
            auto current_daylength = DayLength(46.7298f, 151);

            HourlyTempraturesEstimator input;
            input.tmin = 7.64f;
            input.tmax = 13.75f;
            input.sunrise = current_daylength.sunrise;
            input.sunset = current_daylength.sunset;
            input.daylength = current_daylength.daylength;

            input.prev_tmin = 7.57f;
            input.prev_tmax = 17.29f;
            input.prev_sunset = previous_daylength.sunset;

            input.compute();

            std::array<std::float_t, 24> expectedResult = { 9.918212f,9.548716f,9.234689f,8.961630f,8.720073f,8.445311f,9.399063f,10.30797f,11.14885f,11.90026f,12.54306f,13.06085f,13.44042f,13.67211f,13.75f,13.67211f,13.44042f,13.06085f,12.54306f,11.90026f,7.64000f,7.64000f,7.640000f,7.640000f };
            CHECK_THAT(input.hourly_temperatures, EqualsApprox(expectedResult));
        }
    }
    SECTION("Hourly temperature from file") {

        struct CSVHourlyTemperatureDataItem
        {
            int Year;
            int Month;
            int Day;
            float Tmax;
            float Tmin;
            float SPH;
            float SRAD;
            float Rmax;
            float Rmin;
            float latitude;
            float longitude;
            float sunrise;
            float sunset;
            float daylength;
            float hour_0;
            float hour_1;
            float hour_2;
            float hour_3;
            float hour_4;
            float hour_5;
            float hour_6;
            float hour_7;
            float hour_8;
            float hour_9;
            float hour_10;
            float hour_11;
            float hour_12;
            float hour_13;
            float hour_14;
            float hour_15;
            float hour_16;
            float hour_17;
            float hour_18;
            float hour_19;
            float hour_20;
            float hour_21;
            float hour_22;
            float hour_23;

            void parse(const std::string& line)
            {
                std::vector<std::string> elements;
                ba::split(elements, line, [](const char& c) { return c == ','; });
                if (elements.size() == 38)
                {
                    size_t index = 0; 
                    Year = std::atoi(elements[index++].c_str());
                    Month = std::atoi(elements[index++].c_str());
                    Day = std::atoi(elements[index++].c_str());
                    Tmax = std::atof(elements[index++].c_str());
                    Tmin = std::atof(elements[index++].c_str());
                    SPH = std::atof(elements[index++].c_str());
                    SRAD = std::atof(elements[index++].c_str());
                    Rmax = std::atof(elements[index++].c_str());
                    Rmin = std::atof(elements[index++].c_str());
                    latitude = std::atof(elements[index++].c_str());
                    longitude = std::atof(elements[index++].c_str());
                    sunrise = std::atof(elements[index++].c_str());
                    sunset = std::atof(elements[index++].c_str());
                    daylength = std::atof(elements[index++].c_str());
                    hour_0 = std::atof(elements[index++].c_str());
                    hour_1 = std::atof(elements[index++].c_str());
                    hour_2 = std::atof(elements[index++].c_str());
                    hour_3 = std::atof(elements[index++].c_str());
                    hour_4 = std::atof(elements[index++].c_str());
                    hour_5 = std::atof(elements[index++].c_str());
                    hour_6 = std::atof(elements[index++].c_str());
                    hour_7 = std::atof(elements[index++].c_str());
                    hour_8 = std::atof(elements[index++].c_str());
                    hour_9 = std::atof(elements[index++].c_str());
                    hour_10 = std::atof(elements[index++].c_str());
                    hour_11 = std::atof(elements[index++].c_str());
                    hour_12 = std::atof(elements[index++].c_str());
                    hour_13 = std::atof(elements[index++].c_str());
                    hour_14 = std::atof(elements[index++].c_str());
                    hour_15 = std::atof(elements[index++].c_str());
                    hour_16 = std::atof(elements[index++].c_str());
                    hour_17 = std::atof(elements[index++].c_str());
                    hour_18 = std::atof(elements[index++].c_str());
                    hour_19 = std::atof(elements[index++].c_str());
                    hour_20 = std::atof(elements[index++].c_str());
                    hour_21 = std::atof(elements[index++].c_str());
                    hour_22 = std::atof(elements[index++].c_str());
                    hour_23 = std::atof(elements[index++].c_str());
                }
            }

            static CSVHourlyTemperatureDataItem Invalid()
            {
                CSVHourlyTemperatureDataItem invalid;
                invalid.clear();
                return invalid;
            }

            bool isValid() const 
            {
                static CSVHourlyTemperatureDataItem sInvalid = Invalid();
                return !(*this == sInvalid);
            }

            void clear() { memset(this, 0, sizeof(CSVHourlyTemperatureDataItem)); }

            bool operator==(const CSVHourlyTemperatureDataItem& other) const
            {
                return memcmp(this, &other, sizeof(CSVHourlyTemperatureDataItem)) == 0;
            }
        };

        bfs::path dataDirectory = GetTestsDir();
        dataDirectory /= "data";

        bfs::path omakFilePath = dataDirectory / "Omak_HourlyTemperaturesEstimation.csv";
        std::ifstream omakFile(omakFilePath.string(), std::ios_base::in);
        REQUIRE(omakFile.good());

        std::vector<CSVHourlyTemperatureDataItem> data;

        std::string line;

        std::getline(omakFile, line); // first line is column headers

        while (omakFile.good() && !omakFile.eof())
        {
             std::getline(omakFile, line);
             try
             {
                 CSVHourlyTemperatureDataItem item;
                 item.parse(line);
                 if (item.isValid())
                 {
                     data.push_back(item);
                 }
             }
             catch(...)
             {
                 std::cerr << "Line: " << line << " cannot be parsed to CSVData" << std::endl;
             }
        }

        REQUIRE(data.size() == 13881);

        CSVHourlyTemperatureDataItem expected;
        
        expected = { 1979,2,6,1.83,-6.25,0.0028,55.4,100,61.23,48.40625,-119.53125,7.17144570027357,16.8285542997264,9.65710859945285,-4.76008948755474,-5.02424441646498,-5.26110578322043,-5.47578928902288,-5.67209480159138,-5.85292220253554,-6.02053491933582,-6.1767335438027,-4.71929728940465,-2.95065729767772,-1.35583489818696,-0.0188493076474865,0.989863742593826,1.61716273634142,1.83,1.61716273634142,0.989863742593828,-0.253972510197426,-1.94308776995746,-2.97982543578956,-3.72995274949267,-4.31809284307809,-4.80193148440331,-5.21294918358566 };
        CHECK(data[36] == expected);

        expected = { 1979,7,6,33.07,15.2,0.0085,252.625,89.1,21.58,48.40625,-119.53125,3.93431561602807,20.0656843839719,16.1313687679439,19.7303742452638,19.0075824387806,18.3978010149415,17.8704229878499,15.3831705488828,18.1581893499511,20.861313337576,23.4268467188875,25.7924376556453,27.9005936418054,29.7000787815529,31.1471590088389,32.2066649850338,32.8528468424013,33.07,32.8528468424013,32.2066649850338,31.1471590088389,29.7000787815529,27.9005936418054,25.7924376556453,22.44660797383,20.4268105805484,19.0053998401906 };
        CHECK(data[186] == expected);

        expected = { 1982,12,21,1.84,-4.3,0.0036,40.4,100,88,48.40625,-119.53125,7.84458317615193,16.1554168238481,8.31083364769614,-3.48986802737647,-3.64124919147136,-3.77797088437726,-3.90262330827026,-4.01716554711344,-4.12311528772586,-4.22167220701657,-4.31380104697031,-4.05654710188955,-2.51573869605991,-1.09049498277154,0.126872530037326,1.05751625316166,1.64115938998495,1.84,1.64115938998495,1.05751625316166,-0.441263630377153,-1.41451416488852,-2.09137312798071,-2.61083471192319,-3.03246429026715,-3.38733609358046,-3.69372864101575 };
        CHECK(data[1450] == expected);

        expected = { 1999,7,5,27.37,7.29,0.007,325.4,98.69,34.37,48.40625,-119.53125,3.92417750718255,20.0758224928175,16.1516449856349,11.2404514331275,10.6101214416003,10.0784910479075,9.61879479539915,7.52735097636718,10.6420131492972,13.6753726760999,16.5538558131174,19.2076453235285,21.572373885413,23.590685312834,25.2136257227221,26.401830904988,27.1264810963558,27.37,27.1264810963558,26.401830904988,25.2136257227221,23.590685312834,21.572373885413,19.2076453235285,16.4457140770673,14.8022992089702,13.6472623123843 };
        CHECK(data[7490] == expected);

        expected = { 2011,9,20,26.26,5.72,0.0048,203.375,89.59,21.7,48.40625,-119.53125,5.83629680945432,18.1637031905457,12.3274063810914,9.85516016703379,9.17640378273531,8.57957434533616,8.04700749581804,7.56619991043564,7.12797599524873,6.36687212441886,10.2807921975313,14.0263807973456,17.4653942986169,20.4709042677994,22.93198218651,24.7577936439678,25.880950888377,26.26,25.880950888377,24.7577936439678,22.93198218651,20.4709042677994,17.4350924662465,15.5906984388493,14.3094462326179,13.3267196995101,12.5293649012352 };
        CHECK(data[11950] == expected);

        expected = { 2016,12,31,-4.98,-10.23,0.0021,57.7,100,88.06,48.40625,-119.53125,7.80756225656321,16.1924377434368,8.38487548687358,-8.27525538655417,-8.63957376828067,-8.96851300939355,-9.26834034497371,-9.5437923561646,-9.79853610690149,-10.035468721517,-10.2569189005201,-9.97382585417202,-8.66609472026456,-7.4584550222929,-6.42819685770049,-5.64125773588436,-5.14800251443357,-4.98,-5.14800251443357,-5.64125773588436,-10.23,-10.23,-10.23,-10.23,-10.23,-10.23,-10.23 };
        CHECK(data[13879] == expected);

        const size_t sTmaxIndex = 3;
        const size_t sTminIndex = 4;
        const size_t sLatitude = 9;
        const size_t sSunrise = 11;
        const size_t sSunset = 12;
        const size_t sDaylength = 13;
        const size_t sHourlyTemperatureStart = 14;

        std::string compareFileName = GetFileInTempDirectory("compare.csv");

        std::ofstream output(compareFileName, std::ios_base::out);

        output << "cpp, r" << std::endl;

        size_t startIndex = 0;
        size_t endIndex = data.size();

        COleDateTime date(1979, 1, 1, 0, 0, 0);
        date += COleDateTimeSpan(startIndex, 0, 0, 0);
        for (size_t dayIndex = startIndex; dayIndex < endIndex; dayIndex++)
        {
            auto& expectedItem = data[dayIndex];
            auto daylengthResult = DayLength(expectedItem.latitude, ComputeJDay(date));

            INFO(dayIndex);
            REQUIRE(daylengthResult.sunrise == Approx(expectedItem.sunrise));
            REQUIRE(daylengthResult.sunset == Approx(expectedItem.sunset));
            REQUIRE(daylengthResult.daylength == Approx(expectedItem.daylength));

            HourlyTempraturesEstimator estimator;
            estimator.tmin = expectedItem.Tmin;
            estimator.tmax = expectedItem.Tmax;
            estimator.sunrise = expectedItem.sunrise;
            estimator.sunset = expectedItem.sunset;
            estimator.daylength = expectedItem.daylength;
            if (dayIndex > 0)
            {
                estimator.prev_tmin = data[dayIndex - 1].Tmin;
                estimator.prev_tmax = data[dayIndex - 1].Tmax;
                estimator.prev_sunset = data[dayIndex - 1].sunset;
            }
            if (dayIndex < 13880)
            {
                estimator.next_tmin = data[dayIndex + 1].Tmin;
                estimator.next_sunrise = data[dayIndex + 1].sunrise;
            }
            estimator.compute();

            std::array<float, 24> expectedResult = {expectedItem.hour_0,expectedItem.hour_1,expectedItem.hour_2,expectedItem.hour_3,expectedItem.hour_4,expectedItem.hour_5,expectedItem.hour_6,expectedItem.hour_7,expectedItem.hour_8,expectedItem.hour_9,expectedItem.hour_10,expectedItem.hour_11,expectedItem.hour_12,expectedItem.hour_13,expectedItem.hour_14,expectedItem.hour_15,expectedItem.hour_16,expectedItem.hour_17,expectedItem.hour_18,expectedItem.hour_19,expectedItem.hour_20,expectedItem.hour_21,expectedItem.hour_22,expectedItem.hour_23 };            
            CHECK_THAT(estimator.hourly_temperatures, EqualsApprox(expectedResult));

            for (size_t hourIndex = 0; hourIndex < 24; hourIndex++)
            {
                output << estimator.hourly_temperatures[hourIndex] << "," << expectedResult[hourIndex] << std::endl;
            }

            date += COleDateTimeSpan(1, 0, 0, 0);
        }

        std::cout << "Output file written at " << compareFileName << std::endl;
        output.close();

        std::cout << date.Format("%Y/%m/%d") << std::endl;
    }
}