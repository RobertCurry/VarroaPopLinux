#include "catch2/catch.hpp"

//! include explicitely since we are not using the pre-compiled header for tests
//! may change this in the future
#include "stdafx.h" 

#include "cmapstringtoob.h"

class CMyObject : public CObject {
public:
    CMyObject()
    : m_data("nodata")
    , m_initialized(false){
    }
    virtual ~CMyObject(){
        m_data = "badfood";
        m_initialized = false;
    }
    void SetInitialized(){m_initialized = true;}
    bool IsInitialized()const{return m_initialized;}
    void UpdateData(const std::string& data){m_data = data;}
    const std::string& GetData()const{return m_data;}
protected:
    std::string m_data;
    bool m_initialized;
};

TEST_CASE("CMapStringToOb operations", "[port]") {
    
    CMapStringToOb map;

    SECTION("IsEmpty") {
        
        CHECK(map.IsEmpty());
    }

    std::vector<std::string> keys = {
        "a",
        "b",
        "c"
    };

    std::vector<std::pair<bool, std::string>> values = {
        {false, "nodata"},
        {true, "nodata"},
        {false, "code"}
    };

    std::map<std::string, std::unique_ptr<CMyObject>> data;
    for (auto it=keys.begin(); it != keys.end(); it++)
    {
        data.insert(std::make_pair(it->c_str(), std::unique_ptr<CMyObject>(new CMyObject)));
    }

    for (auto it = data.begin(); it != data.end(); it++)
    {
        map.SetAt(it->first.c_str(), it->second.get());
    }

    SECTION("Iteration") {

        size_t mapIdx = 0;

        CString key;
        CMyObject* value = nullptr;
        POSITION position = map.GetStartPosition();
        for (POSITION position = map.GetStartPosition(); position != nullptr; ) 
        {
            map.GetNextAssoc(position, key, (CObject*&)value);
            CHECK(key == keys[mapIdx++]);
        }

        CHECK(mapIdx == 3);
    }

    SECTION("Lookup") {

        CMyObject* value = nullptr;
        REQUIRE(map.Lookup("b", (CObject*&)value));
        value->SetInitialized();

        value = nullptr;
        REQUIRE(map.Lookup("c", (CObject*&)value));
        value->UpdateData("code");

        value = nullptr;
        CHECK_FALSE(map.Lookup("d", (CObject*&)value));
        CHECK(value == nullptr);
            
        SECTION("Iteration") {

            size_t mapIdx = 0;

            CString key;
            CMyObject* value = nullptr;
            POSITION position = map.GetStartPosition();
            for (POSITION position = map.GetStartPosition(); position != nullptr; ) 
            {
                map.GetNextAssoc(position, key, (CObject*&)value);
                CHECK(key == keys[mapIdx]);
                CHECK(value->IsInitialized() == values[mapIdx].first);
                CHECK(value->GetData() == values[mapIdx].second);
                mapIdx++;
            }

            CHECK(mapIdx == 3);
        }

        SECTION("RemoveKey") {

            size_t mapIdx = 0;

            CString key;
            CMyObject* value = nullptr;
            POSITION position = map.GetStartPosition();
            for (POSITION position = map.GetStartPosition(); position != nullptr; ) 
            {
                map.GetNextAssoc(position, key, (CObject*&)value);
                CHECK(key == keys[mapIdx]);
                map.RemoveKey(key);
                mapIdx++;
            }
            
            CHECK(mapIdx == 3);

            // second iteration should have no element
            mapIdx = 0;

            position = map.GetStartPosition();
            for (POSITION position = map.GetStartPosition(); position != nullptr; ) 
            {
                map.GetNextAssoc(position, key, (CObject*&)value);
                CHECK(key == keys[mapIdx]);
                mapIdx++;
            }
            
            CHECK(mapIdx == 0);
        }

        SECTION("RemovalAll") {

            size_t mapIdx = 0;

            CString key;
            CMyObject* value = nullptr;
            POSITION position = map.GetStartPosition();
            for (POSITION position = map.GetStartPosition(); position != nullptr; ) 
            {
                map.GetNextAssoc(position, key, (CObject*&)value);
                CHECK(key == keys[mapIdx]);
                mapIdx++;
            }
            
            CHECK(mapIdx == 3);

            map.RemoveAll();

            // second iteration should have no element
            mapIdx = 0;

            position = map.GetStartPosition();
            for (POSITION position = map.GetStartPosition(); position != nullptr; ) 
            {
                map.GetNextAssoc(position, key, (CObject*&)value);
                CHECK(key == keys[mapIdx]);
                mapIdx++;
            }
            
            CHECK(mapIdx == 0);
        }
    }
}
