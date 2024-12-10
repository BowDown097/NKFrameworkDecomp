#pragma once
#include <cstdint>
#include <string>

class IFile;

namespace HashHelper {
    std::string BufferToHexString(uint8_t const* buffer, unsigned int size);
    int64_t HexStringToValue(const std::string& str);
    std::string MD5String(const std::string& str);
    std::string MD5Data(uint8_t* data, size_t size);
    std::string MD5File(IFile* file);
}
