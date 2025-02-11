#pragma once
#include <string>

enum class eNKFileClientFilePermissions {
    PUBLIC
};
enum class eNKLinkServerCluster {
    ALAN_DEV,
    ROSS_DEV,
    STAGING,
    LIVE,
    NONE
};
enum class eNKLoginService {
    IN_BLACKBOARD = -2,
    NONE = -1,
    CACHE,
    EMAIL,
    GAMECENTER,
    FACEBOOK,
    GOOGLEPLAY,
    STEAM,
    APPLE
};
enum class eNKPublicUrlChannel {
    STANDARD,
    PRIORITY
};
enum class eNKServiceAction {
    IN_BLACKBOARD = -2,
    NONE = -1,
    LOGIN,
    CREATE,
    LINK
};
enum class eNKServiceAuthMethod {
    PLATFORM, // i.e. steam on PC, google play on android
    EMAIL
};
enum class eNKServiceLoginResult {
    SUCCESS,
    FAILURE
};

std::string GetServiceActionStringFromEnum(const eNKServiceAction& serviceAction);
std::string GetLoginServiceStringFromEnum(const eNKLoginService& loginService);
eNKLoginService GetLoginServiceEnumFromString(const std::string& str);
eNKLoginService FindLoginServiceEnumInString(const std::string& str);
