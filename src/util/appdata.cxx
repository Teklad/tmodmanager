#include "appdata.h"
#include <sys/types.h>
#include <sys/stat.h>

#include <dirent.h>
#include <pwd.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

std::string AppData::GetUserDirectory()
{
#ifdef _WIN32
//TODO: Windows implementation of this
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

std::vector<std::string> AppData::GetModList(const std::string &directory)
{
    std::vector<std::string> mods;
    DIR *dir;
    struct dirent *ent;

    dir = opendir(directory.c_str());
    if (dir == NULL) {
        return mods;
    }

    while ((ent = readdir(dir)) != NULL) {
        char *ext_start = strrchr(ent->d_name, '.');
        if (ext_start != NULL && strcmp(".tmod", ext_start) == 0) {
            mods.push_back(ent->d_name);
        }
    }

    closedir(dir);
    return mods;
}
