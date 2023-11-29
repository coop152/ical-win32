/* Copyright (c) 1993 by Sanjay Ghemawat */

#ifndef _CALFILEH
#define _CALFILEH

/*
 * Calendar file encapsulation.
 */

#include "Time_.h"

class Calendar;

class CalFile {
  public:
    CalFile(bool readOnly, const char* name);
    ~CalFile();

    /*
     * Get file name.
     */
    char const* GetName();

    /*
     * Get associated calendar.
     */
    Calendar* GetCalendar();

    /*
     * Has calendar been modified.
     */
    bool IsModified();

    /*
     * Called on each modification.
     */
    void Modified();

    enum CalFile_Result {
        Success,
        Cancel,
        Failure
        };

    /*
     * Write calendar out to file.
     * Return true only on success.
     */
    bool Write();

    /*
     * Read calendar from file.
     */
    bool Read();

    /*
     * Has file changed since last read?
     */
    bool FileHasChanged();

    /*
     * Re-read file contents from disk.
     * Calendar is not changed on failure.
     * Returns nil on error, old calendar on success.
     */
    Calendar* ReRead();

    /*
     * Get last error.
     */
    static char const* LastError();
  protected:
    bool        readOnly;             /* Read-only mode? */
    std::string fileName;             /* Calendar file name */
    std::string backupName;           /* Backup file name */
    std::string tmpName;              /* Tmp file name */
    std::string dirName;              /* Directory name */

    Calendar* calendar;         /* Volatile calendar state */

    // Called after writing to clear modify flag and also
    // update last modify time.
    void  written();
    bool  modified;             /* Has cal been modified since last write */
    Time  lastModifyTime;       /* Last calendar file modify time */
    bool  lastModifyValid;      /* Is lastModifyTime valid? */

    /* Utility routines */
    static bool GetModifyTime(std::string, Time&);

    // Update calendar readonly status.
    void PerformAccessCheck();

    /* Last error */
    static const char* lastError;

    // Read/Write calendar from/to named file
    static Calendar* ReadFrom(std::string);
    static bool WriteTo(Calendar*, std::string, bool delete_history = false);

    // Other internal IO routines
    bool WriteNew();            // Save without writing in-place
    bool WriteInPlace();        // Write in-place to preserve links
    bool CopyBackup();          // Make a backup by copying
};

inline char const* CalFile::GetName() {
    return fileName.c_str();
}

inline Calendar* CalFile::GetCalendar() {
    return calendar;
}

inline bool CalFile::IsModified() {
    return modified;
}

inline const char* CalFile::LastError() {
    return lastError;
}

#endif /* _CALFILEH */
