/* Copyright (c) 1996  by Sanjay Ghemawat */
#include <fstream>
#include <sstream>
#include <cstring>

#include "lexer.h"

char const* Lexer::lastError = "";

Lexer::Lexer(char const* file) {
    index = 0;
    tmp = "";

    // open in binary mode to stop stripping of \r characters
    std::ifstream f(file, std::ios_base::binary);
    if (!f.is_open()) {
        SetError("could not open file");
        return;
    }
    
    /* Read the contents of the file */
    std::stringstream ss;
    ss << f.rdbuf();
    buf = ss.str();

    /* Close the file */
    f.close();

}

bool Lexer::Skip(char const* str) {
    int len = strlen(str);

    if ((index + len) <= buf.length()) {
        /* Still have enough chars left */
        if (buf.substr(index, len) == str) {
            index += len;
            return true;
        }
        SetError("unexpected string");
        return false;
    }

    /* EOF */
    index = buf.length();
    return false;
}

bool Lexer::SkipWS() {
    while (index < buf.length()) {
        char c = buf[index];
        if (!isspace(c)) return true;
        index++;
    }

    return false;
}

static inline bool is_letter(char c) {
    return ((c == '_') || isalpha(c));
}

static inline bool is_numletter(char c) {
    return ((c == '_') || isalnum(c));
}

bool Lexer::GetId(char const*& x) {
    if (index >= buf.length()) {
        return false;
    }

    /* Check first character */
    if (!is_letter(buf[index])) {
        SetError("illegal character when expecting id");
        return false;
    }

    int start = index;
    while ((index < buf.length()) && is_numletter(buf[index])) {
        index++;
    }

    tmp = buf.substr(start, index - start);
    x = tmp.c_str();
    return true;
}

bool Lexer::GetUntil(char terminator, char const*& x) {
    if (index >= buf.length()) {
        return false;
    }

    int start = index;
    while ((index < buf.length()) && (buf[index] != terminator)) {
        index++;
    }

    tmp = buf.substr(start, index - start);
    x = tmp.c_str();
    return true;
}
 
bool Lexer::GetNumber(int& x) {
    if (index >= buf.length()) {
        return false;
    }

    const char* buf_cstr = buf.c_str();
    char* finish;
    long result = strtol(buf_cstr+index, &finish, 10);
    if (finish == (buf_cstr+index)) {
        SetError("error while reading number");
        return false;
    }

    x = result;
    index = (finish - buf_cstr);

    return true;
}

bool Lexer::GetString(char const*& x) {
    if (index >= buf.length()) return false;

    tmp = "";

    while ((index < buf.length()) && (buf[index] != ']')) {
        if (buf[index] == '\\') {
            index++;
            if (index >= buf.length()) return false;
        }
        tmp += buf[index];
        index++;
    }

    x = tmp.c_str();
    return true;
}


std::string Lexer::EscapeString(std::string x) {
    std::string out = "";
    for (char c : x) {
        // escape any of these three characters, if present
        if ((c == '\\') || (c == '[') || (c == ']')) out += "\\";
        out += c;
    }
    return out;
}
