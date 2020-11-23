#include "catch2/catch.hpp"

//! include explicitely since we are not using the pre-compiled header for tests
//! may change this in the future
#include "stdafx.h" 

#include "carray.h"

#include "tests/helpers/myobject.h"

TEST_CASE("CArray operations", "[port]") {
    
    CArray<int32_t, int32_t> intArray;
    CArray<MyObject*, MyObject*> moArray;

    SECTION("IsEmpty") {

        CHECK(intArray.IsEmpty());
        CHECK(moArray.IsEmpty());
    }

    intArray.Add(12);
    intArray.Add(21);
    intArray.Add(54);

    auto obj1 = std::unique_ptr<MyObject>(new MyObject);
    auto obj2 = std::unique_ptr<MyObject>(new MyObject);
    auto obj3 = std::unique_ptr<MyObject>(new MyObject);
    auto obj4 = std::unique_ptr<MyObject>(new MyObject);
    auto obj5 = std::unique_ptr<MyObject>(new MyObject);

    moArray.Add(obj1.get());
    moArray.Add(obj2.get());
    moArray.Add(obj3.get());
    moArray.Add(obj4.get());
    moArray.Add(obj5.get());

    SECTION("Size") {

        CHECK_FALSE(intArray.IsEmpty());
        CHECK_FALSE(moArray.IsEmpty());

        CHECK(intArray.GetCount() == 3);
        CHECK(moArray.GetCount() == 5);
    }

    SECTION("RemoveAll") {

        intArray.RemoveAll();
        moArray.RemoveAll();

        CHECK(intArray.IsEmpty());
        CHECK(moArray.IsEmpty());

        CHECK(intArray.GetCount() == 0);
        CHECK(moArray.GetCount() == 0);
    }

    SECTION("Copy") {

        CArray<int32_t, int32_t> cpIntArray;
        cpIntArray.Copy(intArray);

        CArray<MyObject*, MyObject*> cpMoArray;
        cpMoArray.Copy(moArray);

        CHECK_FALSE(cpIntArray.IsEmpty());
        CHECK_FALSE(cpMoArray.IsEmpty());

        CHECK(cpIntArray.GetCount() == 3);
        CHECK(cpMoArray.GetCount() == 5);

        // However Copy does not make a deep-copy so pointer in the array are copied as-is

        obj3->UpdateData("obj3 updated");
        obj4->SetInitialized();

        CHECK(cpMoArray[2]->GetData() == "obj3 updated");
        CHECK_FALSE(cpMoArray[2]->IsInitialized());
        CHECK(cpMoArray[3]->GetData() == "nodata");
        CHECK(cpMoArray[3]->IsInitialized());

        CHECK(moArray[2]->GetData() == "obj3 updated");
        CHECK_FALSE(moArray[2]->IsInitialized());
        CHECK(moArray[3]->GetData() == "nodata");
        CHECK(moArray[3]->IsInitialized());
    }

    SECTION("[] operator non-const"){

        moArray[0]->UpdateData("altered through operator []");
        CHECK(moArray[0]->GetData() == "altered through operator []");
        CHECK_FALSE(moArray[0]->IsInitialized());

        moArray[4]->SetInitialized();
        CHECK(moArray[4]->GetData() == "nodata");
        CHECK(moArray[4]->IsInitialized());
    }

    SECTION("SetSize") {

        intArray.SetSize(2);
        moArray.SetSize(10);
        
        CHECK(intArray.GetCount() == 2);
        CHECK(moArray.GetCount() == 10);

        CHECK(intArray[0] == 12);
        CHECK(intArray[1] == 21);
        CHECK_THROWS_AS(intArray[3] = 10, std::out_of_range);

        // However in the case of the CArray of pointers objects were not created 
    }

    SECTION("RemoveAt") {

        intArray.SetSize(2);
        moArray.SetSize(10);
        
        CHECK(intArray.GetCount() == 2);
        CHECK(moArray.GetCount() == 10);

        CHECK_THROWS_AS(intArray[4] = 10, std::out_of_range);

        // However in the case of the CArray of pointers objects were not created 
    }
}
