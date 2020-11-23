#pragma once
#ifndef CTIME_CUSTOM_H
#define CTIME_CUSTOM_H

#include "coledatetime.h"

#include <chrono>

/**
 * Only supports the necessary interface for the good behavior of VarroaPop
 */
class CTime
{
public:
	CTime();
	CTime(const SYSTEMTIME& time);

	COleDateTime GetTime() const;

	void FromTimeT(const time_t& time);
	time_t GetAsTimeT() const;

protected:

	// here we use a time point to get milliseconds precision
	std::chrono::system_clock::time_point m_time_point;
};

#endif // CTIME_CUSTOM_H
