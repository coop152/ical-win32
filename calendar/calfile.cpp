#include <fstream>

namespace fs = std::filesystem;
static bool backup_file(std::string src, std::string dst);
    modified = false;

    fileName = name;
    backupName = fileName + "~";
    dirName = fs::path(name).remove_filename().string();
    tmpName = std::format("{}\\ical{}~", dirName, GetCurrentProcessId()); // these backslashes aren't linux safe!
    if (!fs::exists(fileName)) {
        // original file doesn't even exist - write directly
        // also write delete history file
        if (WriteTo(calendar, fileName) && WriteTo(calendar, fileName+".del", true)) {
            written();
            return true;
    if (fs::is_symlink(fileName) || (_access(dirName.c_str(), W_OK) < 0)) {
        // file is a link, or the containing directory is write-protected
        return WriteInPlace();
        // backup by renaming old version
        return WriteNew();
bool CalFile::WriteNew() {
    // try to write to the temp file
        fs::remove(tmpName);
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
    // try to write delete history file
    if (!WriteTo(calendar, fileName+".del", true)) {
bool CalFile::WriteInPlace() {
    CopyBackup();
    if (WriteTo(calendar, fileName) && WriteTo(calendar, fileName+".del", true)) {
bool CalFile::CopyBackup() {
    if (backup_file(fileName, backupName)) return true;
    if (backup_file(fileName, home_backup_file())) return true;
    if (backup_file(fileName, tmp_backup_file())) return true;
    Calendar* old = nullptr;
    if (cal != nullptr) {
Calendar* CalFile::ReadFrom(std::string name) {
    Lexer input(name.c_str());
    Lexer historyInput((name + ".del").c_str());
    // read calendar, failing if it can't be read
    if (!cal->Read(&input)) {
    // try to read the corresponding delete history file, doing nothing if it fails
    cal->ReadDeleteHistory(&historyInput);

bool CalFile::WriteTo(Calendar* cal, std::string name, bool delete_history) {
    std::ofstream outfile(name);

    if (!outfile) {
        lastError = "Could not open calendar file for writing.";
    cal->Write(outfile, delete_history);
    if (outfile.bad()) {
        lastError = "Error writing to calendar file.";
bool CalFile::GetModifyTime(std::string file, Time& t) {
    int ret = stat(file.c_str(), &buf);
    if (_access(fileName.c_str(), W_OK) < 0) {
static bool backup_file(std::string src, std::string dst) {
    if (!copy_file(src.c_str(), dst.c_str())) return false;
    fs::permissions(dst, fs::perms::all, fs::perm_options::add);
        if (uid != nullptr) {