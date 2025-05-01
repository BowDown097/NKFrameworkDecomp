#pragma once

#include "NewFramework/Platform/Shared/Assert.h"

struct LegacyFeatures {
    static bool bAndroidSquareAspectScale;
    static bool bAndroidTabletAspectScale;
    static bool bAssertOnAnimationDupTimeline;
    static bool bLocked;
    static bool bScreenRecursiveBlockingCheck;
    static bool bSpriteTracking;
    static bool bSupportsDesktopRetinaDisplay;
    static bool bUseFastTime;
    static bool bUseLegacyNetworking;
    static bool bUseLegacyTextObject;
    static bool bUseNewerBackButtonbehaviour;
    static bool bUseNewerLineSplit;
    static bool bUseNKMultiProtocol;
    static bool bUseOldSafeRectLogicAndroid;
    static bool bUseOldWebView;
    static bool bUseSpriteInfoFallback;
    static bool bUseTransitionalNetQueue;
    static bool bUseVisibleConcat;
    static int eLegacyNetGame;
    static bool NoAnalyticKeyAssert;
    static bool ReplaceAnalyticKeyUnderscore;






























































































































































    static bool LockCheck() {
        ENFORCE_LINE(186); return NKAssert(bLocked, "Tried to change locked legacy features");
    }
};
