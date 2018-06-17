#include <string>

class FileSystem {
    public:
        static bool CreateDirectory(const std::string &path, uint32_t mode = 755);
        static std::string BaseDirectory(const std::string &path);
};
