#pragma once
#include "NewFramework/Networking/NKAPI/Impl/NKMessages.h"
#include "Uncategorized/NKLoginService.h"

class C_NKLoginQuery;

struct I_NKLoginWebviewListener
{
    virtual ~I_NKLoginWebviewListener() = default;
    virtual void WebView_ManualClose() {}
    virtual void WebView_DidLoad() {}
    virtual void WebView_DidHide() {}
    virtual void WebView_LoginFailed() {}
    virtual void WebView_LoginSucceeded(const std::string& sessionToken, bool newUser, std::string url) {}
    virtual void WebView_UpdateUser(const std::string& sessionToken) {}
    virtual void WebView_ServiceActionRequested(const eNKLoginService& loginServiceType, const eNKServiceAction& serviceAction) {}
    virtual void WebView_StartLogOut() {}
};

class C_NKLoginWebView
{
public:
    C_NKLoginWebView(int appID, int skuID, const std::string& privateKey, const NKAccessToken& accessToken,
                     const NKResponseUser& responseUser, std::vector<std::string> providers);
    virtual ~C_NKLoginWebView();

    void StartIteratingListeners();
    void FinishIteratingListeners();
    void AddListener(I_NKLoginWebviewListener* listener);
    void RemoveListener(I_NKLoginWebviewListener* listener);
    void ManualClose();
    void LoginFailed();
    void LoginSucceeded(const std::string& sessionToken, bool newUser, std::string url);
    void UpdateUser(const std::string& sessionToken);
    void ServiceActionRequested(const eNKLoginService& loginServiceType, const eNKServiceAction& serviceAction);
    void LogOut();
    void DidLoad();
    void DidHide();
    bool UrlMatchesRedirect(const std::string& url, const std::string& redirect);
    bool UrlRequested(const std::string& url);

    virtual bool ShowWithQuery(const C_NKLoginQuery& query, uint16_t x, uint16_t y, uint16_t width, uint16_t height) = 0;
    virtual void Show() = 0;
    virtual void Hide() = 0;
    virtual void Update(const float& elapsed) {}
    virtual void InjectJavascript(const std::string& code) = 0;
    virtual std::string WebviewRedirectDomain() { return "http:/"; }
private:
    std::vector<I_NKLoginWebviewListener*> listeners; // 0x08
    std::vector<I_NKLoginWebviewListener*> listenersToAdd; // 0x20
    std::vector<I_NKLoginWebviewListener*> listenersToRemove; // 0x38
    bool iterating{}; // 0x50
    int appID; // 0x54
    int skuID; // 0x58
    std::string privateKey; // 0x60
    std::vector<std::string> providers; // 0x78
    NKAccessToken accessToken; // 0x90
    NKResponseUser responseUser; // 0xB0
};

// this will be implemented in NKLoginWebView_Steam.cpp for Steam, etc.
C_NKLoginWebView* Create_NKLoginWebview(int appID, int skuID, const std::string& privateKey, const NKAccessToken& accessToken,
                                        const NKResponseUser& responseUser, std::vector<std::string> providers);
