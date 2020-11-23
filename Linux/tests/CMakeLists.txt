cmake_minimum_required(VERSION 3.10)

find_package(Catch2 CONFIG REQUIRED)
find_package(Boost CONFIG REQUIRED system filesystem)

set(SOURCES main.cpp
            helpers/common.cpp
            helpers/myobject.cpp
            test_carchive.cpp
            test_carray.cpp
            test_cfile.cpp
            test_cmapstringtoob.cpp
            test_coblist.cpp
            test_coledatetime.cpp
            test_cptrlist.cpp
            test_cstring.cpp
            test_cstringarray.cpp 
            test_cstringlist.cpp 
            test_ctime.cpp 
            test_cuintarray.cpp
            test_globaloptions.cpp
            test_temperaturedata.cpp
            test_varroapop.cpp )


add_executable(VarroaPopTests ${SOURCES})
target_link_libraries(VarroaPopTests PRIVATE VarroaPopDataModel VarroaPopLinuxPort)
target_link_libraries(VarroaPopTests PRIVATE Catch2::Catch2)
target_link_libraries(VarroaPopTests PRIVATE Boost::system Boost::filesystem)
target_precompile_headers(VarroaPopTests REUSE_FROM VarroaPopLinuxPort)
