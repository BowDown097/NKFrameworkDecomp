#ifndef LOGGINGCOMPONENT_H
#define LOGGINGCOMPONENT_H
#include <typeinfo>

struct ILoggingComponent
{
    std::type_info typeInfo;
    virtual ~ILoggingComponent();
    virtual void Log(const char* message);
};

#endif // LOGGINGCOMPONENT_H
