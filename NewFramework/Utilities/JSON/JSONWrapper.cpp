#include "JSONWrapper.h"
#include "NewFramework/Platform/Shared/Assert.h"
#include "NewFramework/Platform/Shared/Logging.h"
#include "NewFramework/Utilities/StringHelper.h"
#include "NewFramework/Utilities/Utilities.h"
#include "Uncategorized/StephenEncryption.h"

template<typename T> IFile::SFileIOResult CJSONWrapper::WriteWithResult(
    T obj, std::string filePath, eEncryptionType encryption, IFile::eWriteSyncBehaviour writeSyncBehaviour) {
    IFile::SFileIOResult result;
    result.status = IFile::eFileIOStatus::Success;
    IFile* file = fileIO->OpenFile(filePath, fileIO->documentPolicy, eFileOpenMode::ReadWriteNew);
    if (!file) {
        result.info = StringHelper::Format("Failed to open file (%s) for writing", filePath.c_str());
        result.status = IFile::eFileIOStatus::OpenFailed;
        return result;
    }
    std::string objData = json_spirit::write(obj);
    if (encryption == eEncryptionType::StephenEncryption) {
        std::string dgdataHeader = StringHelper::Format("DGDATA%08x", GetCRCFromData(objData.c_str(), objData.size()));
        CStephenEncryption stephenEncryption;
        stephenEncryption.EncryptStream(reinterpret_cast<uint8_t*>(objData.data()), objData.size());
        objData = dgdataHeader + objData;
        result = file->WriteBytesWithResult(reinterpret_cast<uint8_t*>(objData.data()), objData.size(), writeSyncBehaviour);
    } else {
        result = file->WriteStringWithResult(objData, writeSyncBehaviour);
    }
    file->Close(writeSyncBehaviour);
    delete file;
    return result;
}

template<typename T> bool CJSONWrapper::Write(
    T obj, std::string filePath, eEncryptionType encryption, IFile::eWriteSyncBehaviour writeSyncBehaviour) {
    IFile::SFileIOResult ioResult = WriteWithResult(obj, filePath, encryption, writeSyncBehaviour); ENFORCE_LINE(35);
    return NKAssert(ioResult.status == IFile::eFileIOStatus::Success, ioResult.info.c_str()); // assert on line 36
}

std::pair<std::string, bool> CJSONWrapper::DecryptIfEncrypted(uint8_t* data, size_t size) {
    if (size >= 14) {
        std::string header(reinterpret_cast<char*>(data), 0, 6);
        if (header == "DGDATA") {
            std::string crcStr(reinterpret_cast<char*>(data), 6, 8);
            uint32_t crc;
            sscanf(crcStr.c_str(), "%x", &crc);
            CStephenEncryption stephenEncryption;
            stephenEncryption.DecryptStream(data + 14, size - 14);
            if (GetCRCFromData(reinterpret_cast<char*>(data + 14), size - 14) == crc) {
                return std::make_pair(std::string(reinterpret_cast<char*>(data + 14), size - 14), true);
            } else {
                return std::make_pair(std::string(), false);
            }
        }
    }
    if (ensureEncrypted) {
        return std::make_pair(std::string(), false);
    }
    return std::make_pair(std::string(reinterpret_cast<char*>(data), size), true);
}

json_spirit::mObject CJSONWrapper::ReadObject(std::string filePath, const CFilePolicy& filePolicy) {
    json_spirit::mValue value = ReadValue(filePath, filePolicy);
    return value.get_obj();
}

json_spirit::mArray CJSONWrapper::ReadArray(std::string filePath, const CFilePolicy& filePolicy) {
    json_spirit::mValue value = ReadValue(filePath, filePolicy);
    return value.get_array();
}


































































































json_spirit::mValue CJSONWrapper::ReadValue(std::string filePath, const CFilePolicy& filePolicy) {
    if (!fileIO) {
        LOG_ERROR("Couldn't load file %s: No file manager has been set in CJSONWrapper instance.", filePath.c_str()); ENFORCE_LINE(170);
        return {};
    }
    IFile* file = fileIO->OpenFile(filePath, filePolicy, eFileOpenMode::Read);
    if (!file) {
        return {};
    }
    size_t fileLength = file->GetFileLength();
    uint8_t* fileBuf = new uint8_t[fileLength];
    file->ReadBytes(fileBuf, fileLength);
    file->Close(IFile::eWriteSyncBehaviour::Sync);
    delete file;
    json_spirit::mValue value;
    ParseJSONData(fileBuf, fileLength, value, false);
    delete[] fileBuf;
    return value;
}

json_spirit::mObject CJSONWrapper::ReadObject(std::string filePath, bool isDocument) {
    json_spirit::mValue value = ReadValue(filePath, isDocument);
    return value.get_obj();
}

json_spirit::mArray CJSONWrapper::ReadArray(std::string filePath, bool isDocument) {
    json_spirit::mValue value = ReadValue(filePath, isDocument);
    return value.get_array();
}


















json_spirit::mValue CJSONWrapper::ReadValue(std::string filePath, bool isDocument) {
    if (!fileIO) {
        LOG_ERROR("Couldn't load file %s: No file manager has been set in CJSONWrapper instance.", filePath.c_str()); ENFORCE_LINE(217);
        return {};
    }
    IFile* file = fileIO->OpenFile(filePath, isDocument ? fileIO->documentPolicy : fileIO->assetPolicy, eFileOpenMode::Read);
    if (!file) {
        return {};
    }
    size_t fileLength = file->GetFileLength();
    uint8_t* fileBuf = new uint8_t[fileLength];
    file->ReadBytes(fileBuf, fileLength);
    file->Close(IFile::eWriteSyncBehaviour::Sync);
    delete file;
    json_spirit::mValue value;
    ParseJSONData(fileBuf, fileLength, value, false);
    delete[] fileBuf;
    return value;
}


















































































bool CJSONWrapper::ParseJSONData(uint8_t* data, size_t size, json_spirit::mValue& out, bool noErrorOnFail) {
    std::pair<std::string, bool> result = DecryptIfEncrypted(data, size);
    std::string decryptedData = result.first;
    if (result.second) {
        return ParseUnencryptedJSONData(decryptedData, out, noErrorOnFail);
    } else if (noErrorOnFail) {
        return false;
    } else {
        LOG_ERROR("Error reading JSON object:\n %s", decryptedData.c_str()); ENFORCE_LINE(324);
        return false;
    }
}







bool CJSONWrapper::ParseUnencryptedJSONData(const std::string& data, json_spirit::mValue& out, bool noErrorOnFail) {
    bool success = json_spirit::read(data, out);
    if (!success && !noErrorOnFail) {
        LOG_ERROR("Error reading JSON object:\n %s", data.c_str()); ENFORCE_LINE(338);
    }
    return success;
}

bool CJSONWrapper::ParseJSONData(uint8_t* data, size_t size, Json::Value& out, bool noErrorOnFail) {
    std::pair<std::string, bool> result = DecryptIfEncrypted(data, size);
    std::string decryptedData = result.first;
    Json::Reader reader;

    if (result.second) {
        bool success = reader.parse(decryptedData, out);
        if (success || noErrorOnFail) {
            return success;
        }
    } else if (noErrorOnFail) {
        return false;
    }

    LOG_ERROR("Error reading JSON object:\n %s", decryptedData.c_str()); ENFORCE_LINE(357);
    return false;
}

template bool CJSONWrapper::Write<json_spirit::Object>(
    json_spirit::Object, std::string, eEncryptionType, IFile::eWriteSyncBehaviour);
template IFile::SFileIOResult CJSONWrapper::WriteWithResult<json_spirit::Object>(
    json_spirit::Object, std::string, eEncryptionType, IFile::eWriteSyncBehaviour);
template bool CJSONWrapper::Write<json_spirit::mObject>(
    json_spirit::mObject, std::string, eEncryptionType, IFile::eWriteSyncBehaviour);
template IFile::SFileIOResult CJSONWrapper::WriteWithResult<json_spirit::mObject>(
    json_spirit::mObject, std::string, eEncryptionType, IFile::eWriteSyncBehaviour);
template bool CJSONWrapper::Write<json_spirit::mValue>(
    json_spirit::mValue, std::string, eEncryptionType, IFile::eWriteSyncBehaviour);
template IFile::SFileIOResult CJSONWrapper::WriteWithResult<json_spirit::mValue>(
    json_spirit::mValue, std::string, eEncryptionType, IFile::eWriteSyncBehaviour);
