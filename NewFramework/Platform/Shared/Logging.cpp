#include "Logging.h"
#include "NewFramework/Analytics/DGAnalytics.h"
#include <boost/thread/mutex.hpp>

CLogging::CLogging() : m_mutex(new boost::mutex) {}

CLogging::~CLogging()
{
    for (ILoggingComponent* component : m_components)
        delete component;
    m_components.clear();
    delete m_mutex;
}

void CLogging::AddComponent(ILoggingComponent* component)
{
    m_components.push_back(component);
}

ILoggingComponent* CLogging::GetComponentByTypeID(const std::type_info& info) const
{
    auto it = std::find_if(m_components.cbegin(), m_components.cend(), [&info](ILoggingComponent* c) {
        return c->typeInfo == info;
    });

    return it != m_components.cend() ? *it : nullptr;
}

CLogging* CLogging::GetSingletonPtr()
{
    if (!m_pInstance)
        m_pInstance = new CLogging;
    return m_pInstance;
}

void CLogging::PrintError(const char* file, const char* function, int line, const char* message, ...)
{
    va_list args;
    va_start(args, message);
    VPrintError(file, function, line, message, &args);
    va_end(args);
}

void CLogging::PrintWarning(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    VPrintWarning(message, &args);
    va_end(args);
}

void CLogging::RemoveComponent(ILoggingComponent* component)
{
    return m_components.remove(component);
}

void CLogging::SendToComponents(const char* message)
{
    for (ILoggingComponent* component : m_components)
        component->Log(message);
}

size_t CLogging::StripPath(const char* path)
{
    if (!path)
        return 0;

    size_t index = strlen(path) - 1;
    while (path[index] != '/')
    {
        if (index-- <= 0)
            return 0xffffffff;
    }

    return index;
}

void CLogging::VPrintError(const char* file, const char* function, int line, const char* message, va_list* args)
{
    if (!message)
        return;

    boost::unique_lock<boost::mutex> lock(*m_mutex);
    lock.lock();

    static char text[20479];
    vsnprintf(text, sizeof(text) - 1, message, *args);

    size_t fileStrippedIndex = StripPath(file);
    static char final[20480];
    sprintf(final, ">> %s:%d %s %s", &file[fileStrippedIndex], line, function, text);
    SendToComponents(final);

    DGAnalytics::Instance()->Log("ERROR: %s(%d) %s %s", &file[fileStrippedIndex], line, function, text);

    lock.unlock();
}

void CLogging::VPrintWarning(const char* message, va_list* args)
{
    if (!message)
        return;

    boost::unique_lock<boost::mutex> lock(*m_mutex);
    lock.lock();

    static size_t result = 0;
    static char timestamp[32];
    static time_t t = time(NULL);
    size_t timestampSize;

    time_t currentTime = time(NULL);
    if (t != currentTime)
    {
        tm* local = localtime(&currentTime);
        timestampSize = strftime(timestamp, sizeof(timestamp), "%d/%m/%y %H:%M:%S > ", local);
        result = timestampSize;
        t = currentTime;
    }
    else
    {
        timestampSize = result;
    }

    static char text[20479];
    size_t textSize;
    if (timestampSize > 0)
    {
        memcpy(text, timestamp, timestampSize);
        if (timestampSize == 20480)
        {
            lock.unlock();
            return;
        }

        textSize = 20480 - timestampSize;
    }
    else
    {
        textSize = 20480;
    }

    vsnprintf(&text[timestampSize], textSize, message, *args);
    SendToComponents(text);
    lock.unlock();
}
