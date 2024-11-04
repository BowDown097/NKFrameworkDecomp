#pragma once
#include <string>
#include <vector>

enum class eNKLoginService { Cache, Email, GameCenter, Facebook, GooglePlay, Steam, Apple };
enum class eNKServiceAction { Login, Create, Link };
enum class eNKServiceAuthMethod { Email, Platform }; // Platform is i.e. Steam on PC, Google Play on Android
class I_NKLoginServiceObserver;

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
private:
    std::vector<I_NKLoginServiceObserver*> observers;

    void Attach(I_NKLoginServiceObserver* observer);
    void Detach(I_NKLoginServiceObserver* observer);
};
