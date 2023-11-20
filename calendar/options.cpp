/* Copyright (c) 1994 Sanjay Ghemawat */
#include <assert.h>
#include "options.h"
#include "arrays.h"
#include "lexer.h"
#include "misc.h"

void OptionMap::store(char const* key, char const* value) {
    rep.insert_or_assign(key, value);
}

// XXX This is a memory leak of the key/value pair from the option map
void OptionMap::remove(char const* key) {
    rep.erase(key);
}

void OptionMap::write(charArray* out) const {
    // Generated sorted list
    Array<const char*> list;
    for (auto& [k, v] : rep) {
        list.append(k.c_str());
    }

    // This is slow, but the option list should be fairly small
    int i = 0;
    int num = list.size();
    while (i < num) {
        /* Find min element in list[i..] */
        int minIndex = i;
        for (int j = i+1; j < num; j++) {
            if (strcmp(list[j], list[minIndex]) < 0) {
                minIndex = j;
            }
        }

        const char* temp;
        temp = list[i];
        list[i] = list[minIndex];
        list[minIndex] = temp;

        i++;
    }

    // Now print out in sorted order
    for (i = 0; i < num; i++) {
        char const* key = (char const*) list[i];
        assert(rep.contains(key));

        char const* val;
        fetch(key, val);
        append_string(out, key);
        append_string(out, " [");
        Lexer::PutString(out, val);
        append_string(out, "]\n");
    }
}

