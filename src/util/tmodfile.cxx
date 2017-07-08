#include "tmodfile.h"
#include "binaryreader.h"

#include <openssl/sha.h>
#include <zlib.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <assert.h>

namespace TMM {

/**
 * @brief Helper function uses zlib to decompress a data stream using
 *        the DEFLATE algorithm.
 * @param in The input data
 * @param out Output buffer
 * @return int
 * Error codes are as follows:
 *     0  - No error
 *     -1 - Inflate data error
 */
static inline int decompress(std::vector<uint8_t>& in, std::vector<uint8_t> &out)
{
    const size_t BUFSIZE = 1024;
    uint8_t temp_buffer[BUFSIZE];
    z_stream strm;
    strm.zalloc = 0;
    strm.zfree = 0;
    strm.opaque = 0;
    strm.next_in = &in[0];
    strm.avail_in = in.size();
    strm.next_out = temp_buffer;
    strm.avail_out = BUFSIZE;
    
    inflateInit2(&strm, -15);
    while (strm.avail_in != 0) {
        int res = inflate(&strm, Z_NO_FLUSH);
        if (res != Z_OK && res != Z_STREAM_END) {
            std::cout << res << '\n';
            return -1;
        }
        if (strm.avail_out == 0) {
            out.insert(out.end(), temp_buffer, temp_buffer + BUFSIZE);
            strm.next_out = temp_buffer;
            strm.avail_out = BUFSIZE;
        }
    }
    out.insert(out.end(), temp_buffer, temp_buffer + BUFSIZE - strm.avail_out);
    inflateEnd(&strm);
    return 0;
}

TmodFile::TmodFile(const std::string &path)
{
    m_path = path;
}

TmodFile::~TmodFile()
{
    
}

/**
 * @brief Decompresses the file's internal data and retrieves the
 *        specified data for the file name given.
 * @param fileName The name of the file to retrieve.
 * @return std::vector<uint8_t>
 * Notes:
 *     Buffer will return empty if any portion of this process fails.
 */
std::vector<uint8_t> TmodFile::GetFileData(const std::string &fileName)
{
    if (m_files.count(fileName) == 0) {
        return std::vector<uint8_t>();
    }
    
    FILE *modFile = fopen(m_path.c_str(), "rb");
    if (modFile == nullptr) {
        return std::vector<uint8_t>();
    }
    
    BinaryReader *reader = new BinaryReader(modFile);
    reader->SetPosition(m_dataLoc);
    std::vector<uint8_t> tempData = reader->ReadBytes(reader->ReadInt32());
    std::vector<uint8_t> inflatedData;
    if (decompress(tempData, inflatedData) != 0) {
        delete reader;
        fclose(modFile);
        return std::vector<uint8_t>();
    }
    delete reader;
    fclose(modFile);
    modFile = fmemopen(&inflatedData[0], inflatedData.size(), "rb");
    if (modFile == nullptr) {
        return std::vector<uint8_t>();
    }
    reader = new BinaryReader(modFile);
    reader->SetPosition(m_files.at(fileName));
    std::vector<uint8_t> fileData = reader->ReadBytes(reader->ReadInt32());
    delete reader;
    fclose(modFile);
    return fileData;
}

/**
 * @brief Continuously reads from BinaryReader into a vector of strings until a 0 read.
 * @param reader
 * @return std::vector<std::string>
 */
std::vector<std::string> TmodFile::ReadList(TMM::BinaryReader *reader)
{
    std::vector<std::string> li;
    std::string item;
    for (item = reader->ReadString(); item.length() > 0; item = reader->ReadString()) {
        li.push_back(item);
    }
    return li;
}

/**
 * @brief Fills the property information for the tmod file.
 * @param reader
 */
void TmodFile::FillProperties(TMM::BinaryReader *reader)
{
    for (std::string tag = reader->ReadString(); tag.length() > 0; tag = reader->ReadString()) {
        if (tag == "dllReferences") {
            m_properties.dllReferences = ReadList(reader);
        }
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
            m_properties.author = reader->ReadString();
        }
        if (tag == "version") {
            m_properties.version = reader->ReadString();
        }
        if (tag == "displayName") {
            m_properties.displayName = reader->ReadString();
        }
        if (tag == "homepage") {
            m_properties.homepage = reader->ReadString();
        }
        if (tag == "description") {
            m_properties.description = reader->ReadString();
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
            m_properties.side = reader->ReadByte();
        }
    }
}

/**
 * @brief Reads the mod file data into the class structure so we can
 *        display the information to the user.
 * @return int
 * Return codes are as follows:
 *      0 - no error
 *     -1 - File open error
 *     -2 - TMOD Header missing
 *     -3 - Hash mismatch
 *     -4 - Data decompression error
 *     -5 - Memory stream open error
 */
int TmodFile::Read()
{
    FILE *modFile = fopen(m_path.c_str(), "rb");
    if (modFile == nullptr) {
        return -1;
    }
    BinaryReader *reader = new BinaryReader(modFile);
    if(memcmp(reader->ReadBytes(4).data(), "TMOD", 4) != 0) {
        fclose(modFile);
        delete reader;
        return -2;
    }
    m_tModLoaderVersion = reader->ReadString();
    memcpy(m_hash, reader->ReadBytes(20).data(), 20);
    memcpy(m_signature, reader->ReadBytes(256).data(), 256);
    m_dataLoc = reader->GetPosition();
    std::vector<uint8_t> data;
    long unsigned int dataSize = reader->ReadInt32();
    data = reader->ReadBytes(dataSize);
    delete reader;
    fclose(modFile);
    
    // Verify data integrity of the mod.
    uint8_t verifyHash[20];
    SHA1(&data[0], dataSize, verifyHash);
    if (memcmp(m_hash, verifyHash, 20) != 0) {
        return -3;
    }
    
    // Decompress the mod data for reading.
    std::vector<uint8_t> inflatedData;
    if (decompress(data, inflatedData) != 0) {
        return -4;
    }
    modFile = fmemopen(&inflatedData[0], inflatedData.size(), "rb");
    if (modFile == nullptr) {
        return -5;
    }
    reader = new BinaryReader(modFile);
    m_name = reader->ReadString();
    m_version = reader->ReadString();
    int fileCount = reader->ReadInt32();
    std::string fileName;
    long unsigned int fileDataLoc;

    for (int i = 0; i < fileCount; ++i) {
        fileName = reader->ReadString();
        fileDataLoc = reader->GetPosition();
        if (fileName == "Info") {
            reader->ReadInt32();
            FillProperties(reader);
        }else {
            reader->SkipBytes(reader->ReadInt32());
        }
        m_files.emplace(std::make_pair(std::move(fileName), std::move(fileDataLoc)));
    }
    
    delete reader;
    fclose(modFile);
    return 0;
}

const std::string &TmodFile::GetName()
{
    return m_name;
}

const std::string &TmodFile::GetVersion()
{
    return m_version;
}

}
