#include "NKLoginService.h"
#include <algorithm>

static const std::string sServiceActionStrings[3] = { "login", "create", "link" };
static const std::string sLoginServiceStrings[7] = { "cache", "email", "gamecenter", "facebook", "googleplay", "steam", "apple" };
static const std::string sLoginServiceShortStrings[7] = { "cache_", "kloud_", "gc_", "fb_", "gp_", "steam_", "apl_" };

std::string GetServiceActionStringFromEnum(const eNKServiceAction& serviceAction)
{
    if (serviceAction == eNKServiceAction::InBlackboard)
        return "In Blackboard";
    if (serviceAction == eNKServiceAction::None || static_cast<int>(serviceAction) > 2)
        return "INVALID";
    return sServiceActionStrings[static_cast<int>(serviceAction)];
}

std::string GetLoginServiceStringFromEnum(const eNKLoginService& loginService)
{
    if (loginService == eNKLoginService::InBlackboard)
        return "In Blackboard";
    if (loginService == eNKLoginService::None || static_cast<int>(loginService) > 6)
        return "INVALID";
    return sLoginServiceStrings[static_cast<int>(loginService)];
}

eNKLoginService GetLoginServiceEnumFromString(const std::string& str)
{
    for (int i = 0; i < sizeof(sLoginServiceStrings) / sizeof(sLoginServiceStrings[0]); ++i)
        if (sLoginServiceStrings[i] == str)
            return static_cast<eNKLoginService>(i);
    return eNKLoginService::None;
}

eNKLoginService FindLoginServiceEnumInString(const std::string& str)
{
    for (int i = 0; i < sizeof(sLoginServiceStrings) / sizeof(sLoginServiceStrings[0]); ++i)
        if (str.find(sLoginServiceStrings[i]) != std::string::npos || str.find(sLoginServiceShortStrings[i]) != std::string::npos)
            return static_cast<eNKLoginService>(i);
    return eNKLoginService::None;
}

void I_NKLoginService::Attach(I_NKLoginServiceObserver* observer)
{
    auto it = std::find(observers.begin(), observers.end(), observer);
    if (it != observers.end())
        observers.push_back(observer);
}

void I_NKLoginService::Detach(I_NKLoginServiceObserver* observer)
{
    auto it = std::find(observers.begin(), observers.end(), observer);
    if (it != observers.end())
        observers.erase(it);
}
