#pragma once
#include "NewFramework/Networking/NKAPI/NKEnums.h"
#include <vector>

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
