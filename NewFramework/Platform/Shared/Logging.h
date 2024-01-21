#ifndef LOGGING_H
#define LOGGING_H
#include "LoggingComponent.h"
#include <boost/current_function.hpp>
#include <cstdarg>
#include <list>

#define LOG_ERROR(message, ...) \
    CLogging::GetSingletonPtr()->PrintError(__FILE__, BOOST_CURRENT_FUNCTION, __LINE__, message, __VA_ARGS__)

#define LOG_WARNING(message, ...) \
    CLogging::GetSingletonPtr()->PrintWarning(message, __VA_ARGS__)

namespace boost { class mutex; }

class CLogging
{
public:
    CLogging();
    ~CLogging();

    static CLogging* GetSingletonPtr();
    static size_t StripPath(const char* path);

    void AddComponent(ILoggingComponent* component);
    ILoggingComponent* GetComponentByTypeID(const std::type_info& info) const;
    void RemoveComponent(ILoggingComponent* component);
    void SendToComponents(const char* message);

    void PrintError(const char* file, const char* function, int line, const char* message, ...);
    void PrintWarning(const char* message, ...);
private:
    static inline CLogging* m_pInstance;

    std::list<ILoggingComponent*> m_components;
    boost::mutex* m_mutex;

    void VPrintError(const char* file, const char* function, int line, const char* message, va_list* args);
    void VPrintWarning(const char* message, va_list* args);
};

#endif // LOGGING_H
