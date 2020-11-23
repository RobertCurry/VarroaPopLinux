#include "catch2/catch.hpp"

//! include explicitely since we are not using the pre-compiled header for tests
//! may change this in the future
#include "stdafx.h" 

#include "ctime.h"

#include <ctime>

TEST_CASE("CTime operations", "[port]") {
    
    SECTION("construct") {

        COleDateTime dateTime(2020, 1, 11, 0, 0, 0);

        SYSTEMTIME systemTime;
        dateTime.GetAsSystemTime(systemTime);

        CTime time(systemTime);

        CHECK(time.GetTime() >= dateTime);
    }
    
    SECTION("now") {

        CTime time;
        auto now = std::time(nullptr);
        auto tm = std::localtime(&now);

        auto oleTime = time.GetTime();
        CHECK(oleTime.GetYear() == tm->tm_year + 1900);
        CHECK(oleTime.GetMonth() == tm->tm_mon + 1);
        CHECK(oleTime.GetDay() == tm->tm_mday);
        CHECK(oleTime.GetHour() == tm->tm_hour);
        CHECK(oleTime.GetMinute() == tm->tm_min);
    }
}
