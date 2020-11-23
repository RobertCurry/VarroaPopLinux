#include "catch2/catch.hpp"

//! include explicitely since we are not using the pre-compiled header for tests
//! may change this in the future
#include "stdafx.h" 

#include "carchive.h"

TEST_CASE("CArchive operations", "[port]") {
    
    struct ToSerialize
    {
        float aFloat;
        double aDouble;
        int32_t aInt32;
        uint32_t aUInt32;
        int8_t aInt8;
        COleDateTime aDateTime;
        CString aString;
        CTime aTime;
    };

    SYSTEMTIME systemTime;
    systemTime.wYear = 1982;
    systemTime.wMonth = 01;
    systemTime.wDay = 11;
    systemTime.wHour = 11;
    systemTime.wMinute = 30;
    systemTime.wSecond = 18;

    ToSerialize data {23.3f, 89.0, -2147483647, 4294967295, -127, COleDateTime(2020, 01, 11, 13, 30, 43), CString("This is a string"), CTime(systemTime)};

    SECTION("Store") {

        CStdioFile file("store.bin", CFile::modeWrite | CFile::typeBinary);
        CArchive archive(&file, CArchive::store);

        archive << data.aFloat;
        archive << data.aDouble;
        archive << data.aInt32;
        archive << data.aUInt32;
        archive << data.aInt8;
        archive << data.aDateTime;
        archive << data.aString;
        archive << data.aTime;

        file.Close();
    }

    SECTION("Load") {

        CStdioFile file("store.bin", CFile::modeRead | CFile::typeBinary);
        CArchive archive(&file, CArchive::load);

        ToSerialize loaded;
        archive >> loaded.aFloat;
        archive >> loaded.aDouble;
        archive >> loaded.aInt32;
        archive >> loaded.aUInt32;
        archive >> loaded.aInt8;
        archive >> loaded.aDateTime;
        archive >> loaded.aString;
        archive >> loaded.aTime;

        CHECK(loaded.aFloat == data.aFloat);
        CHECK(loaded.aDouble == data.aDouble);
        CHECK(loaded.aInt32 == data.aInt32);
        CHECK(loaded.aUInt32 == data.aUInt32);
        CHECK(loaded.aInt8 == data.aInt8);

        CHECK(loaded.aDateTime.GetYear() == 2020);
        CHECK(loaded.aDateTime.GetMonth() == 1);
        CHECK(loaded.aDateTime.GetDay() == 11);
        CHECK(loaded.aDateTime.GetHour() == 13);
        CHECK(loaded.aDateTime.GetMinute() == 30);

        CHECK(loaded.aString == "This is a string");
        
        CHECK(loaded.aTime.GetTime().GetYear() == 1982);
        CHECK(loaded.aTime.GetTime().GetMonth() == 1);
        CHECK(loaded.aTime.GetTime().GetDay() == 11);
        CHECK(loaded.aTime.GetTime().GetHour() == 11);
        CHECK(loaded.aTime.GetTime().GetMinute() == 30);

        file.Close();
    }
}
