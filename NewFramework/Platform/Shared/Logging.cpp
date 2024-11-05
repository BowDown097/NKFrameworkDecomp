#include "Logging.h"
#include "NewFramework/Analytics/DGAnalytics.h"

CLogging* CLogging::GetSingletonPtr()
{
    if (!m_pInstance)
        m_pInstance = new CLogging;
    return m_pInstance;
}

CLogging::CLogging() : m_mutex(new boost::mutex) {}

CLogging::~CLogging()
{
    for (ILoggingComponent* component : m_components)
        delete component;
    m_components.clear();
    delete m_mutex;
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

void CLogging::AddComponent(ILoggingComponent* component)
{
    m_components.push_back(component);
}

void CLogging::RemoveComponent(ILoggingComponent* component)
{
    return m_components.remove(component);
}

ILoggingComponent* CLogging::GetComponentByTypeID(const std::type_info& info) const
{
    for (ILoggingComponent* component : m_components)
        if (component->typeInfo == info)
            return component;
    return nullptr;
}

void CLogging::PrintError(const char* file, const char* function, int line, const char* message, ...)
{
    va_list args;
    va_start(args, message);
    VPrintError(file, function, line, message, &args);
    va_end(args);
}

#define MAX_TEXT_BUFFER 0x5000
void CLogging::VPrintError(const char* file, const char* function, int line, const char* message, va_list* args)
{
    if (!message)
        return;

    boost::unique_lock<boost::mutex> lock(*m_mutex);

    static char text[MAX_TEXT_BUFFER - 1];
    vsnprintf(text, MAX_TEXT_BUFFER - 2, message, *args);

    size_t fileStrippedIndex = StripPath(file);
    static char final[MAX_TEXT_BUFFER];
    sprintf(final, ">> %s:%d %s %s", &file[fileStrippedIndex], line, function, text);
    SendToComponents(final);

    DGAnalytics::Instance()->Log("ERROR: %s(%d) %s %s", &file[fileStrippedIndex], line, function, text);
}

void CLogging::SendToComponents(const char* message)
{
    for (ILoggingComponent* component : m_components)
        component->Log(message);
}

void CLogging::PrintWarning(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    VPrintWarning(message, &args);
    va_end(args);
}

#define LEN_TIMESTAMP 0x20
void CLogging::VPrintWarning(const char* message, va_list* args)
{
    if (!message)
        return;

    boost::unique_lock<boost::mutex> lock(*m_mutex);

    static size_t result = 0;
    static char timestamp[LEN_TIMESTAMP];
    static time_t t = time(nullptr);
    size_t timestampSize;

    time_t currentTime = time(nullptr);
    if (t != currentTime)
    {
        tm* local = localtime(&currentTime);
        timestampSize = strftime(timestamp, LEN_TIMESTAMP, "%d/%m/%y %H:%M:%S > ", local);
        result = timestampSize;
        t = currentTime;
    }
    else
    {
        timestampSize = result;
    }

    static char text[MAX_TEXT_BUFFER];
    size_t textSize;
    if (timestampSize > 0)
    {
        memcpy(text, timestamp, timestampSize);
        if (timestampSize == MAX_TEXT_BUFFER)
        {
            return;
        }

        textSize = MAX_TEXT_BUFFER - timestampSize;
    }
    else
    {
        textSize = MAX_TEXT_BUFFER;
    }

    vsnprintf(&text[timestampSize], textSize, message, *args);
    SendToComponents(text);
}
