#include <string>

class FileSystem {
    public:
        static bool CreateDirectory(const std::string &path);
        static std::string BaseDirectory(const std::string &path);
};
