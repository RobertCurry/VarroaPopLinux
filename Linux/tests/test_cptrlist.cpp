#include "catch2/catch.hpp"

//! include explicitely since we are not using the pre-compiled header for tests
//! may change this in the future
#include "stdafx.h" 

#include "cptrlist.h"

#include "tests/helpers/myobject.h"

#include <sstream>

TEST_CASE("CPtrList operations", "[port]") {
    
    CTypedPtrList<CPtrList, MyObject*> list;

    SECTION("Empty list checks") {
        
        CHECK(list.GetCount() == 0);
        CHECK(list.IsEmpty());
        CHECK(list.GetTailPosition() == nullptr);
        CHECK(list.GetHeadPosition() == nullptr);
    }

    SECTION("Insert and Find - 1 element") {

        std::unique_ptr<MyObject> object (new MyObject()); 
        object->UpdateData("tail");
        POSITION tail = list.AddTail(object.get());

        CHECK(list.GetCount() == 1);
        CHECK_FALSE(list.IsEmpty());

        CObject* cObj = list.GetAt(tail);
        CHECK(cObj == object.get());

        cObj = list.GetNext(tail);
        CHECK(cObj == object.get());
        CHECK(tail == nullptr);
    }

    SECTION("Insert and Find - 2 elements") {

        std::unique_ptr<MyObject> headObject (new MyObject()); 
        headObject->UpdateData("head");
        list.AddTail(headObject.get());

        std::unique_ptr<MyObject> tailObject (new MyObject()); 
        tailObject->UpdateData("tail");
        list.AddTail(tailObject.get());

        CHECK(list.GetCount() == 2);
        CHECK_FALSE(list.IsEmpty());

        POSITION it = list.GetHeadPosition();
        CObject* cObj = list.GetAt(it);
        CHECK(cObj == headObject.get());

        cObj = list.GetNext(it);
        CHECK(cObj == headObject.get());
        CHECK_FALSE(it == nullptr);

        cObj = list.GetNext(it);
        CHECK(cObj == tailObject.get());
        CHECK(it == nullptr);

        it = list.GetTailPosition();
        cObj = list.GetAt(it);
        CHECK(cObj == tailObject.get());
    }

    SECTION("Insert and Find - Multiple elements") {

        std::vector<std::unique_ptr<MyObject>> myObjects;
        myObjects.resize(10);

        for (size_t i=0; i<10; i++) {

            std::stringstream elementName;
            elementName << "element " << (i+1);

            myObjects[i].reset(new MyObject()); 
            myObjects[i]->UpdateData(elementName.str());
            list.AddTail(myObjects[i].get());
        }

        CHECK(list.GetCount() == 10);
        CHECK_FALSE(list.IsEmpty());

        POSITION it = list.GetHeadPosition();
        REQUIRE(it != nullptr);
        auto cObj = dynamic_cast<MyObject*>(list.GetAt(it));
        CHECK(cObj->GetData() == "element 1");

        it = list.GetTailPosition();
        REQUIRE(it != nullptr);
        cObj = dynamic_cast<MyObject*>(list.GetAt(it));
        CHECK(cObj->GetData() == "element 10");

        list.RemoveHead();

        it = list.GetHeadPosition();
        REQUIRE(it != nullptr);
        cObj = dynamic_cast<MyObject*>(list.GetAt(it));
        CHECK(cObj->GetData() == "element 2");

        it = list.GetTailPosition();
        REQUIRE(it != nullptr);
        cObj = dynamic_cast<MyObject*>(list.GetAt(it));
        CHECK(cObj->GetData() == "element 10");

        list.RemoveAt(list.GetTailPosition());

        it = list.GetHeadPosition();
        REQUIRE(it != nullptr);
        cObj = dynamic_cast<MyObject*>(list.GetAt(it));
        CHECK(cObj->GetData() == "element 2");

        it = list.GetTailPosition();
        REQUIRE(it != nullptr);
        cObj = dynamic_cast<MyObject*>(list.GetAt(it));
        CHECK(cObj->GetData() == "element 9");

        it = list.GetHeadPosition();
        REQUIRE(it != nullptr);

        cObj = dynamic_cast<MyObject*>(list.GetNext(it));
        REQUIRE(it != nullptr);
        CHECK(cObj->GetData() == "element 2");
        
        cObj = dynamic_cast<MyObject*>(list.GetNext(it));
        REQUIRE(it != nullptr);
        CHECK(cObj->GetData() == "element 3");

        cObj = dynamic_cast<MyObject*>(list.GetNext(it));
        REQUIRE(it != nullptr);
        CHECK(cObj->GetData() == "element 4");
        
        cObj = dynamic_cast<MyObject*>(list.GetNext(it));
        REQUIRE(it != nullptr);
        CHECK(cObj->GetData() == "element 5");
        
        cObj = dynamic_cast<MyObject*>(list.GetNext(it));
        REQUIRE(it != nullptr);
        CHECK(cObj->GetData() == "element 6");
        
        cObj = dynamic_cast<MyObject*>(list.GetNext(it));
        REQUIRE(it != nullptr);
        CHECK(cObj->GetData() == "element 7");
        
        cObj = dynamic_cast<MyObject*>(list.GetNext(it));
        REQUIRE(it != nullptr);
        CHECK(cObj->GetData() == "element 8");

        cObj = dynamic_cast<MyObject*>(list.GetNext(it));
        REQUIRE(it == nullptr);
        CHECK(cObj->GetData() == "element 9");

        it = list.GetHeadPosition();
        list.GetNext(it);
        list.GetNext(it);
        // save valid Posision
        POSITION valid = it; list.GetNext(valid);
        list.RemoveAt(it);
        cObj = dynamic_cast<MyObject*>(list.GetAt(valid));
        CHECK(cObj->GetData() == "element 5");
    }
}
