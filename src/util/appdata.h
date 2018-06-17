#ifndef  __APPDATA_H__
#define  __APPDATA_H__
#include <string>
#include <vector>
/**
 * @brief Very basic class for the few filesystem operations we need
 *        to perform.
 */
class AppData {
    public:
        static std::string GetUserDirectory();
        static std::string GetModDirectory();
        static std::vector<std::string> GetModList(const std::string &directory);

        static bool IsDirectoryValid(const std::string &dir);
};
#endif //__APPDATA_H__
