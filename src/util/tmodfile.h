#ifndef  _TMMUTIL_TMODFILE_H_
#define  _TMMUTIL_TMODFILE_H_
#include "binaryreader.h"

#include <QVector>
#include <QString>
#include <QHash>

namespace TMM {
/**
 * @brief This class reads and stores tmod file information for access in the UI.
 *        Most of the functions in here have been borrowed from their C# counterparts from
 *        tModLoader.
 */
class TmodFile {
    public:
        enum class Prop {
            dllReferences,
            modReferences,
            weakReferences,
            sortAfter,
            sortBefore,
            author,
            version,
            displayName,
            homepage,
            description,
            noCompile,
            hideCode,
            hideResources,
            includeSource,
            includePDB,
            editAndContinue,
            side
        };
        TmodFile(const QString &path);
        ~TmodFile();
        bool Read();
        QByteArray GetFileData(const QString &fileName);
        QStringList ListFiles();
        QString GetName();
        QString GetVersion();
        QString GetProperty(Prop p);
    private:
        struct Properties {
            QVector<QString> dllReferences;
            QVector<QString> modReferences;
            QVector<QString> weakReferences;
            QVector<QString> sortAfter;
            QVector<QString> sortBefore;
            QString author;
            QString version;
            QString displayName;
            QString homepage;
            QString description;
            bool noCompile = false;
            bool hideCode = true;
            bool hideResources = true;
            bool includeSource = false;
            bool includePDB = false;
            bool editAndContinue = false;
            int side;
        };
        void FillProperties(BinaryReader &reader);
        QVector<QString> ReadList(BinaryReader &reader);
        QString m_name;
        QString m_version;
        QString m_path;
        QString m_tModLoaderVersion;
        QByteArray m_hash;
        QByteArray m_signature;
        QByteArray m_dataCRC;
        long int m_dataLoc;

        QHash<QString, qint64> m_files;
        Properties m_properties;
};
}

#endif //_TMMUTIL_TMODFILE_H_
