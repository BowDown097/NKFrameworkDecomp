#pragma once
#include <string>

enum class NKErrorType { VALUE0, VALUE1, VALUE2, VALUE3, VALUE4, VALUE5, VALUE6, VALUE7 };

struct NKError
{
    NKErrorType type{}; // 0x00
    std::string typeStr; // 0x08
    std::string reason; // 0x20
    std::string fix; // 0x38
    int field_50 = -1; // 0x50

    NKError();
    NKError(const NKErrorType& type, const std::string& reason,
            const std::string& typeStr, const std::string& fix, int field_50)
        : type(type), typeStr(typeStr), reason(reason), fix(fix), field_50(field_50) {}

    bool IsValid() const
    { return static_cast<int>(type) > 0 || !reason.empty() || !typeStr.empty() || !fix.empty(); }
};
