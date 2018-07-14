#include <assert.h>

#include "binaryreader.h"

namespace TMM {
    
BinaryReader::BinaryReader(FILE *file)
{
    SetFile(file);
}

BinaryReader::BinaryReader(const std::string &fpath)
{
    SetFile(fpath);
}

BinaryReader::~BinaryReader()
{
    if (m_file != nullptr) {
        fclose(m_file);
    }
}

bool BinaryReader::SetFile(FILE *file)
{
    if (m_file != nullptr) {
        fclose(m_file);
    }
    m_file = file;
    return IsValid();
}

bool BinaryReader::SetFile(const std::string &fpath)
{
	FILE *nfile;
    errno_t success = fopen_s(&nfile, fpath.c_str(), "rb");
	if (success != 0) {
		return false;
	}
    return SetFile(nfile);
}

std::vector<uint8_t> BinaryReader::ReadBytes(long int count)
{
    std::vector<uint8_t> result(count, 0);
    size_t total = fread(&result[0], 1, count, m_file);
    // If this check fails we've royally screwed up anyhow.
    assert(total == count);
    return result;
}

void BinaryReader::SetPosition(long int count)
{
    fseek(m_file, count, SEEK_SET);
}

void BinaryReader::SkipBytes(long int count)
{
    fseek(m_file, count, SEEK_CUR);
}

long int BinaryReader::GetPosition()
{
    return ftell(m_file);
}

std::string BinaryReader::ReadString()
{
    int len = Read7BitEncodedInt();
    if (len <= 0) {
        return {};
    }
	std::string ret(len + 1, '\0');
    fread(&ret[0], 1, len, m_file);
    return ret;
}

uint8_t BinaryReader::ReadByte()
{
    uint8_t b;
    fread(&b, 1, 1, m_file);
    return b;
}

int BinaryReader::ReadInt32()
{
    uint8_t b[4];
    fread(b, 1, 4, m_file);
    return static_cast<int>(b[0] | b[1] << 8 | b[2] << 16 | b[3] << 24);
}

int BinaryReader::Read7BitEncodedInt()
{
    int count = 0;
    int shift = 0;
    uint8_t b;
    do {
        assert(shift != 5 * 7);
        b = ReadByte();
        count |= (b & 0x7F) << shift;
        shift += 7;
    } while ((b & 0x80) != 0);
    return count;
}

}
