#include <sys/types.h>
#include <stdlib.h>

#include <stddef.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

#include "lexer.h"
#include "misc.h"
#include "uid.h"
static bool backup_file(char const* src, char const* dst, long mode);
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
    char* lastSlash = strrchr(const_cast<char*>(name), '\\');
    int dirlen = lastSlash + 1 - name;
    tmp = new char[dirlen+1];
    strncpy(tmp, name, dirlen);
    tmp[dirlen] = '\0';
    
    dirName = tmp;

    // Get temporary file name.  Make sure it works even
    // on systems with a 14 character file name limit.
    tmp = new char[strlen(dirName)+20];
    sprintf(tmp, "%sical%d~", dirName, GetCurrentProcessId());
    tmpName = tmp;

    lastModifyValid = false;

    modified = false;
    delete fileName;
    delete backupName;
    delete tmpName;
    delete dirName;
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
    long mode = buf.st_mode & 07777;
    // See if file is a link, or if the containing directory is write-protected
    if (is_slink || (buf.st_nlink > 1) || (_access(dirName, W_OK) < 0)) {
        // Backup by copying to preserve links
        return WriteInPlace(mode);
        // Backup by renaming old version if possible
        return WriteNew(mode);
bool CalFile::WriteNew(long mode) {
        remove(tmpName);
    if (_chmod(tmpName, mode) < 0) {
        /* Could not set new file mode */
        lastError = strerror (errno);
        remove(tmpName);
        return false;

    // We could conceivably do more sanity checks here.

    // Create backup file.
    // We could check for errors and fail here, but that seems too paranoid.
    //link(fileName, backupName);
    remove(backupName);
    if (rename(fileName, backupName) < 0) {
        lastError = strerror(errno);
        remove(tmpName);
    // Now rename the new version
    if (rename(tmpName, fileName) < 0) {
        lastError = strerror (errno);
        remove(tmpName);
bool CalFile::WriteInPlace(long mode) {
    CopyBackup(mode);
    if (WriteTo(calendar, fileName)) {
bool CalFile::CopyBackup(long mode) {
    if (backup_file(fileName, backupName,          mode)) return true;
    if (backup_file(fileName, home_backup_file(),  mode)) return true;
    if (backup_file(fileName, tmp_backup_file(),   mode)) return true;
Calendar* CalFile::ReadFrom(const char* name) {
    Lexer input(name);
    if (! cal->Read(&input)) {
bool CalFile::WriteTo(Calendar* cal, const char* name) {
    FILE* output = fopen(name, "w");
    if (!output) {
        lastError = "could not open file for writing calendar";
    cal->Write(output);
    fflush(output);
    if (ferror(output) || (fsync(_fileno(output)) < 0)) {
        lastError = "error writing calendar file";
        fclose(output);
    fclose(output);
bool CalFile::GetModifyTime(char const* file, Time& t) {
    int ret = stat(file, &buf);
    if (_access(fileName, W_OK) < 0) {
static bool backup_file(char const* src, char const* dst, long mode) {
    if (dst == nullptr) return false;
    if (!copy_file(src, dst)) return false;
    // XXX Ignoring error while changing mode of backup file
    _chmod(dst, mode);