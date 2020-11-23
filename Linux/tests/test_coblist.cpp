#include "catch2/catch.hpp"

//! include explicitely since we are not using the pre-compiled header for tests
//! may change this in the future
#include "stdafx.h" 

#include "coblist.h"

#include "tests/helpers/myobject.h"

#include <array>
#include <sstream>

TEST_CASE("CObList operations", "[port]") {
    
    CObList list;

    SECTION("Empty list checks") {
        
        CHECK(list.GetCount() == 0);
        CHECK(list.IsEmpty());
        CHECK(list.GetTailPosition() == nullptr);
        CHECK(list.GetHeadPosition() == nullptr);
    }

    SECTION("Insert and Find - 1 element") {

        std::unique_ptr<MyObject> object (new MyObject()); 
        object->UpdateData("head");
        POSITION head = list.AddHead(object.get());

        CHECK(list.GetCount() == 1);
        CHECK_FALSE(list.IsEmpty());

        CObject* cObj = list.GetAt(head);
        CHECK(cObj == object.get());

        cObj = list.GetPrev(head);
        CHECK(cObj == object.get());
        CHECK(head == nullptr);

        head = list.GetHeadPosition();
        cObj = list.GetAt(head);
        CHECK(cObj == object.get());

        cObj = list.GetNext(head);
        CHECK(cObj == object.get());
        CHECK(head == nullptr);
    }

    SECTION("Insert and Find - 2 elements") {

        std::unique_ptr<MyObject> headObject (new MyObject()); 
        headObject->UpdateData("head");
        list.AddHead(headObject.get());

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

        cObj = list.GetPrev(it);
        CHECK(cObj == tailObject.get());
        CHECK_FALSE(it == nullptr);

        cObj = list.GetPrev(it);
        CHECK(cObj == headObject.get());
        CHECK(it == nullptr);
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

        list.RemoveTail();

        it = list.GetHeadPosition();
        REQUIRE(it != nullptr);
        cObj = dynamic_cast<MyObject*>(list.GetAt(it));
        CHECK(cObj->GetData() == "element 2");

        it = list.GetTailPosition();
        REQUIRE(it != nullptr);
        cObj = dynamic_cast<MyObject*>(list.GetAt(it));
        CHECK(cObj->GetData() == "element 9");

        cObj = dynamic_cast<MyObject*>(list.GetPrev(it));
        REQUIRE(it != nullptr);
        CHECK(cObj->GetData() == "element 9");
        
        cObj = dynamic_cast<MyObject*>(list.GetPrev(it));
        REQUIRE(it != nullptr);
        CHECK(cObj->GetData() == "element 8");
        
        cObj = dynamic_cast<MyObject*>(list.GetPrev(it));
        REQUIRE(it != nullptr);
        CHECK(cObj->GetData() == "element 7");

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

        it = list.GetTailPosition();
        list.GetPrev(it);
        list.GetPrev(it);
        // save valid Posision
        POSITION valid = it; list.GetPrev(valid);
        list.RemoveAt(it);
        cObj = dynamic_cast<MyObject*>(list.GetAt(valid));
        CHECK(cObj->GetData() == "element 6");
    }
}
