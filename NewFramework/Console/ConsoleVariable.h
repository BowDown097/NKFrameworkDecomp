#pragma once

#include <string>

struct ConsoleVariable {
    const char* sName; // 0x00
    const char* sDescription; // 0x08
    std::string sValue; // 0x10
    float fValue; // 0x28
    int iValue; // 0x2C
    ConsoleVariable* pHead; // 0x30

    ConsoleVariable(const char* sName, const char* sDescription, const char* sDefaultValue);
};

namespace cvar {
    void set(ConsoleVariable* pVariable, const std::string& sValue);
    ConsoleVariable* get_list();
    ConsoleVariable* get(const std::string& sName);
    void set_integer(ConsoleVariable* pVariable, int iValue);
    void set_value(ConsoleVariable* pVariable, float fValue);
}
