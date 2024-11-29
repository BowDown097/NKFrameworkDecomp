#pragma once
#include <string>
#include <vector>

enum class eNKLoginService { InBlackboard = -2, None = -1, Cache, Email, GameCenter, Facebook, GooglePlay, Steam, Apple };
enum class eNKServiceAction { InBlackboard = -2, None = -1, Login, Create, Link };
enum class eNKServiceAuthMethod { Platform, Email }; // Platform is i.e. Steam on PC, Google Play on Android
enum class eNKServiceLoginResult { Success, Failure };

std::string GetServiceActionStringFromEnum(const eNKServiceAction& serviceAction);
std::string GetLoginServiceStringFromEnum(const eNKLoginService& loginService);
eNKLoginService GetLoginServiceEnumFromString(const std::string& str);
eNKLoginService FindLoginServiceEnumInString(const std::string& str);

struct I_NKLoginServiceObserver
{
    class I_NKLoginService* loginService{}; // 0x08
    virtual void OnServiceLogin(const eNKServiceLoginResult& loginResult) = 0;
};

class I_NKLoginService
{
public:
    virtual ~I_NKLoginService() = default;
    virtual bool IsUserAuthenticated() = 0;
    virtual void AuthenticateUser(const eNKServiceAuthMethod& method) = 0;
    virtual eNKLoginService GetServiceType() = 0;
    virtual bool IsServiceSupported() { return true; }
    virtual std::string SerialisedData_For(const eNKServiceAction& action) = 0;
    virtual std::string UrlExtension_For(const eNKServiceAction& action) = 0;
    virtual std::string GetUserIdentifier() = 0;
    virtual std::string GetUserDisplayName() = 0;
    virtual std::string GetProviderString() = 0;
    virtual void PlayerAuthenticated() = 0;
    virtual void PlayerAuthenticationFailed(int code) = 0;

    void Attach(I_NKLoginServiceObserver* observer);
    void Detach(I_NKLoginServiceObserver* observer);
private:
    std::vector<I_NKLoginServiceObserver*> observers;
};
