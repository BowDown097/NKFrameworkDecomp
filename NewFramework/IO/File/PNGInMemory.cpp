#include "PNGInMemory.h"

#include <cstring>

#define CREATE_EMPTY_RESULT_METHOD(head) \
    IFile::SFileIOResult head \
    { \
        SFileIOResult result; \
        return result; \
    }

CREATE_EMPTY_RESULT_METHOD(PNG_In_Memory::ReadStringWithResult(std::string&))
CREATE_EMPTY_RESULT_METHOD(PNG_In_Memory::ReadBytesWithResult(uint8_t* const, const size_t))
CREATE_EMPTY_RESULT_METHOD(PNG_In_Memory::WriteStringWithResult(const std::string&, eWriteSyncBehaviour))
CREATE_EMPTY_RESULT_METHOD(PNG_In_Memory::WriteBytesWithResult(uint8_t* const, const size_t, eWriteSyncBehaviour))

// path is used to hold the PNG's data instead of creating an entirely
// new member for it. Kind of clever I suppose, but confusing at first.
size_t PNG_In_Memory::ReadBytes(uint8_t* const result, const size_t resultSz)
{
    std::memcpy(result, path.data() + offset, resultSz);
    offset += resultSz;
    return resultSz;
}

// The disassembled function does nothing and just returns 0,
// but I believe it has been inlined into load_png_memory.
size_t PNG_In_Memory::WriteBytes(uint8_t* const input, const size_t inputSz, eWriteSyncBehaviour)
{
    path = std::string(reinterpret_cast<char*>(input), inputSz);
    return inputSz;
}
