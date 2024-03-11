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
    // Generate sorted list
    std::vector<std::string> list;
    for (auto& [k, v] : rep) {
        list.push_back(k);
    }
    std::sort(list.begin(), list.end());

    // Now print out in sorted order
    for (int i = 0; i < list.size(); i++) {
        std::string key = list[i];
        assert(rep.contains(key));

        std::string val;
        fetch(key, val);
        out += std::format("{} [{}]\n", key, Lexer::EscapeString(val));
    }
    return out;
}