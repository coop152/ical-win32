/* Copyright (c) 1993 by Sanjay Ghemawat */

#include <ctype.h>
#include "smallintset.h"
#include "arrays.h"
#include "lexer.h"
#include "misc.h"

unsigned int SmallIntSet::Size() const {
    unsigned int size = 0;

    for (int i = 0; i <= SISetLargestMember; i++)
        if (Member(i)) size++;
    return size;
}
bool SmallIntSet::Read(Lexer* lex) {
    Clear();
    while (1) {
        char c;

        lex->SkipWS();
        if (! lex->Peek(c)) return false;
        if (!isdigit(c)) return true;

        int num;
        if (! lex->GetNumber(num)) return false;
        if ((num < 0) || (num > SISetLargestMember)) {
            lex->SetError("illegal smallintset member");
            return false;
        }
        Insert(num);
    }
}

void SmallIntSet::Write(charArray* out) const {
    for (int i = 0; i <= SISetLargestMember; i++) {
        if (Member(i)) {
            format(out, " %d", i);
        }
    }
}
