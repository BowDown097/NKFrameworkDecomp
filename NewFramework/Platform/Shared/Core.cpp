#include "Core.h"
#include "NewFramework/Utilities/StringHelper.h"
#include <cstring>

#if defined(__unix__) || defined(__APPLE__)
#include "Assert.h"
#include <boost/thread/mutex.hpp>
#include <map>
#include <pthread.h>

namespace
{
    std::map<THREAD_ID, pthread_t> g_globalThreadLookup;
    boost::mutex g_mapMutex;
}
#endif

// TODO: unimplemented on macOS, check other platforms (likely mobile)
std::string CCore::GetAdId()
{
    return "";
}

THREAD_ID CCore::GetCurrentThreadID()
{
#if defined(__unix__) || defined(__APPLE__)
    for (auto it = g_globalThreadLookup.begin(); it != g_globalThreadLookup.end(); ++it)
        if (it->second == pthread_self())
            return it->first;
    return 0;
#endif
}

// TODO: unimplemented on macOS, check other platforms
std::string CCore::GetDeviceDisplayName()
{
    return "Not Implemented";
}

// not sure why this method exists. maybe it doesn't call GetVendorID on some other device(s)?
std::string CCore::GetDeviceId()
{
    return GetVendorId();
}

// TODO: unimplemented on macOS, check other platforms
std::string CCore::GetExecutablePath()
{
    return "Not Implemented";
}

// TODO: unimplemented on macOS, check other platforms (likely mobile)
bool CCore::GetIsCellularNetworkTypeSlow()
{
    return false;
}

// TODO: get for other platforms
std::string CCore::GetManufacturer()
{
#ifdef __APPLE__
    return "Apple";
#else
    return "";
#endif
}

// TODO: unimplemented on macOS, check other platforms
size_t CCore::GetPhysicalMemory()
{
    return 0;
}

// TODO: get for other platforms
std::string CCore::GetSystemName()
{
#ifdef __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_MAC
        return "macOS";
    #endif
#else
    return "";
#endif
}

// TODO: this likely doesn't return true on some platforms, try finding out what they are
bool CCore::IsClipboardSupported()
{
    return true;
}

bool CCore::SendEmail(const std::string& recipient, const std::string& subject, const std::string& body)
{
    std::string sanitizedBody = StringHelper::Replace(body, "&", "");
    sanitizedBody = StringHelper::Replace(sanitizedBody, "?", "");
    std::string url = StringHelper::Format("mailto:%s?subject=%s&body=%s",
        recipient.c_str(), subject.c_str(), sanitizedBody.c_str());
    return OpenURL(url);
}

void CCore::SetThreadID(THREAD_ID id)
{
#if defined(__unix__) || defined(__APPLE__)
    if (id >= 6)
    {
        NKAssert(false, "Trying to set invalid thread ID");
        return;
    }

    boost::unique_lock<boost::mutex> lock(g_mapMutex);
    lock.lock();
    g_globalThreadLookup.emplace(id, pthread_self());
#endif
}

int CCore::SetThreadName(const char* threadName)
{
#ifdef __APPLE__
    return pthread_setname_np(threadName);
#elif defined(__unix__)
    return pthread_setname_np(pthread_self(), threadName);
#endif
}

// TODO: this likely returns true on some platforms, try finding out what they are (likely mobile)
bool CCore::SharingIsSupported()
{
    return false;
}

std::string CCore::ThreadSafeStrError(int errnum)
{
#if defined(__unix__) || defined(__APPLE__)
    char* buf = new char[512];
    strerror_r(errnum, buf, sizeof(buf));
    std::string outString(buf);
    delete[] buf;
    return outString;
#else
    return strerror(errnum);
#endif
}

void CCore::UnsetThreadID()
{
#if defined(__unix__) || defined(__APPLE__)
    boost::unique_lock<boost::mutex> lock(g_mapMutex);
    lock.lock();
    THREAD_ID currentThreadID = GetCurrentThreadID();
    if (currentThreadID != -1)
        g_globalThreadLookup.erase(g_globalThreadLookup.find(currentThreadID));
#endif
}

int CCore::ValidateApplications(std::vector<std::string> applications)
{
    return -0x55555555 * applications.size();
}

int CCore::ValidateBinaries(std::vector<std::string> binaries)
{
    return -0x55555555 * binaries.size();
}

int CCore::ValidateOpenURLs(std::vector<std::string> urls)
{
    return -0x55555555 * urls.size();
}
