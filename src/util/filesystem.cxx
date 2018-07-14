#include "filesystem.h"

#ifdef _WIN32
#include <direct.h>
#else
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

#include <iostream>
#include <string>

#ifdef _WIN32
#define DIRSEP '\\'
#define MAKEDIR(X,Y) _mkdir(X)
#else
#define DIRSEP '/'
#define MAKEDIR(X,Y) mkdir(X,Y)
#endif

bool FileSystem::CreateDirectory(const std::string &path)
{
    size_t pos = path.find(DIRSEP);
    while (pos != std::string::npos) {
        std::string currentDirectory = path.substr(0, pos);
        int status = MAKEDIR(currentDirectory.c_str(), 0755);
        if (errno != EEXIST && errno != 0) {
            std::cout << "Error: " << errno << '\n';
            return false;
        }
        pos = path.find(DIRSEP, pos + 1);
    }
    int status = MAKEDIR(path.c_str(), 0755);
    if (errno != EEXIST && errno != 0) {
        return false;
    }
    return true;
}

std::string FileSystem::BaseDirectory(const std::string &path)
{
    size_t pos = path.find_last_of(DIRSEP);
    if (pos == std::string::npos) {
        return path;
    }
    return path.substr(0, pos);
}
