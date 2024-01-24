#ifndef IFILE_H
#define IFILE_H
#include <cstdint>
#include <memory>
#include <string>

enum class eFileOpenMode { Write, Read, ReadWriteNew, ReadWriteExisting, Append };

class IFile
{
public:
    enum class eWriteSyncBehaviour { Blablabla, Blablabla2 };

    virtual ~IFile() = 0;

    virtual void Close(eWriteSyncBehaviour writeSyncBehaviour);
    virtual bool Open(const std::string& path, eFileOpenMode openMode, std::string& error);

    virtual uint64_t ReadBytes(uint8_t* const result, const size_t resultSz);
    virtual bool ReadString(std::string& result);
    virtual uint64_t WriteBytes(uint8_t* const input, const size_t inputSz,
                                IFile::eWriteSyncBehaviour writeSyncBehaviour);
    virtual bool WriteString(const std::string& input, IFile::eWriteSyncBehaviour writeSyncBehaviour);

    virtual uint64_t GetFileLength() { return length; }
    virtual std::string GetFilePath() { return path; }
    virtual uint64_t GetOffset() { return offset; }
    virtual void SetOffset(uint64_t offset) { this->offset = offset; }
protected:
    struct SFileIOResult
    {
        size_t end = 0;
        char* data;
        size_t read = 0;
    };

    virtual SFileIOResult ReadBytesWithResult(uint8_t* const result, const size_t resultSz);
    virtual SFileIOResult ReadStringWithResult(std::string& result);
    virtual SFileIOResult WriteBytesWithResult(uint8_t* const input, const size_t inputSz,
                                               IFile::eWriteSyncBehaviour writeSyncBehaviour);
    virtual SFileIOResult WriteStringWithResult(const std::string& input, IFile::eWriteSyncBehaviour writeSyncBehaviour);

    std::shared_ptr<FILE> filePtr;
    uint64_t length = 0;
    uint64_t offset = 0;
    eFileOpenMode openMode;
    std::string path;
};

#endif // IFILE_H
