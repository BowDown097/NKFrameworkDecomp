#include "ConsoleVariable.h"

ConsoleVariable* g_cvar_head{};

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
    variable->floatValue = atof(variable->stringValue.c_str());
    variable->intValue = atoi(variable->stringValue.c_str());
}

ConsoleVariable* cvar::get_list() {
    return g_cvar_head;
}

ConsoleVariable* cvar::get(const std::string& name) {
    ConsoleVariable* result = g_cvar_head;

    while (result && result->name != name) {
        result = result->head;
    }

    return result;
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
