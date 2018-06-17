#ifndef  _TMMUTIL_BINARYREADER_H_
#define  _TMMUTIL_BINARYREADER_H_

#include <stdint.h>
#include <string>
#include <vector>

namespace TMM {
/**
 * @brief Partial reimplementation of C#'s BinaryReader class in order to properly read tmod
 *        file data.
 */
class BinaryReader {
    public:
        BinaryReader(FILE *file);
        BinaryReader(const std::string &fpath);
        ~BinaryReader();
        bool SetFile(FILE *file);
        bool SetFile(const std::string &fpath);
        bool IsValid() { return (m_file != nullptr); }
        void SkipBytes(long int count);
        std::vector<uint8_t> ReadBytes(long int count);
        void SetPosition(long int count);
        long int GetPosition();
        std::string ReadString();
        uint8_t ReadByte();
        int ReadInt32();
    private:
        FILE *m_file = nullptr;
        int Read7BitEncodedInt();
};
}

#endif //_TMMUTIL_BINARYREADER_H_
