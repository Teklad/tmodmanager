#include "binaryreader.h"
#include <stdio.h>
#include <assert.h>
namespace TMM {
    
BinaryReader::BinaryReader(FILE *file)
{
    m_file = file;
    assert(m_file != nullptr);
}

BinaryReader::~BinaryReader()
{
}

std::vector<uint8_t> BinaryReader::ReadBytes(int count)
{
    std::vector<uint8_t> result(count, 0);
    int total = fread(&result[0], 1, count, m_file);
    assert(total == count);
    return result;
}

void BinaryReader::SetPosition(int count)
{
    fseek(m_file, count, SEEK_SET);
}

void BinaryReader::SkipBytes(int count)
{
    fseek(m_file, count, SEEK_CUR);
}

long int BinaryReader::GetPosition()
{
    return ftell(m_file);
}

std::string BinaryReader::ReadString()
{
    int stringLength = Read7BitEncodedInt();
    if (stringLength <= 0) {
        return std::string();
    }
    char buffer[stringLength+1];
    fread(buffer, 1, stringLength, m_file);
    buffer[stringLength] = 0;
    return std::string(buffer);
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
    return (int)(b[0] | b[1] << 8 | b[2] << 16 | b[3] << 24);
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
