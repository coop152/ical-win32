/* Copyright (c) 1993 by Sanjay Ghemawat */

#ifndef _LEXERH
#define _LEXERH

#include <stdio.h>
#include <string.h>
#include "arrays.h"
#include <string>

/*
 * Help in lexing.
 */


class Lexer {
  public:
    Lexer(char const* file);
    Lexer(charArray*);

    /*
     * Status.
     */
    enum StatusType {
        Valid,
        Eof,
        Error
        };

    StatusType Status();                /* Get current status */

    void SetError(char const*);         /* Set error and corresponding msg */
    static char const* LastError();     /* Return text of last error */

    /*
     * Input operations.
     * Return true iff operation succeeded.
     * May have skipped undetermined amount of input in case of error.
     */

    /*
     * Character operations.
     */
    bool Peek(char&);            /* Return next char without advancing */
    bool Next(char&);            /* Return next char and advance over it */
    bool Advance(char&);         /* Advance and then Peek() */


    /*
     * Skip past expected text.
     */
    bool Skip(char);
    bool Skip(char const*);

    /*
     * Skip all whitespace.
     */
    bool SkipWS();

    /*
     * Read an identifier.
     * An identifier is a non-empty sequence of alphanumeric characters
     * (including underscores), where the first character is not a digit.
     * 
     * X is set to point to a temporary buffer managed by the Lexer.
     * The temporary buffer contains the null-terminated identifier.
     * The next GetId operation on the lexer will invalidate this
     * temporary buffer.
     */
    bool GetId(char const*& x);

    /*
     * Read text until the specified character is hit.
     * X is set as by GetId(char const*&).
     */
    bool GetUntil(char, char const*& x);

    /*
     * Read number.
     */
    bool GetNumber(int& num);

    /*
     * Read a string.  The string read by this method terminates
     * at the first "]".  However, backslashes can be used to
     * quote characters.  Therefore if the input contains "\]",
     * that will be read as "]".  If the input contains "\\",
     * that will be read as a single backslash.  The closing "]"
     * is not included in the returned string and is not consumed
     * from the input.
     *
     * X is set as by GetId(char const*&).
     */
    bool GetString(char const*& x);


     /*
     * Write "string" to out so that it can be later read back with
     * a call to GetString.  No terminating "]" is output.
     */
    static std::string EscapeString(std::string x);

    /*
     * Return current position in file.
     */
    int Index();

    /*
     * Reset to a position returned by an earlier call to "Index".
     * Also clears any error condition.
     */
    void Reset(int pos);
  protected:
    std::string buf;            /* Contents of entire file */
    int         index;          /* Index of next char in file */
    std::string tmp;            /* Temporary buffer */

    /* Last error message */
    static char const* lastError;
};

inline Lexer::StatusType Lexer::Status() {
    if (index == buf.length())        {return Eof;}
    else if (index > buf.length())    {return Error;}
    else                        {return Valid;}
}

inline void Lexer::SetError(char const* msg) {
    lastError = msg;
    index = buf.length()+1;
}

inline char const* Lexer::LastError() {
    return lastError;
}

inline int Lexer::Index() {
    return index;
}

inline void Lexer::Reset(int i) {
    index = i;
}    

inline bool Lexer::Peek(char& c) {
    if (index < buf.length()) {
        c = buf[index];
        return true;
    }
    else
        return false;
}

inline bool Lexer::Next(char& c) {
    if (index < buf.length()) {
        c = buf[index];
        index++;
        return true;
    }
    else
        return false;
}

inline bool Lexer::Advance(char& c) {
    index++;

    if (index < buf.length()) {
        c = buf[index];
        return true;
    }
    else {
        /* Undo advance */
        index = buf.length();
        return false;
    }
}

inline bool Lexer::Skip(char c) {
    if ((index < buf.length()) && (buf[index] == c)) {
        index++;
        return true;
    }
    else {
        SetError("unexpected character");
        return false;
    }
}
        
#endif /* _LEXERH */
