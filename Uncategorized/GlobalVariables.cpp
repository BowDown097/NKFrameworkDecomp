#include "GlobalVariables.h"

// TODO: i think this could be inlining the functions GetPlatformName() and platform_id(),
// which may originate from socialoptions.cpp and nk_attribution.cpp, which have yet to be implemented.
// thankfully these files have line numbers to work off of, which may provide an answer to this theory.
void CGlobalVariables::GetPlatformName(std::string& os, std::string& platform)
{
    os = "";
    platform = "";

// TODO x2: introduce more OSes
#ifdef __APPLE__
    #include <TargetConditionals.h>
    #if TARGET_OS_OSX
        os = "osx";
    #endif
#endif

#ifdef TARGET_PLATFORM
    platform = TARGET_PLATFORM;
#endif
}

CGlobalVariables* CGlobalVariables::Instance()
{
    if (!pInstance)
        pInstance = new CGlobalVariables;
    return pInstance;
}

float CGlobalVariables::GetDefaultSpriteScale()
{
    switch (deviceScale)
    {
    case DeviceScale::Low:
        return 1.0;
    case DeviceScale::High:
    case DeviceScale::Tablet:
        return 0.5;
    case DeviceScale::Ultra:
        return 0.25;
    default:
        return 0.0;
    }
}
