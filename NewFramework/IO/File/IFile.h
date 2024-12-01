#pragma once

#include "FilePlatformData.h"

#include <boost/make_shared.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

#include <string>

enum class eFileOpenMode { Write, Read, ReadWriteNew, ReadWriteExisting, Append };

class IFile
{
public:
    enum class eWriteSyncBehaviour { Sync, NoSync };

    enum class eFileIOStatus
    {
        Success = 0,
        Failed = 1,
        OpenFailed = 2,
        SanityCheckFailed = 3,
        IOFailed = 5,
        IOIncomplete = 6,
        NullWriteBuffer = 9,
        ZeroLengthWriteBuffer = 10,
        NullReadBuffer = 13,
        ZeroLengthReadBuffer = 14
    };

    struct SFileIOResult
    {
        eFileIOStatus status = eFileIOStatus::Failed; // 0x00
        size_t bytesTransferred{}; // 0x08
        size_t bytesAttempted{}; // 0x10
        std::string info; // 0x18
    };

    IFile() : platformData(boost::make_shared<CFilePlatformData>()) {}
    virtual ~IFile();
    virtual void Close(eWriteSyncBehaviour writeSyncBehaviour);
    virtual std::string GetFilePath();
    virtual size_t GetFileLength();
    virtual void SetOffset(size_t offset);
    virtual size_t GetOffset();
    virtual bool ReadString(std::string& result);
    virtual SFileIOResult ReadStringWithResult(std::string& data);
    virtual size_t ReadBytes(uint8_t* const result, const size_t resultSz);
    virtual SFileIOResult ReadBytesWithResult(uint8_t* const data, const size_t size);
    virtual bool WriteString(const std::string& input, eWriteSyncBehaviour writeSyncBehaviour);
    virtual SFileIOResult WriteStringWithResult(const std::string& input, eWriteSyncBehaviour writeSyncBehaviour);
    virtual size_t WriteBytes(uint8_t* const input, const size_t inputSz, eWriteSyncBehaviour writeSyncBehaviour);
    virtual SFileIOResult WriteBytesWithResult(uint8_t* const input, const size_t size, eWriteSyncBehaviour writeSyncBehaviour);
protected:
    std::string path; // 0x08
    size_t length{}; // 0x20
    size_t offset{}; // 0x28
    eFileOpenMode openMode{}; // 0x30
    boost::shared_ptr<CFilePlatformData> platformData; // 0x38
};
