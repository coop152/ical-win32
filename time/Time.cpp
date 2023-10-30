/* Copyright (c) 1993 by Sanjay Ghemawat */

#include <Windows.h>
#include <stddef.h>
#include <sys/types.h>
#include <math.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "Time_.h"
#include "Date.h"
#include <chrono>

using namespace std::chrono;
/*
 * Want to get high resolution for region of time that will be most
 * heavily used.  This region will probably be around now. Therefore,
 * rep will be number of seconds elapsed since we construct the first
 * Time value for this run of the program.
 */

int    Time::junkInt = 0;

Time Time::Now() {
    sys_time t = time_point_cast<microseconds>(system_clock::now());
    return Time(t);
}

void Time::BreakDownDate(int& mday,weekday& wday,month& month, int& year) const
{
    this->BreakDown(mday, wday, month, year);
}

void Time::BreakDownClock(int& hour, int& min, int& sec, int& milli) const {
    weekday junkWDay;
    month   junkMonth;

    BreakDown(junkInt,junkWDay,junkMonth,junkInt, hour, min, sec, milli);
}

void Time::BreakDown(int& mday, weekday& wday, month& mon, int& year,
                     int& hour, int& min, int& sec, int& milli,
                     const char *tz) const
{
    Date d = Date(*this);
    // microseconds since start of day
    microseconds remainder = rep - floor<days>(rep);
    mday  = d.GetMDay();                         /* tm_mday in 1..31 */
    wday = d.GetWDayNew();     /* tm_wday in 0..6.  Sun = 0 */
    mon = d.GetMonthNew();       /* tm_mon  in 0..11. Jan = 0 */
    year  = d.GetYear();
    hour  = floor<hours>(remainder).count();
    min = duration_cast<minutes>( remainder % 1h ).count(); // mins into last hour
    sec = duration_cast<seconds>( remainder % 1min ).count(); // seconds into last minute
    milli = (remainder % 1s).count();
}
