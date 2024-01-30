/* Copyright (c) 1993 by Sanjay Ghemawat */

#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#else
#include <unistd.h>
#define _stat stat
#define _access access
#define _chmod chmod
#define _fileno fileno
#endif
#include <sys/types.h>
#include <stdlib.h>

#include <stddef.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <filesystem>


#include "calfile.h"
#include "calendar.h"
#include "lexer.h"
#include "misc.h"
#include "uid.h"

namespace fs = std::filesystem;

// Only use "fsync" if it is available.
#ifdef _WIN32
static int fsync(int) {return 0;}
#endif

#define W_OK 2 // flag for write access using access()

// Get various file names
static char const* home_backup_file(); // Backup file in home dir
static char const* tmp_backup_file();   // Backup file in tmp dir

static bool backup_file(char const* src, char const* dst, long mode);
// effects      Backup file named "src" to file named "dst".
//              Change the mode of "dst" to "mode".
//              Return true iff successful.

const char* CalFile::lastError = "no error";

CalFile::CalFile(bool ro, const char* name) {
    readOnly = ro;

    int len = strlen(name);
    char* tmp;

    // Copy file name
    tmp = new char[len+1];
    strcpy(tmp, name);
    fileName = tmp;

    // Copy backup file name
    tmp = new char[len+2];
    sprintf(tmp, "%s~", fileName);
    backupName = tmp;

    // Get directory name for access checks
    // XXX: if an included calendar uses the wrong platform's seperator then this will crash
    // i think this is fixed in a newer branch, just don't give invalid file names in the meantime
    char* lastSlash = strrchr(const_cast<char*>(name), fs::path::preferred_separator);
    int dirlen = lastSlash + 1 - name;
    tmp = new char[dirlen+1];
    strncpy(tmp, name, dirlen);
    tmp[dirlen] = '\0';
    
    dirName = tmp;

    // Get temporary file name.  Make sure it works even
    // on systems with a 14 character file name limit.
    tmp = new char[strlen(dirName)+20];
    #ifdef _WIN32
    sprintf(tmp, "%sical%d~", dirName, GetCurrentProcessId());
    #else
    sprintf(tmp, "%sical%d~", dirName, getpid());
    #endif
    tmpName = tmp;

    lastModifyValid = false;

    calendar = new Calendar;
    calendar->SetReadOnly(readOnly);

    modified = false;
}

CalFile::~CalFile() {
    delete calendar;
    delete[] fileName;
    delete[] backupName;
    delete[] tmpName;
    delete[] dirName;
}

void CalFile::Modified() {
    modified = true;
}

bool CalFile::Write() {
    // Get information about the calendar file
    struct _stat buf;
    bool is_slink = false;

    int result = _stat(fileName, &buf);
    if ((result >= 0) && fs::is_symlink(fileName)) {
        //Get mode for real referenced file
        is_slink = true;
    }

    if (result < 0) {
        /* Could not get file mode */
        if (errno == ENOENT) {
            /* Original file does not even exist - try to write directly */
            if (WriteTo(calendar, fileName)) {
                written();
                return true;
            }
        }
        return false;
    }

    long mode = buf.st_mode & 07777;

    // See if file is a link, or if the containing directory is write-protected
    if (is_slink || (buf.st_nlink > 1) || (_access(dirName, W_OK) < 0)) {
        // Backup by copying to preserve links
        return WriteInPlace(mode);
    }
    else {
        // Backup by renaming old version if possible
        return WriteNew(mode);
    }
}

// Write calendar to temporary location, rename the current version
// to a backup file and then rename the new version to the right
// file name.

bool CalFile::WriteNew(long mode) {
    if (!WriteTo(calendar, tmpName)) {
        remove(tmpName);
        return false;
    }

    if (_chmod(tmpName, mode) < 0) {
        /* Could not set new file mode */
        lastError = strerror (errno);
        remove(tmpName);
        return false;
    }

    // We could conceivably do more sanity checks here.

    // Create backup file.
    // We could check for errors and fail here, but that seems too paranoid.
    //link(fileName, backupName);
    remove(backupName);
    if (rename(fileName, backupName) < 0) {
        lastError = strerror(errno);
        remove(tmpName);
        return false;
    }

    // Now rename the new version
    if (rename(tmpName, fileName) < 0) {
        lastError = strerror (errno);
        remove(tmpName);
        return false;
    }

    written();
    return true;
}

// First backup the current stable version of the calendar, and then
// write a new version in place.

bool CalFile::WriteInPlace(long mode) {
    // XXX Should we just ignore errors while making a backup?
    CopyBackup(mode);

    if (WriteTo(calendar, fileName)) {
        written();
        return true;
    }

    return false;
}

bool CalFile::CopyBackup(long mode) {
    // Try backing up in various place until we succeed:
    //
    // * backupName
    // * In home directory
    // * In tmp directory

    if (backup_file(fileName, backupName,          mode)) return true;
    if (backup_file(fileName, home_backup_file(),  mode)) return true;
    if (backup_file(fileName, tmp_backup_file(),   mode)) return true;

    return false;
}

bool CalFile::Read() {
    Calendar* old = ReRead();
    if (old != nullptr) {
        delete old;
        return true;
    }
    return false;
}

Calendar* CalFile::ReRead() {
    Time newFileTime;
    int gotTime = GetModifyTime(fileName, newFileTime);

    Calendar* cal = ReadFrom(fileName);
    Calendar* old = nullptr;
    if (cal != nullptr) {
        old = calendar;
        calendar = cal;
        calendar->SetReadOnly(readOnly);

        PerformAccessCheck();

        modified = 0;

        lastModifyValid = gotTime;
        if (gotTime)
            lastModifyTime = newFileTime;
    }
    return old;
}

bool CalFile::FileHasChanged() {
    PerformAccessCheck();

    Time newModifyTime;
    bool newModifyValid = GetModifyTime(fileName, newModifyTime);

    if (newModifyValid != lastModifyValid) {
        /* Ability to read file information changed */
        return true;
    }

    return (lastModifyValid && (newModifyTime != lastModifyTime));
}

Calendar* CalFile::ReadFrom(const char* name) {
    Calendar* cal = new Calendar;
    Lexer input(name);

    if (! cal->Read(&input)) {
        lastError = input.LastError();
        delete cal;
        cal = nullptr;
    }

    return cal;
}

bool CalFile::WriteTo(Calendar* cal, const char* name) {
    // temporarily making this binary so that the program writes unix line endings
    // TODO: remove this when the program can compile on linux; in that case it (should) be able to convert from CRLF automatically
    FILE* output = fopen(name, "wb");
    if (!output) {
        lastError = "could not open file for writing calendar";
        return false;
    }

    cal->Write(output);
    fflush(output);
    if (ferror(output) || (fsync(_fileno(output)) < 0)) {
        lastError = "error writing calendar file";
        fclose(output);
        return false;
    }

    fclose(output);
    return true;
}

bool CalFile::GetModifyTime(char const* file, Time& t) {
    struct stat buf;

    int ret = stat(file, &buf);
    if (ret < 0) return false;

    t = Time(buf.st_mtime);
    return true;
}

void CalFile::PerformAccessCheck() {
    calendar->SetReadOnly(readOnly);

    if (_access(fileName, W_OK) < 0) {
        switch (errno) {
          case ENOENT:
            /* File does not exist */
            break;
          case EACCES:
          case EROFS:
          case ETXTBSY:
            /* Permission denied */
            calendar->SetReadOnly(1);
            break;
          default:
            /* Should not happen if we were successfuly able to read cal */
            break;
        }
    }
}

void CalFile::written() {
    modified = 0;
    lastModifyValid = GetModifyTime(fileName, lastModifyTime);
}

static bool backup_file(char const* src, char const* dst, long mode) {
    if (dst == nullptr) return false;
    if (!copy_file(src, dst)) return false;

    // XXX Ignoring error while changing mode of backup file
    _chmod(dst, mode);
    return true;
}

static char const* home_backup_file() {
    static char const* part_name = "icalbak~";
    static char const* full_name = nullptr;
    static bool inited = 0;

    // Make sure we initialize only once
    if (!inited) {
        inited = true;
        char const* home = getenv("HOME");
        if (home != nullptr) {
            char* copy = new char[strlen(home) + strlen(part_name) + 2];
            sprintf(copy, "%s%c%s", home, fs::path::preferred_separator, part_name);

            full_name = copy;
        }
    }

    return full_name;
}

static char const* tmp_backup_file() {
    // how horrible is THIS?
    #ifdef _WIN32
    auto sys_tmp_path = std::wstring(std::filesystem::temp_directory_path().c_str());
    auto as_ascii = std::string(sys_tmp_path.begin(), sys_tmp_path.end());
    #else
    auto as_ascii = std::filesystem::temp_directory_path().string();
    #endif
    static char const* prefix = as_ascii.c_str();
    static char const* full_name = nullptr;
    static int inited = false;

    // Make sure we initialize only once
    if (!inited) {
        inited = true;
        char const* uid = my_name();
        if (uid != nullptr) {
            char* copy = new char[strlen(prefix) + strlen(uid) + 2];
            sprintf(copy, "%s%s~", prefix, uid);
            full_name = copy;
        }
    }

    return full_name;
}
