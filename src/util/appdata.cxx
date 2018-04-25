#include "appdata.h"
#include <sys/types.h>
#include <sys/stat.h>

#ifdef linux
#include <pwd.h>
#include <unistd.h>
#endif

std::string AppData::GetUserDirectory()
{
#ifdef _WIN32
#else
    char *homedir = getenv("HOME");
    if (homedir == NULL) {
        struct passwd *pw = getpwuid(getuid());
        homedir = pw->pw_dir;
    }
#endif
    return std::string(homedir);
}

bool AppData::IsDirectoryValid(const std::string &dir)
{
    struct stat info;
    if ((stat (dir.c_str(), &info) == 0) && (info.st_mode & S_IFDIR)) {
        return true;
    }
    return false;
}

std::string AppData::GetModDirectory()
{
    std::string homedir = AppData::GetUserDirectory();
#ifdef _WIN32
    homedir.append("\\Documents\\My Games\\Terraria\\ModLoader\\Mods");
#else
    homedir.append("/.local/share/Terraria/ModLoader/Mods");
#endif
    return homedir;
}
