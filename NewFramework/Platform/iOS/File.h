#ifndef IOS_FILE_H
#define IOS_FILE_H
#include "NewFramework/IO/File/BaseFileIO.h"

class CBaseFileIO::CFile : public IFile
{
public:
    void Close(eWriteSyncBehaviour writeSyncBehaviour) override;
    bool Open(const std::string& path, eFileOpenMode openMode, std::string& error) override;

    size_t ReadBytes(uint8_t* const data, const size_t size) override;
    bool ReadString(std::string& data) override;
    size_t WriteBytes(uint8_t* const input, const size_t size, eWriteSyncBehaviour writeSyncBehaviour) override;
    bool WriteString(const std::string& input, eWriteSyncBehaviour writeSyncBehaviour) override;

    size_t GetFileLength() override;
    bool SanityCheck(std::string& errorString, eFileOpenMode expectedOpenMode);
    void SyncToDisk();
    int UpdateFileLength();
private:
    SFileIOResult ReadBytesWithResult(uint8_t* const data, const size_t size) override;
    SFileIOResult ReadStringWithResult(std::string& data) override;
    SFileIOResult WriteBytesWithResult(uint8_t* const input, const size_t size, eWriteSyncBehaviour writeSyncBehaviour) override;
    SFileIOResult WriteStringWithResult(const std::string& input, eWriteSyncBehaviour writeSyncBehaviour) override;

    SFileIOResult ReadBytes_Internal(uint8_t* const data, const size_t size);
};

#endif // IOS_FILE_H
