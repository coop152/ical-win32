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
    bool fetch(std::string key, std::string& value) const;
    void store(char const* key, char const* value);
    void remove(char const* key);

    operator std::string() const;
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

inline bool OptionMap::fetch(std::string key, std::string& value) const {
    if (auto x = rep.find(key); x != rep.end()) {
        value = x->second;
        return true;
    }
    return false;
}



#endif /* _OPTIONS_H */
