/* Copyright (c) 1993 by Sanjay Ghemawat */


#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "calendar.h"
#include "misc.h"
#include "uid.h"
#include "version.h"

#include "arrays.h"

// Calendar options with default values
struct OptionDesc {
    char const* key;
    char const* val;
};

static OptionDesc option_list[] = {
    { "DefaultEarlyWarning",    "1"             },
    { "DefaultAlarms",          "0 5 10 15"     },

    { "DayviewTimeStart",       "8"             },
    { "DayviewTimeFinish",      "18"            },
    { "ItemWidth",              "9"             },
    { "NoticeHeight",           "6"             },

    { "AmPm",                   "1"             },
    { "MondayFirst",            "0"             },
    { "AllowOverflow",          "1"             },

    { "Visible",                "1"             },
    { "IgnoreAlarms",           "0"             },
    { "Color",            "<Default> <Default>" },
    { "Timezone",               "<Local>"       },

    { nullptr,                  nullptr         }
};

static OptionMap* option_default = nullptr;

Calendar::Calendar(): items(), deleted(), includes(), hidden()
{
    // Initialize default option map if not done already
    if (option_default == nullptr) {
        option_default = new OptionMap;
        for (int i = 0; option_list[i].key != nullptr; i++) {
            option_default->store(option_list[i].key, option_list[i].val);
        }
    }
    
    readonly = 0;
    options = new OptionMap;
}

Calendar::~Calendar() {
    clear();
    delete options;
}

void Calendar::clear() {
    int i;

    for (i = 0; i < items.size(); i++) {
        Item* item = (Item*) items[i];
        delete item;
    }
    items.clear();

    for (i = 0; i < includes.size(); i++) {
        char* includeName = (char*) includes[i];
        delete includeName;
    }
    includes.clear();

    for (char const* s : hidden) {
        delete[] s;
    }
    hidden.clear();

    delete options;
    options = new OptionMap;

    readonly = 0;
}

void Calendar::Add(Item* item) {
    items.push_back(item);
}

void Calendar::Remove(Item* item) {
    for (int i = 0; i < items.size(); i++) {
        if (items[i] == item) {
            /* Found it */
            items.erase(items.begin() + i);
            break;
        }
    }
}

void Calendar::SoftDelete(Item* item) {
    //??? Should this check if the item actually exists and error out if it doesn't?
    Remove(item);
    deleted.push_back(item);
}

void Calendar::Expunge(Item* item) {
    for (int i = 0; i < deleted.size(); i++) {
        if (deleted[i] == item) {
            /* Found it */
            deleted.erase(deleted.begin() + i);
            break;
        }
    }
}

bool Calendar::Read(Lexer* lex) {
    clear();

    if (lex->Status() == Lexer::Error) {
        /* No input file at all */
        return true;
    }

    int file_major, file_minor;
    const char* modifier;

    if (! lex->SkipWS() ||
        ! lex->Skip("Calendar") ||
        ! lex->SkipWS()) {
        lex->SetError("file does not contain calendar");
        return false;
    }

    /*
     * Get file version number.
     * We can only understand version numbers that have a major
     * component <= to my major version number and >= 2.
     * Version 1 is not supported anymore.
     */

    if (! lex->Skip('[') ||
        ! lex->Skip('v') ||

        ! lex->GetNumber(file_major) ||
        ! (file_major >= 2) ||
        ! (file_major <= VersionMajor) ||

        ! lex->Skip('.') ||

        ! lex->GetNumber(file_minor) ||
        ! (file_minor >= 0) ||

        ! lex->GetUntil(']', modifier) ||
        /* Possibly check modifier here */

        ! lex->Skip(']')) {
        lex->SetError("illegal version");
    }

    while (1) {
        char c;

        lex->SkipWS();
        lex->Peek(c);

        switch (lex->Status()) {
          case Lexer::Eof:
            return true;
          case Lexer::Error:
            return false;
          default:
            break;
        }

        char const* keyword;

        if (! lex->GetId(keyword) ||
            ! lex->SkipWS() ||
            ! lex->Skip('[')) {
            lex->SetError("error reading item header");
            return false;
        }

        if (strcmp(keyword, "Appt") == 0) {
            Item* item = new Appointment;
            if (! item->Read(lex)) {
                delete item;
                return false;
            }
            Add(item);
        }
        else if (strcmp(keyword, "Note") == 0) {
            Item* item = new Notice;
            if (! item->Read(lex)) {
                delete item;
                return false;
            }
            Add(item);
        }
        else if (strcmp(keyword, "IncludeCalendar") == 0) {
            /* Read the name */
            char const* name;
            if (!lex->GetString(name)) {
                lex->SetError("error reading included file name");
                return false;
            }
            Include(name);
        }
        else if (strcmp(keyword, "Hide") == 0) {
            char const* x;
            if (!lex->SkipWS() || !lex->GetUntil(']', x)) {
                lex->SetError("error reading hidden item uid");
                return false;
            }
            Hide(x);
        }
        else {
            // Stash away the keyword because lex->GetString overwrites it.
            char* key = copy_string(keyword);
            char const* val;

            if (!lex->GetString(val)) {
                lex->SetError("error reading calendar property");
                delete [] key;
                return false;
            }

            // Enter option into this calendar
            options->store(key, val);

            delete [] key;
        }

        if (! lex->SkipWS() ||
            ! lex->Skip(']')) {
            lex->SetError("incomplete item");
            return false;
        }
    }
}

void Calendar::Write(FILE* file, bool delete_history) const {
    charArray* out = new charArray;

    format(out, "Calendar [v%d.%d]\n", VersionMajor, VersionMinor);
    options->write(out);
    for (int i = 0; i < includes.size(); i++) {
        char const* name = (char const*) includes[i];
        append_string(out, "IncludeCalendar [");
        Lexer::PutString(out, name);
        append_string(out, "]\n");
    }
    // if delete_history is true, write out the delete history instead
    const std::vector<Item*>& to_write = delete_history ? deleted : items; 
    for (int i = 0; i < to_write.size(); i++) {
        Item* item = (Item*) to_write[i];

        if (item->AsNotice() != nullptr) {
            append_string(out, "Note [\n");
        }
        else {
            append_string(out, "Appt [\n");
        }
        item->Write(out);
        append_string(out, "]\n");
    }

    for (char const* s : hidden) {
        format(out, "Hide [%s]\n", s);
    }

    // Just dump array out to file.
    out->append('\0');
    fputs(out->as_pointer(), file);
    delete out;
}

int Calendar::Size() const {
    return items.size();
}

void Calendar::Include(char const* name) {
    includes.push_back(copy_string(name));
}

void Calendar::Exclude(char const* name) {
    for (int i = 0; i < includes.size(); i++) {
        if (strcmp(name, includes[i]) == 0) {
            char* includeName = includes[i];
            delete includeName;

            /* Shift other includes over */
            includes.erase(includes.begin() + i);
            return;
        }
    }
}

int Calendar::NumIncludes() const {
    return includes.size();
}

char const* Calendar::GetInclude(int i) const {
    return ((char const*) includes[i]);
}

Item* Calendar::Get(int i) const {
    return (Item*) items[i];
}

bool Calendar::Hidden(char const* uid) const {
    return hidden.contains(uid);
}

void Calendar::Hide(char const* uid) {
    hidden.insert(copy_string(uid));
}

void Calendar::RestrictHidden(std::set<char const*> set) {
    std::set<char const*> to_remove;

    // Collect list of hide entries that will be removed 
    for (const char* h : hidden) {
        if (!set.contains(h)) {
            to_remove.insert(h);
        }
    }
    // Remove the collected entries.
    for (const char* r : to_remove) {
        hidden.erase(r);
        delete [] r;
    }
}

char const* Calendar::GetOption(char const* key) const {
    char const* val;
    if (options->fetch(key, val)) return val;
    if (option_default->fetch(key, val)) return val;
    return nullptr;
}

void Calendar::SetOption(char const* key, char const* val) {
    options->store(key, val);
}

void Calendar::RemoveOption(char const* key) {
    options->remove(key);
}
