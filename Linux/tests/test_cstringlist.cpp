#include "catch2/catch.hpp"

//! include explicitely since we are not using the pre-compiled header for tests
//! may change this in the future
#include "stdafx.h" 

#include "coblist.h"

#include <array>
#include <sstream>

TEST_CASE("CStringList operations", "[port]") {
    
    CStringList list;

    SECTION("Empty list checks") {
        
        CHECK(list.GetCount() == 0);
        CHECK(list.IsEmpty());
        CHECK(list.GetHeadPosition() == nullptr);
    }

    SECTION("Insert and Find - 1 element") {

        list.AddTail("tail");

        CHECK(list.GetCount() == 1);
        CHECK_FALSE(list.IsEmpty());

        POSITION head = list.GetHeadPosition();
        auto cObj = list.GetNext(head);
        CHECK(cObj == "tail");
    }

    SECTION("Insert and Find - 2 elements") {

        list.AddTail("head");
        list.AddTail("tail");

        CHECK(list.GetCount() == 2);
        CHECK_FALSE(list.IsEmpty());

        POSITION it = list.GetHeadPosition();
        {
            auto cObj = list.GetNext(it);
            CHECK(cObj == "head");
            CHECK(it != nullptr);
        }
        {
            auto cObj = list.GetNext(it);
            CHECK(cObj == "tail");
            CHECK(it == nullptr);
        }
        list.RemoveAll();
        CHECK(list.GetCount() == 0);
        CHECK(list.IsEmpty());
        CHECK(list.GetHeadPosition() == nullptr);
    }
}
