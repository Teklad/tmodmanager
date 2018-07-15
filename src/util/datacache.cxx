#include "datacache.h"

#include <QDir>
#include <QStandardPaths>

#include <cryptopp/files.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/zinflate.h>

#include <iostream>

DataCache::DataCache()
{
    constexpr char cacheName[] = "tmodmanager";
    QString dataLoc = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QDir dataDir(dataLoc);
    if (!dataDir.exists(cacheName)) {
        dataDir.mkdir(cacheName);
    }
    dataDir.cd(cacheName);
    m_dataDir = dataDir.absolutePath();
    for (QString entry : dataDir.entryList()) {
        m_availHashes.push_back(entry);
    }
}


DataCache &DataCache::GetInstance()
{
    static DataCache instance;
    return instance;
}

QString DataCache::GetData(const QByteArray &sha1, const QByteArray &data)
{
    QDir dataDir(m_dataDir);
    if (dataDir.exists(sha1)) {
        return dataDir.filePath(sha1);
    }
    CryptoPP::Inflator inflator;
    inflator.Attach(new CryptoPP::FileSink(dataDir.filePath(sha1).toLatin1().data(), true));
    inflator.Put2(reinterpret_cast<const uint8_t*>(data.data()),data.size(), 0, true);
    inflator.MessageEnd();
    return dataDir.filePath(sha1);
}

QString DataCache::GetData(const QByteArray &sha1)
{
    QDir dataDir(m_dataDir);
    return dataDir.filePath(sha1);
}
