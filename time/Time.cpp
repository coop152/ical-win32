/* Copyright (c) 1993 by Sanjay Ghemawat */

#include <Windows.h>
#include <stddef.h>
#include <sys/types.h>
#include <math.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

//#include "config.h"

#include "Month.h"
#include "WeekDay.h"
#include "Time_.h"
#include <chrono>

using namespace std::chrono;
/*
 * Want to get high resolution for region of time that will be most
 * heavily used.  This region will probably be around now. Therefore,
 * rep will be number of seconds elapsed since we construct the first
 * Time value for this run of the program.
 */

int    Time::initialized = 0;   /* Initialized yet? */
double Time::offset = 0.0;      /* Offset for time values */
int    Time::junkInt = 0;

void gettimeofday(struct win_timeval *tv) {
    auto now = system_clock::now().time_since_epoch();
    auto us = duration_cast<microseconds>(now).count();
    tv->tv_sec = us / 1000000;
    tv->tv_usec = us % 1000000;
}

void Time::Initialize() {
    struct win_timeval buf;
    gettimeofday(&buf);
    offset = buf.tv_sec;
    initialized = 1;
}

Time Time::Now() {
    struct win_timeval buf;
    gettimeofday(&buf);
    return Time(buf);
}

void Time::BreakDownDate(int& mday,WeekDay& wday,Month& month, int& year) const
{
    this->BreakDown(mday, wday, month, year);
}

void Time::BreakDownClock(int& hour, int& min, int& sec, int& milli) const {
    WeekDay junkWDay;
    Month   junkMonth;

    BreakDown(junkInt,junkWDay,junkMonth,junkInt, hour, min, sec, milli);
}

void Time::BreakDown(int& mday, WeekDay& wday, Month& month, int& year,
                     int& hour, int& min, int& sec, int& milli,
                     const char *tz) const
{
    char *old = getenv("TZ");
    if (! initialized) Initialize();

    time_t clock = (time_t) round(rep + offset);

    if (tz) {
        if ((old=getenv("TZ"))) old=_strdup(old);
        //setenv("TZ", tz, 1);
        SetEnvironmentVariable("TZ", tz);
        _tzset();
    }

    struct tm* t = localtime(&clock);

    if (tz) {
        //if (old) setenv("TZ", old, 1); else unsetenv("TZ");
        if (old) SetEnvironmentVariable("TZ", old);
        else SetEnvironmentVariable("TZ", nullptr);
        _tzset();
        free(old);
    }

    mday  = t->tm_mday;                         /* tm_mday in 1..31 */
    wday  = WeekDay::Sunday() + t->tm_wday;     /* tm_wday in 0..6.  Sun = 0 */
    month = Month::January() + t->tm_mon;       /* tm_mon  in 0..11. Jan = 0 */
    year  = t->tm_year + 1900;
    hour  = t->tm_hour;
    min   = t->tm_min;
    sec   = t->tm_sec;
    milli = (int)round((rep - floor(rep)) * 1000);
}

Time::Time(const struct win_timeval& tv) {
    if (! initialized) Initialize();
    rep = (tv.tv_sec - offset) + ((double) tv.tv_usec) / 1000000.0;
}


void Time::Convert(struct win_timeval& tv) const {
    if (! initialized) Initialize();

    tv.tv_sec  = (long) floor(rep + offset);
    tv.tv_usec = (long) round((rep + offset - tv.tv_sec) * 1000000.0);
}

Duration::Duration(const struct win_timeval& tv) {
    rep = tv.tv_sec + ((double) tv.tv_usec) / 1000000.0;
}

void Duration::Convert(struct win_timeval& tv) const {
    tv.tv_sec  = (long) floor(rep);
    tv.tv_usec = (long) round((rep - tv.tv_sec) / 1000000.0);
}
