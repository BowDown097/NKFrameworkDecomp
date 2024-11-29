#pragma once
#include <string>

struct ConsoleVariable {
    const char* name; // 0x00
    const char* description; // 0x08
    std::string stringValue; // 0x10
    float floatValue; // 0x28
    int intValue; // 0x2C
    ConsoleVariable* head; // 0x30

    ConsoleVariable(const char* name, const char* description, const char* defaultValue);
};

namespace cvar {
    void set(ConsoleVariable* variable, const std::string& value);
    ConsoleVariable* get_list();
    ConsoleVariable* get(const std::string& name);
    void set_integer(ConsoleVariable* variable, int value);
    void set_value(ConsoleVariable* variable, float value);
}
