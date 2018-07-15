#ifndef  _TMMUTIL_BINARYREADER_H_
#define  _TMMUTIL_BINARYREADER_H_

#include <QDataStream>
#include <QFile>

namespace TMM {
/**
 * @brief Partial reimplementation of C#'s BinaryReader class in order to properly read tmod
 *        file data.
 */
class BinaryReader {
    public:
        BinaryReader(const QString &name);
        bool SetFile(const QString &fpath);
        bool IsValid();
        qint64 GetPosition() {return m_file->pos();}
        bool SetPosition(qint64 pos) {return m_file->seek(pos);}
        bool SkipBytes(qint64 bytes) {return m_file->skip(bytes);}
        QByteArray ReadBytes(qint64 count);
        QString ReadString();
        uint8_t ReadByte();
        int ReadInt32();
    private:
        int Read7BitEncodedInt();
        QFile *m_file = nullptr;
        QDataStream m_stream;
};
}

#endif //_TMMUTIL_BINARYREADER_H_
