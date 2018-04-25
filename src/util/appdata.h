#ifndef  __APPDATA_H__
#define  __APPDATA_H__
#include <string>

/**
 * @brief Very basic class for the few filesystem operations we need
 *        to perform.
 */
class AppData {
    public:
        static std::string GetUserDirectory();
        static std::string GetModDirectory();
        static bool IsDirectoryValid(const std::string &dir);
};
#endif //__APPDATA_H__
