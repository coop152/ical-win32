/* Copyright (c) 1993 by Sanjay Ghemawat */

#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#define _stat stat
#define _access access
#define _chmod chmod
#define _fileno fileno
#define GetCurrentProcessId getpid
#endif
#include <filesystem>

#include "calfile.h"
#include "calendar.h"
#include <fstream>

namespace fs = std::filesystem;

// Only use "fsync" if it is available.
#ifdef _WIN32
static int fsync(int) {return 0;}
#endif

#define W_OK 2 // flag for write access using access()

// Get various file names
static char const* home_backup_file();  // Backup file in home dir
static char const* tmp_backup_file();   // Backup file in tmp dir

static bool backup_file(std::string src, std::string dst);
// effects      Backup file named "src" to file named "dst".
//              Change the mode of "dst" to "mode".
//              Return true iff successful.

const char* CalFile::lastError = "no error";

CalFile::CalFile(bool ro, const char* name) {
    readOnly = ro;
    lastModifyValid = false;
    modified = false;

    fileName = name;
    backupName = fileName + "~";
    dirName = fs::path(name).remove_filename().string();
    fs::path tmpPath = fs::path(dirName) / std::format("ical{}~", GetCurrentProcessId());
    tmpName = tmpPath.string();

    calendar = new Calendar;
    calendar->SetReadOnly(readOnly);
}

CalFile::~CalFile() {
    delete calendar;
}

void CalFile::Modified() {
    modified = true;
}

bool CalFile::Write() {
    if (!fs::exists(fileName)) {
        // original file doesn't even exist - write directly
        // also write delete history file
        if (WriteTo(calendar, fileName) && WriteTo(calendar, fileName+".del", true)) {
            written();
            return true;
        }
        return false;
    }


    if (fs::is_symlink(fileName) || (_access(dirName.c_str(), W_OK) < 0)) {
        // file is a link, or the containing directory is write-protected
        return WriteInPlace();
    }
    else {
        // backup by renaming old version
        return WriteNew();
    }
}

// Write calendar to temporary location, rename the current version
// to a backup file and then rename the new version to the right
// file name.

bool CalFile::WriteNew() {
    // try to write to the temp file
    if (!WriteTo(calendar, tmpName)) {
        fs::remove(tmpName);
        return false;
    }


    // delete the old backup file, rename the old calendar file into the new backup,
    // and rename the temp file into the new calendar file.
    try {
        fs::remove(backupName);
        fs::rename(fileName, backupName);
        fs::rename(tmpName, fileName);
    }
    catch (fs::filesystem_error& e) {
        lastError = e.what();
        fs::remove(tmpName);
        return false;
    }

    // try to write delete history file
    if (!WriteTo(calendar, fileName+".del", true)) {
        return false;
    }

    written();
    return true;
}

// First backup the current stable version of the calendar, and then
// write a new version in place.

bool CalFile::WriteInPlace() {
    // XXX Should we just ignore errors while making a backup?
    CopyBackup();

    if (WriteTo(calendar, fileName) && WriteTo(calendar, fileName+".del", true)) {
        written();
        return true;
    }

    return false;
}

bool CalFile::CopyBackup() {
    // Try backing up in various place until we succeed:
    //
    // * backupName
    // * In home directory
    // * In tmp directory

    if (backup_file(fileName, backupName)) return true;
    if (backup_file(fileName, home_backup_file())) return true;
    if (backup_file(fileName, tmp_backup_file())) return true;

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

Calendar* CalFile::ReadFrom(std::string name) {
    Calendar* cal = new Calendar;
    Lexer input(name.c_str());
    Lexer historyInput((name + ".del").c_str());

    // read calendar, failing if it can't be read
    if (!cal->Read(&input)) {
        lastError = input.LastError();
        delete cal;
        cal = nullptr;
    }

    // try to read the corresponding delete history file, doing nothing if it fails
    cal->ReadDeleteHistory(&historyInput);

    return cal;
}

bool CalFile::WriteTo(Calendar* cal, std::string name, bool delete_history) {
    std::ofstream outfile(name, std::ios_base::binary); // binary mode as to not alter the line endings (for cross-OS compatibility)

    if (!outfile) {
        lastError = "Could not open calendar file for writing.";
        return false;
    }

    cal->Write(outfile, delete_history);
    if (outfile.bad()) {
        lastError = "Error writing to calendar file.";
        return false;
    }

    return true;
}

bool CalFile::GetModifyTime(std::string file, Time& t) {
    struct stat buf;

    int ret = stat(file.c_str(), &buf);
    if (ret < 0) return false;

    t = Time(buf.st_mtime);
    return true;
}

void CalFile::PerformAccessCheck() {
    calendar->SetReadOnly(readOnly);

    if (_access(fileName.c_str(), W_OK) < 0) {
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

static bool backup_file(std::string src, std::string dst) {
    if (!copy_file(src.c_str(), dst.c_str())) return false;

    fs::permissions(dst, fs::perms::all, fs::perm_options::add);
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
