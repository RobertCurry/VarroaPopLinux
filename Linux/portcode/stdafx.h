#pragma once
#ifndef STDAFX_CUSTOM_H
#define STDAFX_CUSTOM_H

/**
 * Void all MFC macros 
 */
#define DECLARE_DYNCREATE(Class) public:
#define IMPLEMENT_DYNCREATE(Class, BaseClass) // void

#define DECLARE_SERIAL(Class) // void
#define IMPLEMENT_SERIAL(Class, BaseClass, Offset) // void

#define DECLARE_MESSAGE_MAP() // void
#define BEGIN_MESSAGE_MAP(Class, BaseClass) // void
#define END_MESSAGE_MAP() // void

#define DDX_Text(_1, _2, _3) // void
#define DDV_MinMaxInt(_1, _2, _3, _4) // void
#define DDX_Radio(_1, _2, _3) // void

#define afx_msg 

#define XSTR(x) STR(x)
#define STR(x) #x

#include <cassert>
#define ASSERT assert

#include <memory>
#define DEBUG_NEW new

#include <cstdio> // stderr
#include <cstdlib> // stderr
#include <iostream> // std::cerr

/**
 * Make the TRACE method to work using the fmt dependency
 */
#include "fmt/printf.h"
#ifdef TRACE
#define TRACE(...) fmt::printf(__VA_ARGS__)
#else 
#define TRACE(...)
#endif // DEBUG

#ifndef WINDOWS
#if _WIN32 || _WIN64
#define WINDOWS
#endif
#endif

/**
 * Typedef MFC types to standard types
 */
#include <cstdint>
typedef int32_t BOOL;
typedef unsigned char BYTE;
typedef BYTE BOOLEAN;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef uint64_t ULONG_PTR;
typedef ULONG_PTR DWORD_PTR;
typedef uint32_t UINT;
typedef char* LPTSTR;
typedef const char* LPCTSTR;
typedef char TCHAR;
typedef intptr_t INT_PTR;
typedef uintptr_t UINT_PTR;
typedef long LONG;
typedef int64_t LONGLONG;
typedef uint64_t ULONGLONG;
typedef double DATE;

#define VAR_TIMEVALUEONLY ((DWORD)0x00000001) /* return time value */
#define VAR_DATEVALUEONLY ((DWORD)0x00000002) /* return date value */

/**
 * Redeclare the SYSTEMTIME structure for non-Windows systems
 */
#ifndef WINDOWS
#ifndef _SYSTEMTIME_
#define _SYSTEMTIME_
typedef struct _SYSTEMTIME
{
	WORD wYear;
	WORD wMonth;
	WORD wDayOfWeek;
	WORD wDay;
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
	WORD wMilliseconds;
} SYSTEMTIME;
typedef struct _SYSTEMTIME *PSYSTEMTIME;
typedef struct _SYSTEMTIME *LPSYSTEMTIME;
typedef struct {
    SYSTEMTIME st;
    WORD  wDayOfYear;
} UDATE;
#endif
#else
#define NOMINMAX
#include <windows.h>
#endif

#define TRUE true
#define FALSE false

// Replacement of the POSITION struct in the MFC framework
#include "position.h"

#include "carchive.h"
#include "carray.h"
#include "cobject.h"
#include "cdialog.h"
#include "ccmdtarget.h"
#include "cfile.h"
#include "cmapstringtoob.h"
#include "coblist.h"
#include "coledatetime.h"
#include "cptrlist.h"
#include "cstring.h"
#include "cstringarray.h"
#include "ctime.h"
#include "cuintarray.h"

/**
 * Make CString serializable in fmt formater
 */
#include "fmt/format.h"
template<>
struct fmt::formatter<CString>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx);

    template<typename FormatContext>
    auto format(CString const& number, FormatContext& ctx);
};
#include "fmt/ostream.h"
std::ostream &operator<<(std::ostream &stream, const CString& string);

#include "cstring.format.h"

/**
 * Define strcpy_s on non-Windows systems
 */
#if !defined(_WIN32) && !defined(_WIN64) && !defined(__STDC_LIB_EXT1__)
#define strcpy_s strcpy
#define strtok_s(_In, _Delim, _SafeToken) strtok(_In, _Delim)
#endif

#define NOT_IMPLEMENTED() std::cerr << __FUNCTION__  << ": NOT IMPLEMENTED in " << __FILE__ << "(" << __LINE__ << ")" << std::endl;

#endif // STDAFX_CUSTOM_H

