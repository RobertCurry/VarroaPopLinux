#pragma once
#ifndef MYOBJECT_H
#define MYOBJECT_H

#include "cobject.h" 

#include <string>

class MyObject : public CObject {
public:
    MyObject()
    : m_data("nodata")
    , m_initialized(false){
    }
    virtual ~MyObject(){
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

#endif // MYOBJECT_H
