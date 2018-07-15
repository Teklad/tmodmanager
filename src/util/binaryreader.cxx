#include <assert.h>

#include "binaryreader.h"

namespace TMM {

BinaryReader::BinaryReader(const QString &file)
{
    SetFile(file);
}

bool BinaryReader::SetFile(const QString &file)
{
    if (m_file != nullptr && m_file->isOpen()) {
        m_file->close();
        delete m_file;
    }
    m_file = new QFile(file);
    if (m_file->open(QFile::ReadOnly)) {
        m_stream.setDevice(m_file);
    }
    return m_file->isOpen();
}

bool BinaryReader::IsValid()
{
    return m_file->isOpen();
}

QByteArray BinaryReader::ReadBytes(qint64 count)
{
    QByteArray bits = m_file->read(count);
    return bits;
}

QString BinaryReader::ReadString()
{
    int len = Read7BitEncodedInt();
    if (len <= 0) {
        return {};
    }
    return m_file->read(len);
}

uint8_t BinaryReader::ReadByte()
{
    uint8_t b;
    m_stream >> b;
    return b;
}

int BinaryReader::ReadInt32()
{
    uint8_t bits[4];
    m_stream >> bits[0] >> bits[1] >> bits[2] >> bits[3];
    return reinterpret_cast<int>(bits[0] | bits[1] << 8 | bits[2] << 16 | bits[3] << 24);
}

int BinaryReader::Read7BitEncodedInt()
{
    int count = 0;
    int shift = 0;
    uint8_t b;
    do {
        if (shift == 5 * 7) {
            throw "Format_Bad7BitInt32";
        }
        b = ReadByte();
        count |= (b & 0x7F) << shift;
        shift += 7;
    } while ((b & 0x80) != 0);
    return count;
}

}
