#ifndef CORE_H
#define CORE_H
#include <string>
#include <vector>

#define THREAD_ID int

class CCore
{
public:
    static double getCurrentTime();
    static float GetMemoryUsage(bool mb);
    static size_t GetPhysicalMemory();
    static std::string GetHTTPProxyName();
    static int GetHTTPProxyPort();
    static std::string GetMACAddress();
    static bool GetIsCellularNetworkTypeSlow();
    static std::string GetVendorId();
    static std::string GetAdId();
    static std::string GetDeviceId();
    static int SetThreadName(const char* threadName);
    static void SetScreenCanTimeout(bool timeout); // TODO: unimplemented on macOS, check other platforms
    static void GetSupportedAdProviders(); // TODO: unimplemented on macOS, check other platforms (likely mobile)
    static void OpenAppProduct(std::string, std::string, std::string, std::string); // TODO: unimplemented on macOS, check other platforms (likely mobile)
    static void CopyToClipboard(std::string text);
    static bool HasClipboardTextEntry();
    static std::string PasteFromClipboard();
    static bool IsClipboardSupported();
    static bool SharingIsSupported();
    static void Share(std::string, std::string); // TODO: unimplemented on macOS, check other platforms (likely mobile)
    static bool OpenURL(std::string url);
    static void TriggerRatingPopup(); // TODO: unimplemented on macOS, check other platforms (likely mobile)
    static void ShowMessageBox(std::string title, std::string body);
    static std::string GetManufacturer();
    static std::string GetDeviceName();
    static std::string GetDeviceDisplayName();
    static double GetDeviceBootTime();
    static std::string GetExecutablePath();
    static std::string GetLanguageCode();
    static std::string GetCountryCode();
    static std::string GetSystemName();
    static std::string GetSystemVersion();
    static int ValidateApplications(std::vector<std::string> applications);
    static int ValidateOpenURLs(std::vector<std::string> urls);
    static int ValidateBinaries(std::vector<std::string> binaries);
    static void RequestStoragePermissions(); // TODO: unimplemented on macOS, check other platforms (likely mobile)
    static bool SendEmail(const std::string& recipient, const std::string& subject, const std::string& body);
    static void SetThreadID(THREAD_ID id);
    static void UnsetThreadID();
    static THREAD_ID GetCurrentThreadID();
    static bool IsCurrentThread(THREAD_ID id);
    static std::string ThreadSafeStrError(int errnum);
};

#endif // CORE_H
