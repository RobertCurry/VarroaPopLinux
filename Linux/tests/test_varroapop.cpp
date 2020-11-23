#include "catch2/catch.hpp"

//! include explicitely since we are not using the pre-compiled header for tests
//! may change this in the future
#include "stdafx.h"

#include "varroapop.h"

TEST_CASE("VarroaPop operations", "[port]")
{
    SECTION("SplitPath")
    {
#if defined(WINDOWS)
        CHECK(SplitPath("C:/dev/github/something/file.ext", DRV) == "C:");
#endif
        CHECK(SplitPath("C:/dev/github/something/file.ext", DIR) == "C:/dev/github/something/");
        CHECK(SplitPath("C:/dev/github/something/file.ext", FNAME) == "file");
        CHECK(SplitPath("C:/dev/github/something/file.ext", EXT) == ".ext");

        CHECK(SplitPath("/dev/github/something/file.ext", DRV) == "");
        CHECK(SplitPath("/dev/github/something/file.ext", DIR) == "/dev/github/something/");
        CHECK(SplitPath("/dev/github/something/file.ext", FNAME) == "file");
        CHECK(SplitPath("/dev/github/something/file.ext", EXT) == ".ext");
    }
}
