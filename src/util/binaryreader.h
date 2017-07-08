#ifndef  _TMMUTIL_BINARYREADER_H_
#define  _TMMUTIL_BINARYREADER_H_

#include <fstream>
#include <vector>

namespace TMM {
class BinaryReader {
    public:
        BinaryReader(FILE *file);
        ~BinaryReader();
        void SkipBytes(int count);
        std::vector<uint8_t> ReadBytes(int count);
        void SetPosition(int count);
        long int GetPosition();
        std::string ReadString();
        uint8_t ReadByte();
        int ReadInt32();
    private:
        FILE *m_file = nullptr;
        uint8_t *m_buffer;
        int Read7BitEncodedInt();
};
}

#endif //_TMMUTIL_BINARYREADER_H_
