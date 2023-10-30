/* Copyright (c) 1993 by Sanjay Ghemawat */

#ifndef TIMEH
#define TIMEH

#include <chrono>

using namespace std::chrono;

/*
 * A class to represent system time.
 */
class Time {
  public:
    /*
     * Constructors and assignments.
     */
    Time();                     /* Unspecified time */
    Time(Time const&);          /* Copy other time */
    Time(double seconds);       /* Seconds since unspecified epoch */
    Time(sys_time<microseconds> x);

    Time& operator=(Time const&);
    static Time Now();

    /* Comparisons */
    inline friend int operator == (Time const&, Time const&);
    inline friend int operator != (Time const&, Time const&);
    inline friend int operator <  (Time const&, Time const&);
    inline friend int operator <= (Time const&, Time const&);
    inline friend int operator >  (Time const&, Time const&);
    inline friend int operator >= (Time const&, Time const&);

    /*
     * Various conversions.
     */

    /* Variables for default references */
    static int junkInt;

    void BreakDownDate(int&     mday,
                       weekday& wday,
                       month&   month,
                       int&     year) const;

    void BreakDownClock(int& hour     = junkInt,
                        int& minute   = junkInt,
                        int& second   = junkInt,
                        int& millisec = junkInt) const;

    void BreakDown(int&     mday,
                   weekday& wday,
                   month&   month,
                   int&     year,
                   int&     hour     = junkInt,
                   int&     minute   = junkInt,
                   int&     second   = junkInt,
                   int&     millisec = junkInt,
                   const char *tz=0) const;

    double EpochSeconds() const;        /* Return seconds since epoch */
    operator sys_time<microseconds>() { return rep; }
  private:
    // microseconds since the system epoch (almost definitely unix epoch)
    sys_time<microseconds> rep;
};

/*
 * Time.
 */
inline Time::Time() { }

inline Time::Time(const Time& src) {
    rep = src.rep;
}

inline Time& Time::operator=(Time const& src) {
    rep = src.rep;
    return *this;
}

inline Time::Time(double r) {
    // convert given second amount to microseconds
    rep = sys_time<microseconds>{ microseconds{int(r * 1000000)} };
}

inline Time::Time(sys_time<microseconds> t) {
    rep = t;
}

inline int operator == (Time const& a, Time const& b) {
    return a.rep == b.rep;
}

inline int operator != (Time const& a, Time const& b) {
    return a.rep != b.rep;
}

inline int operator <  (Time const& a, Time const& b) {
    return a.rep < b.rep;
}

inline int operator <= (Time const& a, Time const& b) {
    return a.rep <= b.rep;
}

inline int operator >  (Time const& a, Time const& b) {
    return a.rep > b.rep;
}

inline int operator >= (Time const& a, Time const& b) {
    return a.rep >= b.rep;
}

inline double Time::EpochSeconds() const {
    return rep.time_since_epoch().count() / 1000000.0;
}

#endif /*TIMEH */
