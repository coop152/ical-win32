/* Copyright (c) 1996  by Sanjay Ghemawat */
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "arrays.h"
#include "lexer.h"
#include <io.h>

char const* Lexer::lastError = "";

Lexer::Lexer(char const* file) {
    tmp = new charArray;

    /* Dummy buffer for errors */
    buf = new char[1];
    buf[0] = '\0';
    index = 0;
    length = 0;

    //int fd = _open((char*)file, O_RDONLY, 0);
    //if (fd < 0) {
    //    SetError("could not open file");
    //    return;
    //}

    FILE* f = fopen(file, "rb"); // open in binary mode to stop stripping of \r characters
    if (f == nullptr) {
        SetError("could not open file");
        return;
    }

    //struct stat fs;
    //if (fstat(fd, &fs) < 0) {
    //    SetError("could not get file size");
    //    return;
    //}

    // seek to the end of the file to get size
    fseek(f, 0, SEEK_END);
    int filesize = ftell(f); 
    fseek(f, 0, SEEK_SET);
    
    /* Read the contents of the file */
    delete [] buf;
    buf = new char[filesize + 1];
    int result = fread(buf, sizeof(char), filesize + 1, f);
    if (result == 0 && errno == EINVAL) {
        // an empty file is fine, it only matters if errno is set
        SetError("Could not read file");
    }
    length = result;
  

    /* Null-terminate the array for fast scanning */
    buf[filesize] = '\0';

    /* Close the file */
    fclose(f);
}

Lexer::Lexer(charArray* text) {
    tmp = new charArray;
    index = 0;
    length = text->size();
    buf = new char[length+1];
    strncpy(buf, text->as_pointer(), length);
    buf[length] = '\0';
}

Lexer::~Lexer() {
    delete [] buf;
    delete tmp;
}

bool Lexer::Skip(char const* str) {
    int len = strlen(str);

    if ((index + len) <= length) {
        /* Still have enough chars left */
        if (strncmp(buf+index, str, len) == 0) {
            index += len;
            return true;
        }
        SetError("unexpected string");
        return false;
    }

    /* EOF */
    index = length;
    return false;
}

bool Lexer::SkipWS() {
    while (index < length) {
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
    if (index >= length) {
        return false;
    }

    /* Check first character */
    if (! is_letter(buf[index])) {
        SetError("illegal character when expecting id");
        return false;
    }

    int start = index;
    while ((index < length) && is_numletter(buf[index])) {
        index++;
    }

    tmp->clear();
    tmp->concat(buf+start, index - start);
    tmp->append('\0');
    x = tmp->as_pointer();
    return true;
}

bool Lexer::GetUntil(char terminator, char const*& x) {
    if (index >= length) {
        return false;
    }

    int start = index;
    while ((index < length) && (buf[index] != terminator)) {
        index++;
    }

    tmp->clear();
    tmp->concat(buf+start, index - start);
    tmp->append('\0');
    x = tmp->as_pointer();
    return true;
}
 
bool Lexer::GetNumber(int& x) {
    if (index >= length) {
        return false;
    }

    char* finish;
    long result = strtol(buf+index, &finish, 10);
    if (finish == (buf+index)) {
        SetError("error while reading number");
        return false;
    }

    x = result;
    index = (finish - buf);
    return true;
}

bool Lexer::GetString(char const*& x) {
    if (index >= length) return false;

    tmp->clear();

    while ((index < length) && (buf[index] != ']')) {
        if (buf[index] == '\\') {
            index++;
            if (index >= length) return false;
        }
        tmp->append(buf[index]);
        index++;
    }

    tmp->append('\0');
    x = tmp->as_pointer();
    return true;
}

void Lexer::PutString(charArray* out, char const* x) {
    while (*x != '\0') {
        char c = *x;
        if ((c == '\\') || (c == '[') || (c == ']')) out->append('\\');
        out->append(c);
        x++;
    }
}
