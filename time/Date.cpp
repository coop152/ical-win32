/* Copyright (c) 1993 by Sanjay Ghemawat */

#include "Date.h"

/*
 * Epoch is Thursday January 1, 1970.
 */
year_month_day Date::epoch = 1970y / January / 1;


Date::Date(Time t) {
	auto us = sys_time<microseconds>{ t };
	rep = year_month_day{ floor<days>(us) };
}

Date Date::Last() {
	year_month_day_last ymdl = year::max() / December / last;
	 
	return Date(year_month_day{ymdl});
}

void Date::BreakDown(int& d, weekday& wd, month& m, int& y) const {
	d = GetMDay();
	wd = GetWDayNew();
	m = GetMonthNew();
	y = GetYear();
}

void Date::BreakDown(int& d, WeekDay& wd, Month& m, int& y) const {
	d = GetMDay();
	wd = GetWDay();
	m = GetMonth();
	y = GetYear();
}

int Date::GetMDay() const {
	return unsigned(rep.day());
}

WeekDay Date::GetWDay() const {
	return WeekDay::First() + year_month_weekday{ rep }.weekday().c_encoding();
}

weekday Date::GetWDayNew() const {
	return year_month_weekday{ rep }.weekday();
}

Month Date::GetMonth() const {
	return Month::First() + unsigned(rep.month()) - 1;
}

month Date::GetMonthNew() const {
	return rep.month();
}

int Date::GetYear() const {
	return int(rep.year());
}

