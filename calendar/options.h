/* Copyright (c) 1994 Sanjay Ghemawat */
#ifndef _OPTIONS_H
#define _OPTIONS_H

// Option map
//
// This class adds appropriate allocation/de-allocation of strings
// to a raw hash map.

#include <string.h>
#include "basic.h"
#include "hashfuncs.h"
#include "arrays.h"
#include <map>
#include <string>


class OptionMap {
  public:
    OptionMap();
    //OptionMap(const OptionMap&); // copy constructor
    //~OptionMap();

    bool contains(char const* key) const;
    bool fetch(char const* key, char const*& value) const;
    void store(char const* key, char const* value);
    void remove(char const* key);

    void write(charArray*) const;
  private:
    std::map<std::string, std::string> rep;
};

inline OptionMap::OptionMap() : rep() {}

//inline OptionMap::OptionMap(const OptionMap &other) : rep(other.rep) {}

inline bool OptionMap::contains(char const* key) const {
    return (rep.contains(key));
}

// put located value in `value` if found, otherwise do not change.
// return true if map contains key.
inline bool OptionMap::fetch(char const* key, char const*& value) const {
    //return (rep->fetch(key, value));
    if (auto x = rep.find(key); x != rep.end()) {
        value = x->second.c_str();
        return true;
    }
    return false;
}

//inline OptionMap_Bindings OptionMap::bindings() const {
//    OptionMap_Bindings b = rep;
//    return b;
//}

#endif /* _OPTIONS_H */
