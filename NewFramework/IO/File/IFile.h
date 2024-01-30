#ifndef IFILE_H
#define IFILE_H
#include "FilePlatformData.h"
#include <boost/smart_ptr/shared_ptr.hpp>

enum class eFileOpenMode { Write, Read, ReadWriteNew, ReadWriteExisting, Append };

class IFile
{
public:
    enum class eWriteSyncBehaviour { Sync, NoSync };

    enum class eFileIOStatus
    {
        Success = 0,
        Failed = 1,
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
        int field_4; // 0x04
        size_t bytesTransferred = 0; // 0x08
        size_t bytesAttempted = 0; // 0x10
        std::string info; // 0x18
    };

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
    void* field_0; // 0x00
    std::string path; // 0x08
    size_t length = 0; // 0x20
    size_t offset = 0; // 0x28
    eFileOpenMode openMode; // 0x30
    int field_34; // 0x34
    boost::shared_ptr<CFilePlatformData> platformData; // 0x38
};

#endif // IFILE_H
