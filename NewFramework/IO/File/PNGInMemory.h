#ifndef PNGINMEMORY_H
#define PNGINMEMORY_H
#include "IFile.h"

struct PNG_In_Memory : IFile
{
    std::string GetFilePath() override { static std::string emptyStr = ""; return emptyStr; }
    size_t GetFileLength() override { return 0; }
    void SetOffset(size_t offset) override { this->offset = offset; }
    size_t GetOffset() override { return offset; }
    bool ReadString(std::string&) override { return false; }
    SFileIOResult ReadStringWithResult(std::string&) override;
    size_t ReadBytes(uint8_t* const result, const size_t resultSz) override;
    SFileIOResult ReadBytesWithResult(uint8_t* const, const size_t) override;
    bool WriteString(const std::string&, eWriteSyncBehaviour) override { return false; }
    SFileIOResult WriteStringWithResult(const std::string&, eWriteSyncBehaviour) override;
    size_t WriteBytes(uint8_t* const input, const size_t inputSz, eWriteSyncBehaviour) override;
    SFileIOResult WriteBytesWithResult(uint8_t* const, const size_t, eWriteSyncBehaviour) override;
};

#endif // PNGINMEMORY_H
