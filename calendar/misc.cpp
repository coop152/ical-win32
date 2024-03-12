/* Copyright (c) 1994 Sanjay Ghemawat */
#include "misc.h"
#include <fstream>
#include <cstring>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#include <stdarg.h>
#endif

#define MAXUNLENGTH 256 + 1

char* copy_string(char const* str) {
    char* out = new char[strlen(str)+1];
    strcpy(out, str);
    return out;
}

char const* my_name() {
    auto len = MAXUNLENGTH;
    char* name = new char[len];

    // Fetch my name
    #ifdef _WIN32
    if (GetUserNameA((LPTSTR)name, (LPDWORD)&len)) {
        return name;
    }
    else {
        return "???";

    }
    #else
    if (getlogin_r(name, len) == 0) {
        return name;
    }
    else {
        return "???";
    }
    #endif
}

bool copy_file(char const* src, char const* dst) {
    std::ifstream in(src, std::ios::binary);
    if (!in.is_open()) {
        return false;
    }

    std::ofstream out(dst, std::ios::binary);
    if (!out.is_open()) {
        return false;
    }

    out << in.rdbuf();

    return true;
}

void append_string(charArray* cbuf, char const* str) {
    cbuf->concat(str, strlen(str));
}

void format(charArray* cbuf, char const* format, ...) {
    static char buffer[4096];

    va_list ap;
    va_start(ap, format);
    vsnprintf(buffer, sizeof(buffer)-1,format, ap);
    va_end(ap);

    append_string(cbuf, buffer);
}
