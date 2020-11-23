#include "ctime.h"

#include <iomanip>
#include <sstream>

CTime::CTime()
: m_time_point(std::chrono::system_clock::now())
{
}

CTime::CTime(const SYSTEMTIME& time)
{
    const char* dateTimeFormat = "%Y-%m-%d %H:%M:%S";
    std::string dateTime = fmt::format("{:0>4}-{:0>2}-{:0>2} {:0>2}:{:0>2}:{:0>2}", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
    std::istringstream stream (dateTime);
    std::tm dt;
    dt.tm_isdst = -1; // needs to be set to unspecified otherwise random value is set
    stream >> std::get_time(&dt, dateTimeFormat);
    if (!stream.fail())
    {
        m_time_point = std::chrono::system_clock::from_time_t(std::mktime(&dt));
    }
}

COleDateTime CTime::GetTime() const
{
    COleDateTime time (m_time_point);
    return time;
}

void CTime::FromTimeT(const time_t& time)
{
    m_time_point = std::chrono::system_clock::from_time_t(time);   
}

time_t CTime::GetAsTimeT() const
{
    time_t time = std::chrono::system_clock::to_time_t(m_time_point);
    return time;
}
