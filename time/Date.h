/* Copyright (c) 1993 by Sanjay Ghemawat */

#ifndef _DATEH
#define _DATEH

#include "Year.h"
#include "Time_.h"
#include "WeekDay.h"
#include "Month.h"

#include <chrono>

using namespace std::chrono;

class Time;
class WeekDay;
class Month;

/*
 * Date class.
 */
class Date {
public:
	/*
	 * Constructors and assignments.
	 */
	Date() : rep{epoch} {} 
	Date(Date const& d) : rep{ d.rep } {}
	Date(year_month_day ymd) : rep{ ymd } {}
	Date(int d, month m, int yr) { rep = year_month_day{ year{yr}, m, day{unsigned(d)} }; }
	Date(Time);

	// DO NOT REMOVE, or the interface just freezes up and won't change date. for some reason.
	Date(long d) { rep = sys_days{ epoch } + days{ d }; }

	Date& operator=(Date const& d) { rep = d.rep; return *this; }
	Date& operator += (int d) { rep = sys_days{ rep } + days{unsigned( d )}; return *this; }
	Date& operator -= (int d) { rep = sys_days{ rep } - days{unsigned( d )}; return *this; }

	static Date Today() { return Date(Time::Now()); }
	static Date First() { return Date(1, January, Year::First()); }
	static Date Last();

	/*
	 * Addition and subtraction - number of days represents interval
	 * between two dates.
	 */
	inline friend Date  operator + (Date, int days);
	inline friend Date  operator - (Date, int days);
	inline friend int   operator - (Date, Date);


	/*
	 * Comparisons.
	 */
	inline friend int operator == (Date, Date);
	inline friend int operator != (Date, Date);
	inline friend int operator <  (Date, Date);
	inline friend int operator <= (Date, Date);
	inline friend int operator >  (Date, Date);
	inline friend int operator >= (Date, Date);

	/*
	 * Conversions.
	 */
	int         GetMDay()    const;       /* Day of the month. */
	WeekDay     GetWDay()    const;       /* Day of the week. */
	weekday		GetWDayNew() const;
	Month       GetMonth()   const;
	month		GetMonthNew() const;
	int         GetYear()    const;

	void BreakDown(int&, weekday&, month&, int&) const;
	void BreakDown(int&, WeekDay&, Month&, int&) const;

	int EpochDays() const { return (sys_days{ rep } - sys_days{ epoch }).count(); }
	static year_month_day epoch;

	// implicit conversion to year_month_day
	operator year_month_day() { return rep; }
private:
	/*
	 * Rep (was) the number of days since some epoch.
	 */
	year_month_day rep;
};

inline Date operator +  (Date date, int d) {
	auto res = year_month_day{ sys_days{ date.rep } + days{ unsigned(d) } };
	return Date(res);
}
inline Date operator -  (Date date, int d) {
	auto res = year_month_day{ sys_days{ date.rep } - days{ unsigned(d) } };
	return Date(res);
}
inline int  operator -  (Date a, Date b) { return (a.rep - b.rep); }
inline int  operator == (Date a, Date b) { return (a.rep == b.rep); }
inline int  operator != (Date a, Date b) { return (a.rep != b.rep); }
inline int  operator <  (Date a, Date b) { return (a.rep < b.rep); }
inline int  operator <= (Date a, Date b) { return (a.rep <= b.rep); }
inline int  operator >  (Date a, Date b) { return (a.rep > b.rep); }
inline int  operator >= (Date a, Date b) { return (a.rep >= b.rep); }

#endif /* _DATEH */
