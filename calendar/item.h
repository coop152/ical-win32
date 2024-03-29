/* Copyright (c) 1993 by Sanjay Ghemawat */

#ifndef _ITEMH
#define _ITEMH

#include "Date.h"

#include "dateset.h"
#include "smallintset.h"
#include "misc.h"
#include "options.h"

class Lexer;

class Notice;
class Appointment;

/*
 * Item
 *
 * Abstract components.
 *
 * Text: string                 associated text
 * Dates: DateSet               occurrence dates
 * RemindStart: int             No. of days before item when reminder begins.
 * Owner: string                User id of person who last changed the item
 * Uid: string                  Unique id for item
 * Hilite: string               Hilite mode
 * Todo: bool                   Is Item a to-do item
 * Done: bool                   Has item been marked "done"
 * Important: bool              Has item been marked "important",
 *                                  meaning it shouldn't be deleted in mass-delete operations
 *
 * AsNotice: Notice*            Typecast to Notice if legal.  Null otherwise.
 * AsAppointment: Appointment*  Typecast to Appointment if legal ... 
 *
 */

class Item {
  public:
    Item();
    virtual ~Item();

    virtual bool Read(Lexer*);
    virtual bool Parse(Lexer*, char const* keyword);

    virtual operator std::string() const;

    virtual Item* Clone() const = 0;

    const char* GetText() const { return text; }
    void SetText(const char*s) { delete [] text; text = copy_string(s); }

    // DateSet interface
    virtual bool contains(Date d) const;
    bool repeats() const { return date->repeats(); }
    bool empty() const { return date->empty(); }
    DateSet::RepeatType repeat_type() const { return date->type(); }
    void describe(charArray* buffer) const { date->describe(buffer); }
    void describe_terse(charArray* buffer) const {date->describe_terse(buffer); }

    bool first(Date& result) const;
    bool next(Date d, Date& result) const;
    bool range(Date&, Date&) const;

    void set_empty() { date->set_empty(); }
    void set_date(Date d) { date->set_date(d); }
    void set_day_based_repeat(int interval, Date anchor)
    { date->set_day_based_repeat(interval, anchor); }

    void set_month_based_repeat(int interval, Date anchor)
    { date->set_month_based_repeat(interval, anchor); }
    void set_monthly_by_days(int c, int i, Date anchor, int back)
    { date->set_monthly_by_days(c, i, anchor, back); }
    void set_monthly_by_workdays(int c, int i, Date anchor, int back)
    { date->set_monthly_by_workdays(c, i, anchor, back); }
    void set_monthly_by_weeks(int c, WeekDay w, int i, Date anchor, int back)
    { date->set_monthly_by_weeks(c, w, i, anchor, back); }

    void set_month_set(SmallIntSet days, SmallIntSet months)
    { date->set_month_set(days, months); }
    void set_week_set(SmallIntSet days, SmallIntSet months)
    { date->set_week_set(days, months); }

    void set_start(Date start);
    void set_finish(Date finish);

    void delete_occurrence(Date d);

    int GetRemindStart() const { return remindStart; }
    void SetRemindStart(int r) { remindStart = r; }

    const char* GetOwner() const { return owner; }
    // effects - Return the owner.  The returned string is guaranteed
    //           to remain valid until the item is modified or deleted.

    void SetOwner(char const* o) { delete [] owner; owner = copy_string(o); }
    // modifies - "this"
    // effects  - Make "o" the owner of the item.

    void MakeOwner() { SetOwner(my_name()); }
    // modifies - this
    // effects  - Make the current user the owner of this item.

    bool IsMine() const { return strcmp(my_name(), owner) == 0; }
    // effects  - Returns true iff this item is owned by the current user.

    char const* GetUid() const { return uid; }

    // effects - Return the uid.  The returned string is guaranteed
    //           to remain valid until the item is deleted.


    bool IsUidPersistent() const { return uid_persistent; }
    // effects - Return true iff uid is also stored persistently

    virtual Notice* AsNotice() { return nullptr; }
    virtual Appointment* AsAppointment() { return nullptr; }

    /* Mark deleted */
    bool Deleted() { return deleted; }
    void MarkDeleted() { deleted = true; }

    /* Hilite mode */
    char const* Hilite() const { return hilite; }
    void Hilite(char const* s) { delete [] hilite; hilite = copy_string(s); }

    /* Todo? */
    bool IsTodo() const { return todo; }
    void SetTodo(bool t) { todo = t; }

    /* Done? */
    bool IsDone() const { return done; }
    void SetDone(bool d) { done = todo && d; }

    /* Important? */
    bool IsImportant() const { return important; }
    void SetImportant(bool i) { important = i; }

    /* Options */
    char const* GetOption(char const* key) const;
    // effects  - Return value associated with option named "key".
    //            Returns 0 if option is not found.

    void SetOption(char const* key, char const* value);
    // modifies - this
    // effects  - add "<key, value>" to option list.

    void RemoveOption(char const* key);
    // modifies - this
    // effects  - Remove any option associated with "key"

    /* Comparison */
    bool similar(Item const* x) const;
    // effects  Returns true iff this has same contents as x.

    static const int defaultRemindStart;

  protected:
    char*       text;
    char*       owner;
    char*       uid;
    bool        uid_persistent;
    int         remindStart;    /* Start reminding early */
    bool        deleted;
    DateSet*    date;
    char*       hilite;
    bool        todo;
    bool        done;
    bool        important;
    OptionMap   options;

    void CopyTo(Item*) const;
};

/*
 * Notice
 *
 * An Item with just a length.
 * The length is just an indication of how big a window the notice must
 * be displayed in.  The length field can be interpreted just as the length
 * field for Appointments, and if notice->length == appt->length, then
 * the notice will be displayed the same size as the appt.
 */
class Notice : public Item {
  public:
    Notice()  {}
    ~Notice() {}

    virtual Item* Clone() const;

    virtual Notice* AsNotice() { return this; }
};

/*
 * Appointment
 *
 * An Item with a start time and a length.
 * Both start and length are in minutes.
 * Start is measured from midnight.
 */
class Appointment : public Item {
  public:
    Appointment() : start(30), length(30), alarms(nullptr)
    { timezone=copy_string("<Local>"); cache.invalidate(); }

    ~Appointment() {
        delete [] timezone;
        if (alarms != nullptr) delete alarms;
    }

    virtual bool Parse(Lexer*, char const* keyword);

    virtual operator std::string() const;

    virtual Item* Clone() const;

    virtual bool contains(Date d) const;

    int GetStart(int d) const { return d < 0 ? start : time_from_tz(Date(d), start); }
    void SetStart(int d, int s) { start = d < 0 ? s : time_to_tz(Date(d), s); }

    int GetLength() const { return length; }
    void SetLength(int l) { length = l; }

    const char* GetTimezone() const { return timezone; }
    void SetTimezone(const char*tz, bool convert_remote) {
        Date d;
        cache.invalidate();
        if (convert_remote && date->first(d))
            start=time_from_tz(d,start);
        else 
            convert_remote=false;
        delete [] timezone;
        timezone = copy_string(tz);
        if (convert_remote) start=time_to_tz(d,start);
    }

    intArray* GetAlarms() const { return alarms; }
    // effects - Return array of alarm times.  If no alarm times
    //           have been set for this item, return NULL.
    //           The returned array is guaranteed to remain
    //           valid until the item is modified or deleted.

    void SetAlarms(intArray* list) {
        if (alarms == nullptr) alarms = new intArray;
        alarms->clear();
        *alarms = *list;
    }
    // modifies - this
    // effects  - Sets alarms for this item from the contents of list.

    virtual Appointment* AsAppointment() { return this; }

  private:
    int start;
    int length;
    intArray* alarms;           /* Alarm times */
    const char *timezone;

    bool has_timezone() const { return timezone[0] != '<'; }
    int time_from_tz(Date d, int t) const
    { if (has_timezone()) convert_tz(d, t, false); return t; }
    int time_to_tz(Date d, int t) const
    { if (has_timezone()) convert_tz(d, t, true); return t; }
    void datetime_to_tz(Date &d, int &t) const
    { if (has_timezone()) convert_tz(d, t, true); }

    void convert_tz(Date &d, int &min, bool to_tz) const;
    /*
     * tcl code calls starttime for example many times per appt.
     * cache the last result to avoid expensive timezone conversions
     */
    mutable struct {
        int from_min = -1, to_min = -1;
        Date from_d, to_d;
        bool to_tz = false;
        void invalidate() { from_min=-1; }
    } cache;
};

#endif /* _ITEMH */
