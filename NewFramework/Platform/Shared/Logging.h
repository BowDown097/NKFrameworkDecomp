#ifndef LOGGING_H
#define LOGGING_H
#include "LoggingComponent.h"
#include <boost/current_function.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/comparison/greater.hpp>
#include <boost/preprocessor/variadic/size.hpp>
#include <cstdarg>
#include <list>

#define LOG_ERROR(...) \
    BOOST_PP_CAT(LOG_ERROR_, BOOST_PP_GREATER(BOOST_PP_VARIADIC_SIZE(__VA_ARGS__), 1))(__VA_ARGS__)
#define LOG_ERROR_0(message) \
    CLogging::GetSingletonPtr()->PrintError(__FILE__, BOOST_CURRENT_FUNCTION, __LINE__, message)
#define LOG_ERROR_1(message, ...) \
    CLogging::GetSingletonPtr()->PrintError(__FILE__, BOOST_CURRENT_FUNCTION, __LINE__, message, __VA_ARGS__)

#define LOG_WARNING(...) \
    BOOST_PP_CAT(LOG_WARNING_, BOOST_PP_GREATER(BOOST_PP_VARIADIC_SIZE(__VA_ARGS__), 1))(__VA_ARGS__)
#define LOG_WARNING_0(message) \
    CLogging::GetSingletonPtr()->PrintWarning(message)
#define LOG_WARNING_1(message, ...) \
    CLogging::GetSingletonPtr()->PrintWarning(message, __VA_ARGS__)

namespace boost { class mutex; }

class CLogging
{
public:
    static CLogging* GetSingletonPtr();
    CLogging();
    ~CLogging();
    static size_t StripPath(const char* path);
    void AddComponent(ILoggingComponent* component);
    void RemoveComponent(ILoggingComponent* component);
    ILoggingComponent* GetComponentByTypeID(const std::type_info& info) const;
    void PrintError(const char* file, const char* function, int line, const char* message, ...);
    void VPrintError(const char* file, const char* function, int line, const char* message, va_list* args);
    void SendToComponents(const char* message);
    void PrintWarning(const char* message, ...);
    void VPrintWarning(const char* message, va_list* args);
private:
    static inline CLogging* m_pInstance;
    std::list<ILoggingComponent*> m_components;
    boost::mutex* m_mutex;
};

#endif // LOGGING_H
