#ifndef  _TMMUTIL_TMODFILE_H_
#define  _TMMUTIL_TMODFILE_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "binaryreader.h"

namespace TMM {
/**
 * @brief This class reads and stores tmod file information for access in the UI.
 *        Most of the functions in here have been borrowed from their C# counterparts from
 *        tModLoader.
 */
class TmodFile {
    public:
        TmodFile(const std::string &path);
        ~TmodFile();
        int Read();
        std::vector<uint8_t> GetFileData(const std::string &fileName);
        const std::string &GetName();
        const std::string &GetVersion();
        std::string GetProperty(const std::string &property);
    private:
        struct Properties {
            std::vector<std::string> dllReferences;
            std::vector<std::string> modReferences;
            std::vector<std::string> weakReferences;
            std::vector<std::string> sortAfter;
            std::vector<std::string> sortBefore;
            std::string author;
            std::string version;
            std::string displayName;
            std::string homepage;
            std::string description;
            bool noCompile = false;
            bool hideCode = true;
            bool hideResources = true;
            bool includeSource = false;
            bool includePDB = false;
            bool editAndContinue = false;
            uint8_t side;
        };
        void FillProperties(BinaryReader *reader);
        std::vector<std::string> ReadList(BinaryReader *reader);
        std::string m_name;
        std::string m_version;
        std::string m_path;
        std::string m_tModLoaderVersion;
        uint8_t m_hash[20];
        uint8_t m_signature[256];
        long int m_dataLoc;
        std::unordered_map<std::string, long unsigned int> m_files;
        Properties m_properties;
};
}

#endif //_TMMUTIL_TMODFILE_H_
