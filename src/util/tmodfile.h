#ifndef  _TMMUTIL_TMODFILE_H_
#define  _TMMUTIL_TMODFILE_H_

#include <unordered_map>
#include <vector>
#include <string>

namespace TMM {
class TmodFile {
    public:
        TmodFile(const std::string &path);
        int Read();
        std::vector<uint8_t> GetFileData(const std::string &fileName);
        const std::string &GetDescription();
        const std::string &GetName();
        const std::string &GetVersion();
    private:
        std::string m_name;
        std::string m_version;
        std::string m_path;
        std::string m_tModLoaderVersion;
        std::string m_description;
        uint8_t m_hash[20];
        uint8_t m_signature[256];
        long int m_dataLoc;
        std::unordered_map<std::string, long unsigned int> m_files;
};
}

#endif //_TMMUTIL_TMODFILE_H_
