#include "catch2/catch.hpp"

//! include explicitely since we are not using the pre-compiled header for tests
//! may change this in the future
#include "stdafx.h" 

#include "cstringarray.h"

TEST_CASE("CStringArray operations", "[port]") {
    
    CStringArray array;

    CHECK(array.GetSize() == 0);
    CHECK(array.GetCount() == 0);

    array.SetSize(10, 10);

    CHECK(array.GetSize() == 10);
    CHECK(array.GetCount() == 10);

    array.Add("foo");
    
    CHECK(array.GetSize() == 11);
    CHECK(array.GetCount() == 11);

    CHECK(array[10] == "foo");
    
    CHECK(array.GetUpperBound() == 10);
    
    array.Add("bar");
    array.Add("hello");
    array.Add("world");
    
    CHECK(array.GetUpperBound() == 13);

    CHECK(array.GetAt(10) == "foo");
    CHECK(array.GetAt(11) == "bar");
    CHECK(array.GetAt(12) == "hello");
    CHECK(array.GetAt(13) == "world");
    
    array.RemoveAll();

    array.Add("foo");
    
    CHECK(array.GetSize() == 1);
    CHECK(array.GetCount() == 1);

    CHECK(array[0] == "foo");
    
    CHECK(array.GetUpperBound() == 0);
    
    array.Add("bar");
    array.Add("hello");
    array.Add("world");
    
    CHECK(array.GetUpperBound() == 3);

    CHECK(array.GetAt(0) == "foo");
    CHECK(array.GetAt(1) == "bar");
    CHECK(array.GetAt(2) == "hello");
    CHECK(array.GetAt(3) == "world");
}