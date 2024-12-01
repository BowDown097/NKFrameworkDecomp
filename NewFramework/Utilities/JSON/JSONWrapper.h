#pragma once
#include "NewFramework/IO/File/BaseFileIO.h"
#include <json/json.h>
#include <json_spirit/json_spirit.h>

enum class eEncryptionType { None, StephenEncryption };

class CJSONWrapper {
public:
    CJSONWrapper(CBaseFileIO* fileIO) : fileIO(fileIO) {}
    template<typename T> bool Write(
        T obj, std::string filePath, eEncryptionType encryption, IFile::eWriteSyncBehaviour writeSyncBehaviour);
    template<typename T> IFile::SFileIOResult WriteWithResult(
        T obj, std::string filePath, eEncryptionType encryption, IFile::eWriteSyncBehaviour writeSyncBehaviour);
    json_spirit::mObject ReadObject(std::string filePath, const CFilePolicy& filePolicy);
    json_spirit::mValue ReadValue(std::string filePath, const CFilePolicy& filePolicy);
    json_spirit::mObject ReadObject(std::string filePath, bool isDocument);
    json_spirit::mValue ReadValue(std::string filePath, bool isDocument);
    json_spirit::mArray ReadArray(std::string filePath, const CFilePolicy& filePolicy);
    json_spirit::mArray ReadArray(std::string filePath, bool isDocument);
    bool ParseJSONData(uint8_t* data, size_t size, json_spirit::mValue& out, bool noErrorOnFail);
    std::pair<std::string, bool> DecryptIfEncrypted(uint8_t* data, size_t size);
    bool ParseUnencryptedJSONData(const std::string& data, json_spirit::mValue& out, bool noErrorOnFail);
    bool ParseJSONData(uint8_t* data, size_t size, Json::Value& out, bool noErrorOnFail);
private:
    CBaseFileIO* fileIO;
    bool field_8{};
};
