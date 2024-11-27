#include "NKAction_ShowWebView.h"
#include "NewFramework/Analytics/DGAnalytics.h"
#include "NewFramework/BaseApplication/AppClass/NKSDKApp.h"
#include "NewFramework/Networking/NKAPI/Impl/NKSessionImpl.h"
#include "NewFramework/Networking/NKAPI/NKSignature.h"
#include "NewFramework/Utilities/Encoding.h"
#include "Uncategorized/Blackboards.h"
#include "Uncategorized/NKLoginQuery.h"

void BA_ShowWebView::Start(BehaviourTree::IBlackboard* blackboard) {
    state = BehaviourTree::AState::Running;
    timeout = 600.0f;
    webViewLoaded = false;

    this->blackboard = dynamic_cast<NKSessionBlackboard*>(blackboard);
    this->blackboard->LogMsg("Showing Web View..");
    this->blackboard->uiWasShown = true;

    this->blackboard->webView = boost::shared_ptr<C_NKLoginWebView>(
        Create_NKLoginWebview(
        this->blackboard->appID,
        this->blackboard->skuID,
        this->blackboard->privateKey,
        this->blackboard->accessToken,
        this->blackboard->responseUser,
        this->blackboard->providers));

    if (this->blackboard->webView) {
        this->blackboard->webView->AddListener(this);

        C_NKLoginQuery loginQuery(endpoint);
        loginQuery.AddField("platform", "steam");
        loginQuery.AddField("locale", appInterface->GetLanguageCode());
        loginQuery.AddField("version", appInterface->GetVersionCode());
        loginQuery.AddField("supports", this->blackboard->sessionImpl->GetSupportedInterfaceTags());
        loginQuery.AddField("agegate", appInterface->GetAgeGate());

        uint16_t focalX, focalY;
        this->blackboard->sessionImpl->GetScreenFocalPoint(focalX, focalY);

        this->blackboard->webView->ShowWithQuery(loginQuery, focalX, focalY, width, height);
    } else {
        state = BehaviourTree::AState::Failure;
    }
}

void BA_ShowWebView::Stop(BehaviourTree::IBlackboard* blackboard) {
    BehaviourTree::Action::Stop(blackboard);

    if (this->blackboard && this->blackboard->webView) {
        this->blackboard->webView->Update(0);
        this->blackboard->webView->RemoveListener(this);
    }
}

void BA_ShowWebView::WebView_DidLoad() {
    if (state != BehaviourTree::AState::Running) {
        return;
    }

    DGAnalyticsData analyticsData("NKWebviewLoad");
    analyticsData << "timePassed" << 600.0f - timeout;
    analyticsData << "url" << endpoint;
    analyticsData << "success" << true;
    DGAnalytics::Instance()->SendDataEvent(analyticsData, true, AnalyticsEventGroups::Group::Framework, 2);

    webViewLoaded = true;
}

void BA_ShowWebView::WebView_ManualClose() {
    if (state != BehaviourTree::AState::Running || !blackboard->field_C0) {
        return;
    }

    blackboard->LogMsg("Webview: Will CLose");
    blackboard->error = NKError(NKErrorType::VALUE9, "Webview Closed", "", "Webview: Manual Close");
    blackboard->LogMsg("Webview: Manual Close");

    state = BehaviourTree::AState::Failure;
}

void BA_ShowWebView::WebView_LoginFailed() {
    if (state != BehaviourTree::AState::Running) {
        return;
    }

    DGAnalyticsData analyticsData("NKWebviewLoad");
    analyticsData << "timePassed" << 600.0f - timeout;
    analyticsData << "url" << endpoint;
    analyticsData << "success" << false;
    DGAnalytics::Instance()->SendDataEvent(analyticsData, true, AnalyticsEventGroups::Group::Framework, 2);

    blackboard->error = NKError(NKErrorType::VALUE2, "Webview Error", "", "Webview: Login Failed");
    blackboard->LogMsg("Webview: Login Failed");

    state = BehaviourTree::AState::Failure;
}

void BA_ShowWebView::Update(BehaviourTree::IBlackboard*, const float& elapsed) {
    if (state != BehaviourTree::AState::Running) {
        return;
    }

    if (this->blackboard->webView) {
        this->blackboard->webView->Update(elapsed);
    }

    if (!webViewLoaded) {
        timeout -= elapsed;
        if (timeout <= 0.0f) {
            LOG_ERROR("BA_ShowWebView: hit timeout fail-safe (%fs)", 600.0); ENFORCE_LINE(111);
            this->blackboard->error = NKError(NKErrorType::VALUE2, "Webview Error", "", "hit timeout fail-safe");
            this->blackboard->LogMsg("hit timeout fail-safe");
            state = BehaviourTree::AState::Failure;
        }
    }
}

void BA_ShowWebView::WebView_LoginSucceeded(const std::string& sessionToken, bool newUser, std::string url) {
    if (state != BehaviourTree::AState::Running) {
        return;
    }

    blackboard->LogMsg("Webview: Reported a successful login");

    std::string sessionTokenDecoded;
    if (!base64_to_string(sessionToken, sessionTokenDecoded)) {
        std::string errorInfo = "Couldn't decode session token";

        DGAnalyticsData analyticsData("NKNetworkingError");
        analyticsData.AddPair("type", sNKError_HttpParse);
        analyticsData.AddPair("info", errorInfo);
        analyticsData.AddPair("url", url);
        DGAnalytics::Instance()->SendDataEvent(analyticsData, true, AnalyticsEventGroups::Group::Framework, 2);

        blackboard->LogMsg("Webview: " + errorInfo);
        blackboard->error = NKError(NKErrorType::VALUE4, "Http Response parse error", "", errorInfo);

        state = BehaviourTree::AState::Failure;
        return;
    }

    NKMessageResponse messageResponse;
    if (NKJSON::TryParse<NKMessageResponse>(messageResponse, sessionTokenDecoded) && messageResponse.error.type.empty()) {
        if (!NKSignature::Verify("", blackboard->privateKey, messageResponse.data, messageResponse.sig)) {
            std::string errorInfo = "Couldn't verify response signature: " + messageResponse.sig;

            DGAnalyticsData analyticsData("NKNetworkingError");
            analyticsData.AddPair("type", sNKError_HttpSecurity);
            analyticsData.AddPair("info", "WebviewLoginSuccess: " + errorInfo);
            analyticsData.AddPair("url", url);
            DGAnalytics::Instance()->SendDataEvent(analyticsData, true, AnalyticsEventGroups::Group::Framework, 2);

            blackboard->LogMsg("Webview: " + errorInfo);
            blackboard->error = NKError(NKErrorType::VALUE5, "Security Error", "", "Couldn't verify response signature");
            state = BehaviourTree::AState::Failure;
            return;
        }

        NKResponseLogin responseLogin;
        if (!NKJSON::TryParse<NKResponseLogin>(responseLogin, messageResponse.data)) {
            throw std::runtime_error("Failed to parse NKResponseLogin");
        }

        blackboard->accessToken = NKAccessToken(responseLogin.session);
        blackboard->responseUser = responseLogin.user;
        blackboard->newUser = newUser;
        blackboard->loginServiceFromToken = eNKLoginService::Email;
        state = BehaviourTree::AState::Success;
        return;
    }

    blackboard->error = NKError(NKErrorType::VALUE3, "", "", "");
    if (messageResponse.error.type.empty()) {
        std::string errorInfo = "Couldn't parse response";
        blackboard->LogMsg("Webview: " + errorInfo);

        DGAnalyticsData analyticsData("NKNetworkingError");
        analyticsData.AddPair("type", sNKError_HttpParse);
        analyticsData.AddPair("info", "WebviewLoginSuccess: " + errorInfo);
        analyticsData.AddPair("url", url);
        DGAnalytics::Instance()->SendDataEvent(analyticsData, true, AnalyticsEventGroups::Group::Framework, 2);
    } else {
        std::string errorInfo = "Server reported an error: \n\t Type: " + messageResponse.error.type;
        errorInfo += "\n\tReason: " + messageResponse.error.details.reason;
        errorInfo += "\n\tFix: " + messageResponse.error.details.fix;
        blackboard->LogMsg("Webview: " + errorInfo);

        DGAnalyticsData analyticsData("NKNetworkingError");
        analyticsData.AddPair("type", sNKError_HttpLink);
        analyticsData.AddPair("info", "WebviewLoginSuccess: " + errorInfo);
        analyticsData.AddPair("url", url);
        DGAnalytics::Instance()->SendDataEvent(analyticsData, true, AnalyticsEventGroups::Group::Framework, 2);
    }

    state = BehaviourTree::AState::Failure;
}

void BA_ShowWebView::WebView_UpdateUser(const std::string& sessionToken) {
    if (state != BehaviourTree::AState::Running) {
        return;
    }

    blackboard->LogMsg("Webview: Requested User details be updated");

    std::string sessionTokenDecoded;
    if (!base64_to_string(sessionToken, sessionTokenDecoded)) {
        blackboard->LogMsg("Webview: Couldn't decode session token");
        return;
    }

    NKResponseUserCurrent userCurrent;
    if (NKJSON::TryParse<NKResponseUserCurrent>(userCurrent, sessionTokenDecoded)) {
        blackboard->responseUser = userCurrent.user;
    } else {
        throw std::runtime_error("Failed to parse NKResponseUserCurrent");
    }
}

BehaviourTree::Action* BA_ShowWebView::clone() {
    return new BA_ShowWebView(*this);
}

BA_ShowWebView* BA_ShowWebView::Create(I_NKSDKApp* appInterface, const std::string& endpoint,
                                       const int& width, const int& height) {
    return new BA_ShowWebView(appInterface, endpoint, width, height);
}
