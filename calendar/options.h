/* Copyright (c) 1994 Sanjay Ghemawat */
#ifndef _OPTIONS_H
#define _OPTIONS_H

// Option map
//
// This class adds appropriate allocation/de-allocation of strings
// to a raw hash map.

#include <string.h>
#include "arrays.h"
#include <map>
#include <string>


class OptionMap {
  public:
    OptionMap();

    bool contains(char const* key) const;
    bool fetch(char const* key, char const*& value) const;
    void store(char const* key, char const* value);
    void remove(char const* key);

    void write(charArray*) const;

    operator std::string();
  private:
    std::map<std::string, std::string> rep;
};

inline OptionMap::OptionMap() : rep() {}

inline bool OptionMap::contains(char const* key) const {
    return (rep.contains(key));
}

// put located value in `value` if found, otherwise do not change.
// return true if map contains key.
inline bool OptionMap::fetch(char const* key, char const*& value) const {
    if (auto x = rep.find(key); x != rep.end()) {
        value = x->second.c_str();
        return true;
    }
    return false;
}



#endif /* _OPTIONS_H */
