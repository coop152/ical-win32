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

void gettimeofday(struct win_timeval *tv) {
    auto now = system_clock::now().time_since_epoch();
    auto us = duration_cast<microseconds>(now).count();
    tv->tv_sec = us / 1000000;
    tv->tv_usec = us % 1000000;
}


Time Time::Now() {
    struct win_timeval buf;
    gettimeofday(&buf);
    return Time(buf);
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

Time::Time(const struct win_timeval& tv) {
    //rep = (tv.tv_sec - offset) + ((double) tv.tv_usec) / 1000000.0;
    auto sec = seconds{ tv.tv_sec };
    auto us = microseconds{ tv.tv_usec };
    rep = sys_time<microseconds>{ sec + us };
}


void Time::Convert(struct win_timeval& tv) const {
    //tv.tv_sec  = (long) floor(rep + offset);
    //tv.tv_usec = (long) round((rep + offset - tv.tv_sec) * 1000000.0);
    auto sec = floor<seconds>(rep);
    auto us = rep - sec;
    tv.tv_sec = sec.time_since_epoch().count();
    tv.tv_usec = us.count();
}
