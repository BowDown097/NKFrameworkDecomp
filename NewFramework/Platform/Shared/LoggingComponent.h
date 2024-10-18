#pragma once
#include <typeinfo>

struct ILoggingComponent
{
    std::type_info typeInfo;
    virtual ~ILoggingComponent();
    virtual void Log(const char* message);
};
