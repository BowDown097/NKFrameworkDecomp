#ifndef LOGGINGCOMPONENT_H
#define LOGGINGCOMPONENT_H
#include <typeinfo>

struct ILoggingComponent
{
    std::type_info typeInfo;
    virtual ~ILoggingComponent() = 0;
    virtual void Log(const char* message) = 0;
};

#endif // LOGGINGCOMPONENT_H
