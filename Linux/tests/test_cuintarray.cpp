#include "catch2/catch.hpp"

//! include explicitely since we are not using the pre-compiled header for tests
//! may change this in the future
#include "stdafx.h" 

#include "cuintarray.h"

TEST_CASE("CUIntArray operations", "[port]") {
    
    CUIntArray array;
    
    CHECK(array.GetSize() == 0);

    for (UINT i=15; i<100; i++)
        array.Add(i);
    
    CHECK(array.GetSize() == (100-15));

    CHECK(array.GetAt(0) == 15);
    CHECK(array.GetAt(10) == 25);

    array.RemoveAt(10);
    
    CHECK(array.GetAt(0) == 15);
    CHECK(array.GetAt(10) == 26);
    CHECK(array.GetAt(20) == 36);
    
    array.RemoveAll();
    
    CHECK(array.GetSize() == 0);
}
