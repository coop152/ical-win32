/* Copyright (c) 1993 by Sanjay Ghemawat */

#ifndef _YEARH
#define _YEARH
class Year {
  public:
    static int IsLeap(int);
    static int Size(int);

    /*
     * Limit range of years.
     */
    static int First();
    static int Last();

    /*
     * Find offset of given year from beginning of first year.
     * Offset of First() year is zero. Offset of First() + 1
     * year is equal to the size of the first year.
     */
    static long Offset(int year);
};

#endif /* _YEARH */
