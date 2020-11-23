#include "catch2/catch.hpp"

//! include explicitely since we are not using the pre-compiled header for tests
//! may change this in the future
#include "stdafx.h" 

#include "globaloptions.h"

TEST_CASE("GlobalOptions operations", "[settings]") 
{    
    SECTION("Foragers aging behavior") 
    {
        GlobalOptions options;

        CHECK(options.ShouldForageDayElectionBasedOnTemperatures());
        CHECK_FALSE(options.ShouldComputeHourlyTemperatureEstimation());
        CHECK_FALSE(options.ShouldForagersAlwaysAgeBasedOnForageInc());

        CHECK_THROWS(options.ShouldForagerAgingBasedOnHourlyTemperatureEstimate());

        options.ShouldForagerAgingBasedOnHourlyTemperatureEstimate.Set(true);

        CHECK_FALSE(options.ShouldForageDayElectionBasedOnTemperatures());
        CHECK(options.ShouldComputeHourlyTemperatureEstimation());
        CHECK(options.ShouldForagersAlwaysAgeBasedOnForageInc());
    }
}
