#pragma once
#include "Impl/NKSessionImpl.h"

class NKSession {
public:
    static NKSession* GetActiveSession();
    NKSession();
    const NKAccessToken& GetAccessToken() const;
    const NKResponseUser& GetUserDetails() const;
    void EnableLogging(bool enabled) const;
    bool LoggingEnabled() const;
    int GetAppID() const;
    int GetSkuID() const;
    NKSessionState GetState() const;
    eNKLinkServerCluster GetServerCluster() const;
    void Close(bool hasCache);
    void Initialise(const int& appID, const int& skuID, const std::string& privateKey,
                    CEventManager* eventManager, eNKLinkServerCluster serverCluster,
                    LoginCallback loginCallback);
    void Update(const float& elapsed);
    void SetAppInterface(I_NKSDKApp* appInterface);
    void AddServiceInterface(const eNKLoginService& type, I_NKLoginService* service);
    void Open(bool needSession, bool, bool);
    void LoginUsingService(const eNKLoginService& loginServiceType, bool authWithEmail, bool, bool);
    void LinkService(const eNKLoginService& loginServiceType, bool authWithEmail);
    void ShowUI();
    const std::string& GetPrivateKey() const;
    void SetScreenFocalPoint(uint16_t x, uint16_t y);
    void GetScreenFocalPoint(uint16_t& x, uint16_t& y) const;
    std::string GetSupportedInterfaceTags() const;
    std::vector<std::string> GetLoggedInInterfaceIDs() const;
    std::vector<std::string> GetLoggedInInterfaceProviders() const;
    bool IsAccountUsingService(eNKLoginService type) const;
    const NKError& GetLastError() const;
private:
    inline static NKSession* s_ActiveSession;

    NKSessionImpl impl;

    static void CleanUp();
};
