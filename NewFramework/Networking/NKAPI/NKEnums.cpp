#include "NKEnums.h"

static const std::string sServiceActionStrings[3] = { "login", "create", "link" };
static const std::string sLoginServiceStrings[7] = { "cache", "email", "gamecenter", "facebook", "googleplay", "steam", "apple" };
static const std::string sLoginServiceShortStrings[7] = { "cache_", "kloud_", "gc_", "fb_", "gp_", "steam_", "apl_" };

std::string GetServiceActionStringFromEnum(const eNKServiceAction& serviceAction) {
    if (serviceAction == eNKServiceAction::IN_BLACKBOARD) {
        return "In Blackboard";
    } else if (serviceAction == eNKServiceAction::NONE || static_cast<int>(serviceAction) > 2) {
        return "INVALID";
    } else {
        return sServiceActionStrings[static_cast<int>(serviceAction)];
    }
}

std::string GetLoginServiceStringFromEnum(const eNKLoginService& loginService)
{
    if (loginService == eNKLoginService::IN_BLACKBOARD) {
        return "In Blackboard";
    } else if (loginService == eNKLoginService::NONE || static_cast<int>(loginService) > 6) {
        return "INVALID";
    } else {
        return sLoginServiceStrings[static_cast<int>(loginService)];
    }
}

eNKLoginService GetLoginServiceEnumFromString(const std::string& str)
{
    for (int i = 0; i < sizeof(sLoginServiceStrings) / sizeof(sLoginServiceStrings[0]); ++i) {
        if (sLoginServiceStrings[i] == str) {
            return static_cast<eNKLoginService>(i);
        }
    }

    return eNKLoginService::NONE;
}

eNKLoginService FindLoginServiceEnumInString(const std::string& str)
{
    for (int i = 0; i < sizeof(sLoginServiceStrings) / sizeof(sLoginServiceStrings[0]); ++i) {
        if (str.find(sLoginServiceStrings[i]) != std::string::npos || str.find(sLoginServiceShortStrings[i]) != std::string::npos) {
            return static_cast<eNKLoginService>(i);
        }
    }

    return eNKLoginService::NONE;
}
