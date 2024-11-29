#include "ConsoleVariable.h"
#include <xmmintrin.h>

ConsoleVariable::ConsoleVariable(const char* name, const char* description, const char* defaultValue)
    : name(name), description(description), head(g_cvar_head) {
    g_cvar_head = this;
    cvar::set(this, defaultValue);
}

void cvar::set(ConsoleVariable* variable, const std::string& value) {
    if (variable->stringValue == value) {
        return;
    }

    variable->stringValue = value;
    variable->floatValue = std::atof(variable->stringValue.c_str());
    variable->intValue = std::atoi(variable->stringValue.c_str());
}

void cvar::set_integer(ConsoleVariable* variable, int value) {
    char s[32];
    snprintf(s, sizeof(s), "%i", value);
    cvar::set(variable, s);
}

void cvar::set_value(ConsoleVariable* variable, float value) {
    char s[32];
    if (int(value) == value) {
        snprintf(s, sizeof(s), "%i", int(value));
    } else {
        snprintf(s, sizeof(s), "%f", value);
    }

    cvar::set(variable, s);
}
