#ifndef __DATACACHE_H__
#define __DATACACHE_H__

#include <QFile>
#include <QString>
#include <QVector>

class DataCache {
    public:
        static DataCache& GetInstance();

        QString GetData(const QByteArray &sha1, const QByteArray &data);
        QString GetData(const QByteArray &sha1);
        QFile GetFileData(const QString &fileName);
    private:
        DataCache();
        ~DataCache() = default;

        DataCache(const DataCache&) = delete;
        DataCache& operator=(const DataCache&) = delete;
        DataCache(DataCache&&) = delete;
        DataCache& operator=(DataCache&&) = delete;

        QVector<QString> m_availHashes;
        QString m_dataDir;
};
#endif
