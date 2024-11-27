#pragma once
#include <string>

enum class NKErrorType { VALUE0, VALUE1, VALUE2, VALUE3, VALUE4, VALUE5, VALUE6, VALUE7, VALUE8, VALUE9 };

struct NKError
{
    NKErrorType type{}; // 0x00
    std::string typeStr; // 0x08
    std::string reason; // 0x20
    std::string fix; // 0x38
    int code = -1; // 0x50

    NKError() = default;
    NKError(const NKErrorType& type, const std::string& reason,
            const std::string& typeStr, const std::string& fix, int field_50 = -1)
        : type(type), typeStr(typeStr), reason(reason), fix(fix), code(field_50) {}

    bool IsValid() const;
};
