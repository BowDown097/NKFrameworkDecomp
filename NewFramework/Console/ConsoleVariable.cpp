#include "ConsoleVariable.h"

ConsoleVariable* g_cvar_head{};

ConsoleVariable::ConsoleVariable(const char* sName, const char* sDescription, const char* sDefaultValue)
    : sName(sName),
      sDescription(sDescription),
      pHead(g_cvar_head) {

    g_cvar_head = this;
    cvar::set(this, sDefaultValue);
}

void cvar::set(ConsoleVariable* pVariable, const std::string& sValue) {
    if (pVariable->sValue != sValue) {
        pVariable->sValue = sValue;
        pVariable->fValue = atof(pVariable->sValue.c_str());
        pVariable->iValue = atoi(pVariable->sValue.c_str());
    }
}

ConsoleVariable* cvar::get_list() {
    return g_cvar_head;
}

ConsoleVariable* cvar::get(const std::string& sName) {
    ConsoleVariable* result = g_cvar_head;
    while (result && result->sName != sName) {
        result = result->pHead;
    }
    return result;
}

void cvar::set_integer(ConsoleVariable* pVariable, int iValue) {
    char s[32];
    snprintf(s, sizeof(s), "%i", iValue);
    cvar::set(pVariable, s);
}

void cvar::set_value(ConsoleVariable* pVariable, float fValue) {
    char s[32];
    if (int(fValue) == fValue) {
        snprintf(s, sizeof(s), "%i", int(fValue));
    } else {
        snprintf(s, sizeof(s), "%f", fValue);
    }

    cvar::set(pVariable, s);
}
