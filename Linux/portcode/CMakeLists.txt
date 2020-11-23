cmake_minimum_required(VERSION 3.10)

find_package(fmt CONFIG REQUIRED)
find_package(Boost CONFIG REQUIRED system filesystem)

set(SOURCES carchive.cpp
            carray.cpp
            ccmdtarget.cpp
            cdialog.cpp
            cfile.cpp
            cmapstringtoob.cpp
            cobject.cpp
            coblist.cpp
            coledatetime.cpp
            cptrlist.cpp
            cstring.cpp
            cstringarray.cpp
            ctime.cpp
            cuintarray.cpp
            position.cpp
            stdafx.cpp
            varroapop.cpp )

add_library(VarroaPopLinuxPort ${SOURCES})
target_link_libraries(VarroaPopLinuxPort PRIVATE fmt::fmt)
target_link_libraries(VarroaPopLinuxPort PRIVATE Boost::system Boost::filesystem)
target_precompile_headers(VarroaPopLinuxPort PRIVATE "${VarroaPopPCH}")
