#include "NKLoginWebview.h"
#include "NewFramework/Platform/Shared/Logging.h"
#include "NewFramework/Utilities/StringHelper.h"
#include "NewFramework/Utilities/URLParser.h"

bool GLOBAL_HACK_disable_fx;

C_NKLoginWebView::C_NKLoginWebView(int appID, int skuID, const std::string& privateKey, const NKAccessToken& accessToken,
                                   const NKResponseUser& responseUser, std::vector<std::string> providers)
    : appID(appID),
      skuID(skuID),
      privateKey(privateKey),
      providers(providers),
      accessToken(accessToken),
      responseUser(responseUser)
{
    LOG_WARNING("C_NKLoginWebView()");
    GLOBAL_HACK_disable_fx = true;
}

C_NKLoginWebView::~C_NKLoginWebView()
{
    LOG_WARNING("~C_NKLoginWebView()");
    GLOBAL_HACK_disable_fx = false;
}

void C_NKLoginWebView::StartIteratingListeners()
{
    iterating = true;
}

// the disassembly claims this calls AddListener() and RemoveListener(), but i don't know if i agree with that,
// reason being, i'm pretty sure this would recurse a shit load of times due to those methods also calling this one.
// but idk, ig we'll see in testing later on. or maybe someone can check this and see if i'm missing something.
void C_NKLoginWebView::FinishIteratingListeners()
{
    iterating = false;
    for (I_NKLoginWebviewListener* listener : listenersToAdd)
        AddListener(listener);
    for (I_NKLoginWebviewListener* listener : listenersToRemove)
        RemoveListener(listener);
}

void C_NKLoginWebView::AddListener(I_NKLoginWebviewListener* listener)
{
    if (iterating)
    {
        listenersToAdd.push_back(listener);
    }
    else
    {
        StartIteratingListeners();
        listeners.push_back(listener);
        FinishIteratingListeners();
    }
}

void C_NKLoginWebView::RemoveListener(I_NKLoginWebviewListener* listener)
{
    if (iterating)
    {
        listenersToRemove.push_back(listener);
    }
    else
    {
        StartIteratingListeners();
        listeners.erase(std::find(listeners.begin(), listeners.end(), listener));
        FinishIteratingListeners();
    }
}

void C_NKLoginWebView::ManualClose()
{
    StartIteratingListeners();
    for (I_NKLoginWebviewListener* listener : listeners)
        listener->WebView_ManualClose();
    FinishIteratingListeners();
}

void C_NKLoginWebView::LoginFailed()
{
    StartIteratingListeners();
    for (I_NKLoginWebviewListener* listener : listeners)
        listener->WebView_LoginFailed();
    FinishIteratingListeners();
}

void C_NKLoginWebView::LoginSucceeded(const std::string& sessionToken, bool newUser, std::string url)
{
    StartIteratingListeners();
    for (I_NKLoginWebviewListener* listener : listeners)
        listener->WebView_LoginSucceeded(sessionToken, newUser, url);
    FinishIteratingListeners();
}

void C_NKLoginWebView::UpdateUser(const std::string& sessionToken)
{
    StartIteratingListeners();
    for (I_NKLoginWebviewListener* listener : listeners)
        listener->WebView_UpdateUser(sessionToken);
    FinishIteratingListeners();
}

void C_NKLoginWebView::ServiceActionRequested(const eNKLoginService& loginServiceType, const eNKServiceAction& serviceAction)
{
    StartIteratingListeners();
    for (I_NKLoginWebviewListener* listener : listeners)
        listener->WebView_ServiceActionRequested(loginServiceType, serviceAction);
    FinishIteratingListeners();
}

void C_NKLoginWebView::LogOut()
{
    StartIteratingListeners();
    for (I_NKLoginWebviewListener* listener : listeners)
        listener->WebView_StartLogOut();
    FinishIteratingListeners();
}

void C_NKLoginWebView::DidLoad()
{
    StartIteratingListeners();
    for (I_NKLoginWebviewListener* listener : listeners)
        listener->WebView_DidLoad();
    FinishIteratingListeners();
}

void C_NKLoginWebView::DidHide()
{
    StartIteratingListeners();
    for (I_NKLoginWebviewListener* listener : listeners)
        listener->WebView_DidHide();
    FinishIteratingListeners();
}

bool C_NKLoginWebView::UrlMatchesRedirect(const std::string& url, const std::string& redirect)
{
    std::string fullRedirect = WebviewRedirectDomain() + redirect;
    return url.find(fullRedirect) != std::string::npos;
}

bool C_NKLoginWebView::UrlRequested(const std::string& url)
{
    if (UrlMatchesRedirect(url, "/loaded"))
    {
        DidLoad();
        InjectJavascript(StringHelper::Format(
            "$.api.app.setAppInfo( [ %d , %d , \"%s\" ] );",
            appID, skuID, privateKey.c_str()));

        std::string providerData;
        for (const std::string& provider : providers)
        {
            if (!providerData.empty())
                providerData += ",";
            providerData += "\"" + provider + "\"";
        }

        InjectJavascript(StringHelper::Format(
            "$.api.app.setCurrentProviders( [ %s ] );",
            providerData.c_str()));

        if (!accessToken.token.empty() && accessToken.expires > 0)
        {
            InjectJavascript(StringHelper::Format(
                "$.api.app.userDetailsBackup(\"%s\", \"%s\");",
                responseUser.nkapiID.c_str(), responseUser.displayName.c_str()));
            InjectJavascript(StringHelper::Format(
                "$.api.app.setSession( \"%s\" );",
                accessToken.token.c_str()));
            InjectJavascript("$.api.user.login.sessionIsAvailable();");
            InjectJavascript("$.api.user.update.sessionIsAvailable();");
        }

        Show();
        return false;
    }

    if (UrlMatchesRedirect(url, "/restored"))
        return false;

    if (UrlMatchesRedirect(url, "/close"))
    {
        ManualClose();
        return false;
    }

    if (UrlMatchesRedirect(url, "/login") || UrlMatchesRedirect(url, "/onlylogin"))
    {
        URLParser urlParser(url);
        std::string queryValue;

        if (urlParser.GetValue("provider", queryValue))
        {
            eNKLoginService loginServiceType = GetLoginServiceEnumFromString(queryValue);
            if (loginServiceType != eNKLoginService::None)
            {
                ServiceActionRequested(loginServiceType, eNKServiceAction::Login);
                return false;
            }
        }
        else if (urlParser.GetValue("data", queryValue))
        {
            std::string newValue;
            bool newUser = urlParser.GetValue("new", newValue) && newValue == "true";
            LoginSucceeded(queryValue, newUser, url);
            return false;
        }

        LoginFailed();
        return false;
    }

    if (UrlMatchesRedirect(url, "/linking"))
    {
        URLParser urlParser(url);
        std::string provider;

        if (urlParser.GetValue("provider", provider))
        {
            eNKLoginService loginServiceType = GetLoginServiceEnumFromString(provider);
            if (loginServiceType != eNKLoginService::None)
            {
                ServiceActionRequested(loginServiceType, eNKServiceAction::Login);
                return false;
            }
        }

        LoginFailed();
        return false;
    }

    if (UrlMatchesRedirect(url, "/linked"))
    {
        URLParser urlParser(url);
        std::string data;

        if (urlParser.GetValue("data", data))
            UpdateUser(data);
        else
            LoginFailed();

        return false;
    }

    if (UrlMatchesRedirect(url, "/user/update"))
    {
        URLParser urlParser(url);
        std::string data;

        if (urlParser.GetValue("data", data))
        {
            UpdateUser(data);
            ManualClose();
        }
        else
        {
            LoginFailed();
        }

        return false;
    }

    if (UrlMatchesRedirect(url, "/logout"))
    {
        LogOut();
        ManualClose();
        LoginFailed();
        return false;
    }

    return true;
}
