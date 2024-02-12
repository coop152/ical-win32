/* Copyright (c) 1993 by Sanjay Ghemawat */

#ifndef _CALENDARH
#define _CALENDARH

#include "options.h"
#include "item.h"
#include "lexer.h"
#include <vector>
#include <set>
#include <fstream>

class Calendar {
  public:
    Calendar();                 /* Create a new calendar */
    ~Calendar();

    /*
     * Item list.
     */
    int Size();                         /* Number of items */
    Item* Get(int);                     /* Get the ith item */
    void Add(Item*);                    /* Add an item */
    void Remove(Item*);                 /* Remove an item */
    void SoftDelete(Item*);             /* Move an item into the delete history */
    void Restore(Item*);                /* Move an item from the delete history back into the calendar */

    /*
     * Included calendars.
     */
    void Include(char const*);
    void Exclude(char const*);

    int  NumIncludes() const;
    char const* GetInclude(int) const;

    /*
     * True iff this calendar is read-only.
     */
    bool ReadOnly() const { return readonly; }
    void SetReadOnly(bool t) { readonly = t; }

    bool HistoryMode() const { return historyMode; }
    void setHistoryMode(bool t) { historyMode = t; }

    /*
     * Read/Write.
     */
    bool Read(Lexer*);
    void Write(std::ofstream&, bool delete_history = false) const;

    bool ReadDeleteHistory(Lexer*);
    // effects - Takes a lexer for the delete history file and loads in the deleted items list.

    bool Hidden(char const* uid) const;
    // effects - Returns true iff item named by uid should be hidden.

    void Hide(char const* uid);
    // effects - Add specified uid to set of uids of hidden items.

    void RestrictHidden(std::set<char const*> set);
    // effects - Restrict hidden items to the specified set of items.

    // Options...

    char const* GetOption(char const* key) const;
    // effects  - Return value associated with option named "key".
    //            Returns 0 if option is not found.

    void SetOption(char const* key, char const* value);
    // modifies - this
    // effects  - add "<key, value>" to option list.

    void RemoveOption(char const* key);
    // modifies - this
    // effects  - Remove any option associated with "key"

    bool Visible() const { return GetOption("Visible")[0] == '1'; }

  protected:
    std::vector<Item*> items;           // Items
    std::vector<Item*> deleted;         // Items in the delete history (stored in .del calendar file)
    std::vector<char*> includes;        // Included calendars
    bool readonly;                      // Readonly calendar?
    bool historyMode;                   // Is the delete history currently being viewed?
    std::set<char const*> hidden;       // Hidden items from other calendars
    OptionMap* options;                 // Calendar options

    void clear();
    // modifies this
    // effects  Restores calendar to pristine state.
};

#endif /* _CALENDARH */
