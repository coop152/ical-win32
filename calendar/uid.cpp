/* Copyright (c) 1996  by Sanjay Ghemawat */
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include "misc.h"
#include "uid.h"

#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 1000
#endif

char const* uid_new() {
    static int   inited = 0;
    static char  hostname[MAXHOSTNAMELEN+1];
    static char  buffer[MAXHOSTNAMELEN+1000];
    static int   hostid = 0;
    static int   pid = 0;
    static int   counter = 0;

    if (! inited) {
        if (gethostname(hostname, MAXHOSTNAMELEN+1) < 0) {
            // Error?
            strcpy(hostname, "unknownhost");
        }
        hostid = 42;

        #ifdef _WIN32
        pid = GetCurrentProcessId();
        #else
        pid = getpid();
        #endif

        counter = 0;
        inited = 1;
    }

    sprintf(buffer, "%s_%x_%x_%x", hostname, hostid, pid, counter);
    counter++;

    return copy_string(buffer);
}

