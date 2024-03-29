/* Copyright (c) 1993 by Sanjay Ghemawat */


#include "item.h"
#include "lexer.h"
#include "misc.h"
#include "uid.h"


const int Item::defaultRemindStart = 1;

Item::Item(): options() {
    text = copy_string("");
    owner = copy_string("");

    uid = (char*) uid_new();
    uid_persistent = 0;

    deleted = 0;
    remindStart = defaultRemindStart;

    date = new DateSet;

    hilite = copy_string("always");
    todo = 0;
    done = 0;
    important = 0;
}

Item::~Item() {
    delete [] text;
    delete [] owner;
    delete [] uid;
    delete [] hilite;
    delete date;
}

bool Item::Read(Lexer* lex) {
    while (1) {
        char c;
        char const* keyword;

        if (! lex->SkipWS() ||
            ! lex->Peek(c)) {
            lex->SetError("incomplete item");
            return false;
        }

        if (c == ']')
            return true;

        if (! lex->GetId(keyword) ||
            ! lex->SkipWS() ||
            ! lex->Skip('[')) {
            lex->SetError("error reading item property name");
            return false;
        }

        if (! Parse(lex, keyword) ||
            ! lex->SkipWS() ||
            ! lex->Skip(']')) {
            lex->SetError("error reading item property");
            return false;
        }
    }
}

bool Item::Parse(Lexer* lex, char const* keyword) {
    if (strcmp(keyword, "Remind") == 0) {
        if (! lex->SkipWS() ||
            ! lex->GetNumber(remindStart)) {
            lex->SetError("error reading remind level");
            return false;
        }
        return true;
    }

    if (strcmp(keyword, "Owner") == 0) {
        char const* x;
        if (!lex->GetString(x)) {
            lex->SetError("error reading owner information");
            return false;
        }
        delete [] owner;
        owner = copy_string(x);
        return true;
    }

    if (strcmp(keyword, "Uid") == 0) {
        char const* x;
        if (!lex->SkipWS() || !lex->GetUntil(']', x)) {
            lex->SetError("error reading unique id");
            return false;
        }
        delete [] uid;
        uid = copy_string(x);
        uid_persistent = 1;
        return true;
    }

    if (strcmp(keyword, "Contents") == 0) {
        char const* x;
        if (! lex->GetString(x)) {
            lex->SetError("error reading item text");
            return false;
        }
        SetText(x);
        return true;
    }

    if (strcmp(keyword, "Dates") == 0) {
        int start = lex->Index();
        if (! date->read(lex)) {
            // Could not understand date format.
            // Stash it away as an option.
            lex->Reset(start);
            char const* val;
            if (! lex->GetString(val)) {
                lex->SetError("error reading date information");
                return false;
            }
            options.store("Dates", val);
        }
        return true;
    }

    if (strcmp(keyword, "Hilite") == 0) {
        char const* x;
        if (!lex->GetString(x)) {
            lex->SetError("error reading item hilite");
            return false;
        }

        delete [] hilite;
        hilite = copy_string(x);
        return true;
    }

    if (strcmp(keyword, "Todo") == 0) {
        todo = 1;
        return true;
    }

    if (strcmp(keyword, "Done") == 0) {
        done = 1;
        return true;
    }

    if (strcmp(keyword, "Important") == 0) {
        important = 1;
        return true;
    }

    char* key = copy_string(keyword);
    char const* val;
    if (! lex->GetString(val)) {
        lex->SetError("error reading item property");
        delete [] key;
        return false;
    }

    options.store(key, val);
    delete [] key;
    return true;
}

Item::operator std::string() const {
    std::string out = "";

    out += std::format("Uid [{}]\n", uid);

    if (strlen(owner) != 0) {
        out += std::format("Owner [{}]\n", Lexer::EscapeString(owner));
    }

    out += std::format("Contents [{}]\n", Lexer::EscapeString(text));

    out += std::format("Remind [{}]\n", remindStart);

    out += std::format("Hilite [{}]\n", Lexer::EscapeString(hilite));

    if (todo) { out += "Todo []\n"; }
    if (done) { out += "Done []\n"; }
    if (important) { out += "Important []\n"; }

    out += std::format("Dates [{}]\n", std::string(*date));

    out += options;

    return out;
}

void Item::CopyTo(Item* item) const {
    // The code below cannot correctly handle aliasing.
    if (item == this) return;

    item->SetText(text);
    item->Hilite(hilite);
    item->SetOwner(owner);

    item->todo = todo;
    item->done = done;
    item->important = important;
    *item->date = *date;
    item->remindStart = remindStart;
    // Do NOT copy uid.  That would defeat the whole purpose of uids

    item->options = options;
}

char const* Item::GetOption(char const* key) const {
    char const* val;
    if (options.fetch(key, val)) return val;
    else return nullptr;
}

void Item::SetOption(char const* key, char const* val) {
    options.store(key, val);
}

void Item::RemoveOption(char const* key) {
    options.remove(key);
}

bool Item::similar(Item const* x) const {
    // Fast check
    if (this == x) return true;
    
    // XXX Compare the string versions (yes, the original code also did this)
    return std::string(*this) == std::string(*x);
}

Item* Notice::Clone() const {
    Notice* copy = new Notice;

    Item::CopyTo(copy);
    return copy;
}

bool Appointment::Parse(Lexer* lex, char const* keyword) {
    if (strcmp(keyword, "Start") == 0) {
        if (! lex->SkipWS() ||
            ! lex->GetNumber(start)) {
            lex->SetError("error reading appointment start time");
            return false;
        }
        return true;
    }

    if (strcmp(keyword, "Length") == 0) {
        if (! lex->SkipWS() ||
            ! lex->GetNumber(length)) {
            lex->SetError("error reading appointment length");
            return false;
        }
        return true;
    }

    if (strcmp(keyword, "Timezone") == 0) {
        char const* x;
        if (!lex->SkipWS() || !lex->GetString(x)) {
            lex->SetError("error reading appointment timezone");
            return false;
        }
        delete [] timezone;
        timezone = copy_string(x);
        return true;
    }

    if (strcmp(keyword, "Alarms") == 0) {
        if (alarms == nullptr) alarms = new intArray;
        alarms->clear();

        while (1) {
            char c;

            lex->SkipWS();
            if (! lex->Peek(c)) {
                lex->SetError("error reading alarm list");
                return false;
            }

            if (!isdigit(c)) break;

            int num;
            if (! lex->GetNumber(num)) return false;
            alarms->append(num);
        }

        return true;
    }

    return Item::Parse(lex, keyword);
}


Appointment::operator std::string() const {
    std::string out = "";

    out += std::format("Start [{}]\n", start);
    out += std::format("Length [{}]\n", length);
    if (has_timezone())
        out += std::format("Timezone [{}]\n", timezone);
    if (alarms != nullptr) {
        out += "Alarms [";
        for (int i = 0; i < alarms->size(); i++) {
            int x = alarms->slot(i);
            out += std::format(" {}", x); // with a space before it!
        }
        out += "]\n";
    }
    
    // and call the base class version to get the rest
    out += this->Item::operator std::string();
    return out;
}

Item* Appointment::Clone() const {
    Appointment* copy = new Appointment;

    Item::CopyTo(copy);
    copy->SetStart(-1, start);
    copy->SetLength(length);
    copy->SetTimezone(timezone, false);

    if (copy->alarms != nullptr) {
        delete copy->alarms;
        copy->alarms = nullptr;
    }
    if (alarms != nullptr) {
        copy->alarms = new intArray(*alarms);
    }

    return copy;
}

void Appointment::convert_tz(Date &d, int &min, bool to_tz) const {
    if ((min==cache.from_min) && (d==cache.from_d) && (to_tz==cache.to_tz)) {
        d=cache.to_d;
        min=cache.to_min;
        return;
    }

    cache.from_min=min;
    cache.from_d=d;
    cache.to_tz=to_tz;

    // time that appointment begins, as a time_point<minutes>, according to system clock
    auto start_time = sys_days{ year_month_day{d} } + minutes{ min };
    zoned_time zoned{ timezone, start_time };
    local_time converted = zoned.get_local_time();
    // get date by truncating to days and converting to year_month_day
    d = Date(year_month_day{ floor<days>(converted) });
    // get time by taking the remainder (in seconds)
    seconds remainder = converted - floor<days>(converted);
    min = floor<minutes>(remainder).count();

    cache.to_d = d;
    cache.to_min = min;
}

bool Appointment::contains(Date d) const {
    if (!has_timezone()) return Item::contains(d);
    Date dd;
    int tt;
    dd=d; tt=0; datetime_to_tz(dd,tt);
    if (Item::contains(dd) && start > tt) return true;
    dd=d; tt=24*60-1; datetime_to_tz(dd,tt);
    if (Item::contains(dd) && start < tt) return true;
    return false;
}

// DateSet wrappers
bool Item::contains(Date d) const {
    if (!todo || done) return date->contains(d);

    // Special handling for todo items
    Date today = Date::Today();
    if (d < today) return false;
    if (d > today) return date->contains(d);

    // d == today
    Date f;
    return (date->first(f) && (f <= today));
}
bool Item::first(Date& result) const {
    if (!date->first(result)) return false;
    if (!todo || done) return true;

    // Special handling for todo items
    Date today = Date::Today();
    if (result < today) result = today;
    return true;
}

bool Item::range(Date& s, Date& f) const {
    if (!date->get_range(s, f)) return false;
    return true;
}

bool Item::next(Date d, Date& result) const {
    if (!todo || done) return date->next(d, result);

    // Special handling for todo items
    Date today = Date::Today();
    if (d >= today) return date->next(d, result);

    // Starting search before "today" -- just find first item
    if (!date->first(result)) return false;
    if (result < today) result = today;
    return true;
}

void Item::set_start(Date start) {
    date->set_start(start);
}

void Item::set_finish(Date finish) {
    date->set_finish(finish);
}

void Item::delete_occurrence(Date d) {
    if (!todo || done) {
        date->delete_occurrence(d);
        return;
    }

    // Special handling for todo items
    Date today = Date::Today();
    if (d < today) return;
    if (d > today) {
        date->delete_occurrence(d);
        return;
    }

    // Deleting "today" -- do it by setting range
    Date f;
    if (date->first(f) && (f <= today)) date->set_start(today+1);
}
