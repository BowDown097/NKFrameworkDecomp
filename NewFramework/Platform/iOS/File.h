#pragma once
#include "NewFramework/IO/File/BaseFileIO.h"

class CBaseFileIO::CFile : public IFile
{
public:
    ~CFile() override;
    bool Open(const std::string& path, eFileOpenMode openMode, std::string& error);
    int UpdateFileLength();
    void Close(eWriteSyncBehaviour writeSyncBehaviour) override;
    void SyncToDisk();
    bool SanityCheck(std::string& errorString, eFileOpenMode expectedOpenMode);
    bool ReadString(std::string& data) override;
    SFileIOResult ReadStringWithResult(std::string& data) override;
    SFileIOResult ReadBytes_Internal(uint8_t* const data, const size_t size);
    size_t ReadBytes(uint8_t* const data, const size_t size) override;
    SFileIOResult ReadBytesWithResult(uint8_t* const data, const size_t size) override;
    bool WriteString(const std::string& input, eWriteSyncBehaviour writeSyncBehaviour) override;
    SFileIOResult WriteStringWithResult(const std::string& input, eWriteSyncBehaviour writeSyncBehaviour) override;
    size_t WriteBytes(uint8_t* const input, const size_t size, eWriteSyncBehaviour writeSyncBehaviour) override;
    SFileIOResult WriteBytesWithResult(uint8_t* const input, const size_t size, eWriteSyncBehaviour writeSyncBehaviour) override;
    std::string GetFilePath() override { return path; }
    void SetOffset(size_t offset) override { this->offset = offset; }
    size_t GetOffset() override { return offset; }
    size_t GetFileLength() override;
};
