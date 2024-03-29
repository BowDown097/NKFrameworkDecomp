#include "NewFramework/Platform/Shared/Core.h"
#include <cstdint>
#include <AppKit/NSAlert.h>
#include <AppKit/NSPasteboard.h>
#include <Foundation/NSLocale.h>
#include <Foundation/NSString.h>
#include <mach/mach.h>
#include <net/if.h>
#include <net/if_dl.h>
#include <QuartzCore/QuartzCore.h>
#include <sys/socket.h>
#include <sys/sysctl.h>

CFTimeInterval CCore::getCurrentTime()
{
    return CACurrentMediaTime();
}

float CCore::GetMemoryUsage(bool mb)
{
    task_basic_info task_info_out;
    mach_msg_type_number_t task_info_outCnt = TASK_BASIC_INFO_COUNT;

    kern_return_t kr = task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)&task_info_out, &task_info_outCnt);
    if (kr != KERN_SUCCESS)
        return -1;

    float result = task_info_out.resident_size;
    if (mb)
        result *= 0.00000095367432;

    return result;
}

std::string CCore::GetHTTPProxyName()
{
    CFDictionaryRef proxySettings = CFNetworkCopySystemProxySettings();
    CFStringRef httpProxyRef = (CFStringRef)CFDictionaryGetValue(proxySettings, kCFNetworkProxiesHTTPProxy);
    char httpProxyBuffer[4096] = {0};
    return CFStringGetCString(httpProxyRef, httpProxyBuffer, sizeof(httpProxyBuffer), kCFStringEncodingUTF8)
        ? std::string(httpProxyBuffer) : "";
}

int CCore::GetHTTPProxyPort()
{
    CFDictionaryRef proxySettings = CFNetworkCopySystemProxySettings();
    CFNumberRef httpPortRef = (CFNumberRef)CFDictionaryGetValue(proxySettings, kCFNetworkProxiesHTTPPort);
    int valuePtr;
    return CFNumberGetValue(httpPortRef, kCFNumberSInt32Type, &valuePtr) ? valuePtr : -1;
}

std::string CCore::GetMACAddress()
{
    int mgmtInfoBase[6];
    char* msgBuffer;
    NSString* errorFlag;
    size_t length;

    // setup the management information base
    mgmtInfoBase[0] = CTL_NET; // request network subsystem
    mgmtInfoBase[1] = AF_ROUTE; // request routing table info
    mgmtInfoBase[2] = 0;
    mgmtInfoBase[3] = AF_LINK; // request link layer information
    mgmtInfoBase[4] = NET_RT_IFLIST; // request all configured interfaces

    // with all configured interfaces requested, get handle index
    if ((mgmtInfoBase[5] = if_nametoindex("en0")) == 0)
        errorFlag = @"if_nametoindex failure";
    // get the size of the data available (store in len)
    else if (sysctl(mgmtInfoBase, 6, nullptr, &length, nullptr, 0) < 0)
        errorFlag = @"sysctl mgmtInfoBase failure";
    // alloc memory based on above call
    else if (!(msgBuffer = (char*)malloc(length)))
        errorFlag = @"buffer allocation failure";
    // get system information, store in buffer
    else if (sysctl(mgmtInfoBase, 6, msgBuffer, &length, nullptr, 0) < 0)
    {
        free(msgBuffer);
        errorFlag = @"sysctl msgBuffer failure";
    }
    else
    {
        // map msgbuffer to interface message structure
        if_msghdr* interfaceMsgStruct = (if_msghdr*)msgBuffer;

        // map to link-level socket structure
        sockaddr_dl* socketStruct = (sockaddr_dl*)(interfaceMsgStruct + 1);

        // copy link layer address data in socket structure to an array
        unsigned char macAddress[6];
        memcpy(&macAddress, socketStruct->sdl_data + socketStruct->sdl_nlen, sizeof(macAddress));

        // read from char array into a string object, into traditional MAC address format
        NSString* macAddressString = [NSString stringWithFormat:@"%02X:%02X:%02X:%02X:%02X:%02X",
            macAddress[0], macAddress[1], macAddress[2], macAddress[3], macAddress[4], macAddress[5]];

        // release the buffer memory
        free(msgBuffer);

        return [macAddressString UTF8String];
    }

    // error...
    NSLog(@"Error: %@", errorFlag);
    return [errorFlag UTF8String];
}

// TODO: i don't feel like doing this yet because this relies on some stuff and it might be a headache:
// CSteamInterface class (CSteamInterface::IsSteamRunning), hashlib++ library, and the steam API
std::string CCore::GetVendorId()
{
    return "";
}

void CCore::CopyToClipboard(std::string text)
{
    NSString* textNS = [[[NSString alloc] initWithUTF8String:text.c_str()] autorelease];
    NSPasteboard* pasteboard = [NSPasteboard generalPasteboard];
    [pasteboard declareTypes:[NSArray arrayWithObject:NSStringPboardType] owner:nil];
    [pasteboard setString:textNS forType:NSStringPboardType];
}

bool CCore::HasClipboardTextEntry()
{
    return [[NSPasteboard generalPasteboard] canReadObjectForClasses:@[[NSString class]] options:[NSDictionary dictionary]];
}

std::string CCore::PasteFromClipboard()
{
    NSString* clipboardText = [[NSPasteboard generalPasteboard] stringForType:NSPasteboardTypeString];
    return clipboardText ? [clipboardText UTF8String] : "";
}

bool CCore::OpenURL(std::string url)
{
    CFURLRef cfurl = CFURLCreateWithBytes(kCFAllocatorDefault, reinterpret_cast<const uint8_t*>(url.c_str()), url.size(), kCFStringEncodingASCII, nullptr);
    LSOpenCFURLRef(cfurl, nullptr);
    CFRelease(cfurl);
    return true;
}

void CCore::ShowMessageBox(std::string title, std::string body)
{
    dispatch_async(dispatch_get_main_queue(), ^() {
        NSAlert* alert = [[[NSAlert alloc] init] autorelease];
        NSString* titleNS = [[[NSString alloc] initWithUTF8String:title.c_str()] autorelease];
        NSString* bodyNS = [[[NSString alloc] initWithUTF8String:body.c_str()] autorelease];
        [alert setMessageText:titleNS];
        [alert setInformativeText:bodyNS];
        [alert runModal];
    });
}

std::string CCore::GetDeviceName()
{
    CFMutableDictionaryRef expertDevice = IOServiceMatching("IOPlatformExpertDevice");
    io_service_t expertService = IOServiceGetMatchingService(kIOMasterPortDefault, expertDevice);
    CFTypeRef modelProperty = IORegistryEntryCreateCFProperty(expertService, CFSTR("model"), kCFAllocatorDefault, 0);
    NSString* modelString = [[NSString alloc] initWithData:(__bridge NSData*)modelProperty encoding:NSUTF8StringEncoding];
    CFRelease(modelProperty);
    IOObjectRelease(expertService);
    return modelString ? [modelString UTF8String] : "";
}

NSTimeInterval CCore::GetDeviceBootTime()
{
    static NSTimeInterval boot_time = 0;
    if (!boot_time)
    {
        NSTimeInterval timeNow = [[NSDate date] timeIntervalSince1970];
        NSTimeInterval uptime = [[NSProcessInfo processInfo] systemUptime];
        boot_time = timeNow - uptime;
    }

    return boot_time;
}

std::string CCore::GetLanguageCode()
{
    NSString* languageCode = [[NSLocale preferredLanguages] objectAtIndex:0];
    return languageCode ? [languageCode UTF8String] : "";
}

std::string CCore::GetCountryCode()
{
    NSString* countryCode = [[NSLocale currentLocale] objectForKey:NSLocaleCountryCode];
    return countryCode ? [countryCode UTF8String] : "";
}

std::string CCore::GetSystemVersion()
{
    NSProcessInfo* processInfo = [NSProcessInfo processInfo];
    NSString* versionString;
    if (processInfo)
    {
        NSOperatingSystemVersion osVersion = [processInfo operatingSystemVersion];
        versionString = [NSString stringWithFormat:@"%ld.%ld.%ld",
            osVersion.majorVersion, osVersion.minorVersion, osVersion.patchVersion];
    }
    else
    {
        versionString = [NSString stringWithFormat:@"%ld.%ld.%ld", 0l, 0l, 0l];
    }

    return [versionString UTF8String];
}
