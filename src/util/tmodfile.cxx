#include "tmodfile.h"

#include "datacache.h"

#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <QDebug>

namespace TMM {


TmodFile::TmodFile(const QString &path)
{
    m_path = path;
}

TmodFile::~TmodFile()
{
}

/**
 * @brief Gets the requested property from the internal property class and returns it as a string
 * @param property
 * @return string
 */
QString TmodFile::GetProperty(Prop p)
{
    QString propValue;
    switch(p) {
        case Prop::dllReferences:
            for (const auto& i : m_properties.dllReferences) {
                propValue.append(i).push_back(',');
            }
            break;
        case Prop::modReferences:
            for (const auto& i : m_properties.modReferences) {
                propValue.append(i).push_back(',');
            }
            break;
        case Prop::weakReferences:
            for (const auto& i : m_properties.weakReferences) {
                propValue.append(i).push_back(',');
            }
            break;
        case Prop::sortAfter:
            for (const auto& i : m_properties.sortAfter) {
                propValue.append(i).push_back(',');
            }
            break;
        case Prop::sortBefore:
            for (const auto& i : m_properties.sortBefore) {
                propValue.append(i).push_back(',');
            }
            break;
        case Prop::side:
            switch (m_properties.side) {
                case 0: return "Both";
                case 1: return "Client";
                case 2: return "Server";
                case 3: return "NoSync";
            }
            break;
        case Prop::author:
            return m_properties.author;
        case Prop::version:
            return m_properties.version;
        case Prop::displayName:
            return m_properties.displayName;
        case Prop::homepage:
            return m_properties.homepage;
        case Prop::description:
            return m_properties.description;
        case Prop::noCompile:
            return (m_properties.noCompile ? "true" : "false");
        case Prop::hideCode:
            return (m_properties.hideCode ? "true" : "false");
        case Prop::hideResources:
            return (m_properties.hideResources ? "true" : "false");
        case Prop::includeSource:
            return (m_properties.includeSource ? "true" : "false");
        case Prop::includePDB:
            return (m_properties.includePDB ? "true" : "false");
        case Prop::editAndContinue:
            return (m_properties.editAndContinue ? "true" : "false");
    }
    return propValue;
}

/**
 * @brief Decompresses the file's internal data and retrieves the
 *        specified data for the file name given.
 * @param fileName The name of the file to retrieve.
 * @return std::vector<uint8_t>
 * Notes:
 *     Buffer will return empty if any portion of this process fails.
 */
QByteArray TmodFile::GetFileData(const QString &fileName)
{
    if (m_files.contains(fileName)) {
        return {};
    }
    BinaryReader reader(DataCache::GetInstance().GetData(m_hash));
    if (!reader.IsValid()) {
        return {};
    }
    reader.SetPosition(m_files.value(fileName));
    return reader.ReadBytes(reader.ReadInt32());
}

/**
 * @brief Continuously reads from BinaryReader into a vector of strings until a 0 read.
 * @param reader
 * @return QVector<QString>
 */
QVector<QString> TmodFile::ReadList(TMM::BinaryReader &reader)
{
    QVector<QString> li;
    for (auto item = reader.ReadString(); item.length() > 0; item = reader.ReadString()) {
        li.push_back(item);
    }
    return li;
}

/**
 * @brief Fills the property information for the tmod file.
 * @param reader
 * Notes:
 *     This will probably get moved to its own BuildProperties file in the future if I get time.
 */
void TmodFile::FillProperties(TMM::BinaryReader &reader)
{
    for (auto tag = reader.ReadString(); tag.length() > 0; tag = reader.ReadString()) {
        if (tag == "dllReferences") {
            m_properties.dllReferences = ReadList(reader);
        }
        //TODO: tModLoader handles modReferences a bit differently.
        // Should probably try to conform just to be safe later.
        if (tag == "modReferences") {
            m_properties.modReferences = ReadList(reader);
        }
        if (tag == "weakReferences") {
            m_properties.weakReferences = ReadList(reader);
        }
        if (tag == "sortAfter") {
            m_properties.sortAfter = ReadList(reader);
        }
        if (tag == "sortBefore") {
            m_properties.sortBefore = ReadList(reader);
        }
        if (tag == "author") {
            m_properties.author = reader.ReadString();
        }
        if (tag == "version") {
            m_properties.version = reader.ReadString();
        }
        if (tag == "displayName") {
            m_properties.displayName = reader.ReadString();
        }
        if (tag == "homepage") {
            m_properties.homepage = reader.ReadString();
        }
        if (tag == "description") {
            m_properties.description = reader.ReadString();
        }
        if (tag == "noCompile") {
            m_properties.noCompile = true;
        }
        if (tag == "!hideCode") {
            m_properties.hideCode = false;
        }
        if (tag == "!hideResources") {
            m_properties.hideResources = false;
        }
        if (tag == "includeSource") {
            m_properties.includeSource = true;
        }
        if (tag == "includePDB") {
            m_properties.includePDB = true;
        }
        if (tag == "editAndContinue") {
            m_properties.editAndContinue = true;
        }
        if (tag == "side") {
            m_properties.side = reader.ReadByte();
        }
    }
}

/**
 * @brief Reads the mod file data into the class structure so we can
 *        display the information to the user.
 * @return bool
 */
bool TmodFile::Read()
{
    BinaryReader reader(m_path);
    try {
        if (!reader.IsValid()) {
            throw "Reader state";
        }
        QByteArray header = reader.ReadBytes(4);
        if(!std::equal(header.begin(), header.end(), "TMOD")) {
            throw "Malformed header";
        }
        m_tModLoaderVersion = reader.ReadString();
        m_hash = reader.ReadBytes(20);
        m_signature = reader.ReadBytes(256);
        m_dataLoc = reader.GetPosition();
        int datalen = reader.ReadInt32();
        QByteArray data = reader.ReadBytes(datalen);
        // Verify data integrity of the mod.
        CryptoPP::SHA1 sha1;
        QByteArray computedHash(20, 0);
        CryptoPP::ArraySource((uint8_t*)data.data(), data.size(), true,
            new CryptoPP::HashFilter(sha1,
                new CryptoPP::ArraySink(reinterpret_cast<uint8_t*>(computedHash.data()), 20)
            )
        );
        if (m_hash != computedHash) {
            throw "SHA1 mismatch";
        }

        reader.SetFile(DataCache::GetInstance().GetData(m_hash.toHex(), data));
        if (!reader.IsValid()) {
            throw "Reader state";
        }
        m_name = reader.ReadString();
        m_version = reader.ReadString();
        int count = reader.ReadInt32();
        for (int i = 0; i < count; i++) {
            QString fileName = reader.ReadString();
            qint64 fileDataLoc = reader.GetPosition();
            int fileLength = reader.ReadInt32();
            if (fileName == "Info") {
                FillProperties(reader);
            }else {
                reader.SkipBytes(fileLength);
            }
            m_files.insert(std::move(fileName), std::move(fileDataLoc));
        }
    } catch (const char* e) {
        return false;
    }
    return true;
}

QString TmodFile::GetName()
{
    return m_name;
}

QString TmodFile::GetVersion()
{
    return m_version;
}

QStringList TmodFile::ListFiles()
{
    return m_files.keys();
}

}
