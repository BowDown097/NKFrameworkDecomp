#ifndef CORE_H
#define CORE_H
#include <cstdint>
#include <string>

#define THREAD_ID int

class CCore
{
public:
    static double getCurrentTime();
    static std::string GetAdId();
    static void CopyToClipboard(std::string text);
    static std::string GetCountryCode();
    static THREAD_ID GetCurrentThreadID();
    static double GetDeviceBootTime();
    static std::string GetDeviceDisplayName();
    static std::string GetDeviceID();
    static std::string GetDeviceName();
    static std::string GetExecutablePath();
    static std::string GetHTTPProxyName();
    static uint32_t GetHTTPProxyPort();
    static bool GetIsCellularNetworkTypeSlow();
    static std::string GetLanguageCode();
    static std::string GetMACAddress();
    static std::string GetManufacturer();
    static float GetMemoryUsage(bool mb);
    static size_t GetPhysicalMemory();
    static void GetSupportedAdProviders(); // TODO: unimplemented on macOS, check other platforms (likely mobile)
    static std::string GetSystemName();
    static std::string GetSystemVersion();
    static std::string GetVendorID();
    static bool HasClipboardTextEntry();
    static bool IsClipboardSupported();
    static bool IsCurrentThread(THREAD_ID id);
    static void OpenAppProduct(std::string, std::string, std::string, std::string); // TODO: unimplemented on macOS, check other platforms (likely mobile)
    static bool OpenURL(std::string url);
    static std::string PasteFromClipboard();
    static void RequestStoragePermissions(); // TODO: unimplemented on macOS, check other platforms (likely mobile)
    static bool SendEmail(const std::string& recipient, const std::string& subject, const std::string& body);
    static void SetScreenCanTimeout(bool timeout); // TODO: unimplemented on macOS, check other platforms
    static void SetThreadID(THREAD_ID id);
    static int SetThreadName(const char* threadName);
    static void Share(std::string, std::string); // TODO: unimplemented on macOS, check other platforms (likely mobile)
    static bool SharingIsSupported();
    static void ShowMessageBox(std::string title, std::string body);
    static std::string ThreadSafeStrError(int errnum);
    static void TriggerRatingPopup(); // TODO: unimplemented on macOS, check other platforms (likely mobile)
    static void UnsetThreadID();
    static int ValidateApplications(std::vector<std::string> applications);
    static int ValidateBinaries(std::vector<std::string> binaries);
    static int ValidateOpenURLs(std::vector<std::string> urls);
};

#endif // CORE_H
