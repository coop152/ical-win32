/* Copyright (c) 1994 Sanjay Ghemawat */
#include <assert.h>
#include "options.h"
#include "arrays.h"
#include "lexer.h"
#include "misc.h"
#include <vector>
#include <algorithm>
#include <format>

void OptionMap::store(char const* key, char const* value) {
    rep.insert_or_assign(key, value);
}

// XXX This is a memory leak of the key/value pair from the option map
void OptionMap::remove(char const* key) {
    rep.erase(key);
}

OptionMap::operator std::string() const {
    std::string out = "";
    // Generated sorted list
    std::vector<std::string> list;
    for (auto& [k, v] : rep) {
        list.push_back(k);
    }

    // This is slow, but the option list should be fairly small
    int i = 0;
    int num = list.size();
    while (i < num) {
        /* Find min element in list[i..] */
        int minIndex = i;
        for (int j = i + 1; j < num; j++) {
            //if (strcmp(list[j], list[minIndex]) < 0) {
            if (list[j].compare(list[minIndex]) < 0) {
                minIndex = j;
            }
        }

        std::string temp;
        temp = list[i];
        list[i] = list[minIndex];
        list[minIndex] = temp;

        i++;
    }
    // or...
    //std::sort(list.begin(), list.end());

    // Now print out in sorted order
    for (i = 0; i < num; i++) {
        std::string key = list[i];
        assert(rep.contains(key));

        std::string val;
        fetch(key, val);
        out += std::format("{} [{}]\n", key, Lexer::EscapeString(val));
    }
    return out;
}