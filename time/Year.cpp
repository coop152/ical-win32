/* Copyright (c) 1993 by Sanjay Ghemawat */

#include "Year.h"

#include <chrono>
using namespace std::chrono;


int Year::IsLeap(int y) {
    return year(y).is_leap();
}

int Year::First() {
    return int(year::min());
}

int Year::Last() {
    return int(year::max());
}