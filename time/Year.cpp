/* Copyright (c) 1993 by Sanjay Ghemawat */

#include "Year.h"

#include <chrono>
using namespace std::chrono;

#define first_year 1900
#define last_year 2400

long Year::Offset(int y) {
    return round<days>(year(y) - year(first_year)).count();
}

int Year::IsLeap(int y) {
    return year(y).is_leap();
}

int Year::Size(int y) {
    return IsLeap(y) ? 366 : 365;
}

int Year::First() {
    return first_year;
}

int Year::Last() {
    return last_year;
}