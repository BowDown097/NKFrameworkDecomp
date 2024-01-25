#ifndef IFILE_H
#define IFILE_H
#include <cstdint>
#include <memory>
#include <string>

enum class eFileOpenMode { Write, Read, ReadWriteNew, ReadWriteExisting, Append };

class IFile
{
public:
    enum class eWriteSyncBehaviour { Sync, NoSync };

    virtual ~IFile();

    virtual void Close(eWriteSyncBehaviour writeSyncBehaviour);
    virtual bool Open(const std::string& path, eFileOpenMode openMode, std::string& error);

    virtual size_t ReadBytes(uint8_t* const result, const size_t resultSz);
    virtual bool ReadString(std::string& result);
    virtual size_t WriteBytes(uint8_t* const input, const size_t inputSz, eWriteSyncBehaviour writeSyncBehaviour);
    virtual bool WriteString(const std::string& input, eWriteSyncBehaviour writeSyncBehaviour);

    virtual size_t GetFileLength() { return length; }
    virtual std::string GetFilePath() { return path; }
    virtual size_t GetOffset() { return offset; }
    virtual void SetOffset(size_t offset) { this->offset = offset; }
protected:
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
        eFileIOStatus status = eFileIOStatus::Failed;
        char gap1[4];
        size_t bytesTransferred = 0;
        size_t bytesAttempted = 0;
        std::string info;
    };

    virtual SFileIOResult ReadBytesWithResult(uint8_t* const data, const size_t size);
    virtual SFileIOResult ReadStringWithResult(std::string& data);
    virtual SFileIOResult WriteBytesWithResult(uint8_t* const input, const size_t size, eWriteSyncBehaviour writeSyncBehaviour);
    virtual SFileIOResult WriteStringWithResult(const std::string& input, eWriteSyncBehaviour writeSyncBehaviour);

    std::shared_ptr<FILE> filePtr;
    size_t length = 0;
    size_t offset = 0;
    eFileOpenMode openMode;
    std::string path;
};

#endif // IFILE_H
