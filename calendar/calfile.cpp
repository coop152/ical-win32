static bool backup_file(char const* src, char const* dst, long mode);
CalFile::CalFile(bool ro, const char* name) {
    lastModifyValid = false;
    modified = false;
    modified = true;
bool CalFile::Write() {
    bool is_slink = false;
        is_slink = true;
                return true;
        return false;
bool CalFile::WriteNew(long mode) {
        return false;
        return false;
    if (rename(fileName, backupName) < 0) {
        lastError = strerror(errno);
        remove(tmpName);
        return false;
    }
        return false;
    return true;
bool CalFile::WriteInPlace(long mode) {
        return true;
    return false;
bool CalFile::CopyBackup(long mode) {
    if (backup_file(fileName, backupName,          mode)) return true;
    if (backup_file(fileName, home_backup_file(),  mode)) return true;
    if (backup_file(fileName, tmp_backup_file(),   mode)) return true;
    return false;
bool CalFile::Read() {
    if (old != nullptr) {
        return true;
    return false;
bool CalFile::FileHasChanged() {
    bool newModifyValid = GetModifyTime(fileName, newModifyTime);
        return true;
        cal = nullptr;
bool CalFile::WriteTo(Calendar* cal, const char* name) {
    if (!output) {
        return false;
        return false;
    return true;
bool CalFile::GetModifyTime(char const* file, Time& t) {
    if (ret < 0) return false;
    t = Time(buf.st_mtime);
    return true;
static bool backup_file(char const* src, char const* dst, long mode) {
    if (dst == 0) return false;
    if (!copy_file(src, dst)) return false;
    return true;
    static char const* full_name = nullptr;
    static bool inited = 0;
        inited = true;
        if (home != nullptr) {
    static char const* full_name = nullptr;
    static int inited = false;
        inited = true;