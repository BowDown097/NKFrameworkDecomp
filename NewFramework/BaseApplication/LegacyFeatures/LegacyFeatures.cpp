#include "LegacyFeatures.h"

// TODO: obviously, some features are platform-dependent. figure all that out.

/*
How to find LegacyFeatures without symbols
(bScreenRecursiveBlockingCheck, bUseNewerBackButtonbehaviour, bUseNKMultiProtocol, bSpriteTracking have no reliable method)

bAssertOnAnimationDupTimeline:
    Search for the string "Timeline with id '%d' already exists.  Sizes: old[%d] / new[%d]".
    If there are no results, the value is likely false.
    Otherwise:
        Look for byte_XXXXX shortly before xref to the string.

bSupportsDesktopRetinaDisplay:
    Only true on macOS

bUseFastTime:
    Search for the string "use_fast_time".
    Look for byte_XXXXX shortly after xref to the string.

bUseOldWebView:
    Search for the string "/sdk/#signup=true".
    If there are no results, the value is likely true.
    Otherwise:
        Look for byte_XXXXX shortly before xref to the string.

bLocked:
    Search for the string "Tried to change locked legacy features".
    If there are no results, the value is likely true.
    Otherwise:
        Look for comparison with 0 of byte_XXXXX shortly before either xref to the string.

bUseLegacyNetworking:
    Search for the string "Receive timer expired (local dead-man switch) - disconnecting network queue".
    If there are no results, the value is likely false.
    Otherwise:
        Open up the xref to the containing function.
        Look for comparison with 1 of byte_XXXXX shortly before the first xref to that function.

bUseTransitionalNetQueue:
    Follow the steps for bUseLegacyNetworking, except in the final step, look before the second xref.

NoAnalyticKeyAssert:
    Follow the steps for bLocked, except in the final step, look immediately after the first xref to the string.

ReplaceAnalyticKeyUnderscore:
    Follow the steps for NoAnalyticKeyAssert, except look immediately after the second xref.

bUseSpriteInfoFallback:
    Search for string "texture_not_found". Look for byte_XXXXX shortly after xref to the string.

bAndroidTabletAspectScale, bAndroidSquareAspectScale, bUseOldSafeRectLogicAndroid:
    Only true on Android
*/

// these 4 are true across all so far checked platforms (macOS, Windows, Android, iOS).
bool LegacyFeatures::bAssertOnAnimationDupTimeline = true;
bool LegacyFeatures::bScreenRecursiveBlockingCheck = true;
bool LegacyFeatures::bUseFastTime = true;
bool LegacyFeatures::bUseOldWebView = true;

#ifdef __APPLE__
    #include <TargetConditionals.h>
    #if TARGET_OS_OSX
        bool LegacyFeatures::bSupportsDesktopRetinaDisplay = true;
    #endif
#elif defined(__ANDROID__)
    bool LegacyFeatures::bAndroidSquareAspectScale = true;
    bool LegacyFeatures::bAndroidTabletAspectScale = true;
    bool LegacyFeatures::bUseOldSafeRectLogicAndroid = true;
#endif
