#include "NKAction_WebViewErrorToolTip.h"
#include "NewFramework/Utilities/StringHelper.h"
#include "Uncategorized/Blackboards.h"
#include "Uncategorized/NKLoginWebview.h"

void BA_WebViewErrorToolTip::Start(BehaviourTree::IBlackboard* blackboard) {
    state = BehaviourTree::AState::Running;

    NKSessionBlackboard* sessionBlackboard = dynamic_cast<NKSessionBlackboard*>(blackboard);
    std::string serviceKey;

    switch (sessionBlackboard->loginService) {
        case eNKLoginService::GameCenter:
            serviceKey = "optional_login_ios";
            break;
        case eNKLoginService::Facebook:
            serviceKey = "login_facebook";
            break;
        case eNKLoginService::GooglePlay:
            serviceKey = "optional_login_android_with_gp";
            break;
        case eNKLoginService::Steam:
            serviceKey = "optional_login_steam";
            break;
        case eNKLoginService::Apple:
            serviceKey = "optional_login_apple";
            break;
        default:
            break;
    }

    std::string code = StringHelper::Format("$.api.user.login.showToolTipWithText(\"#%s\", \"%s\", \"%s\");",
        serviceKey.c_str(), sessionBlackboard->error.reason.c_str(), sessionBlackboard->error.fix.c_str());
    sessionBlackboard->LogMsg("Showing tooltip using JavaScript: " + code);

    if (!sessionBlackboard->webView) {
        std::string errorInfo = "No webview to inject JavaScript into?";
        sessionBlackboard->error = NKError(NKErrorType::VALUE7, "Webview Javascript Inject Error", "", errorInfo);
        sessionBlackboard->LogMsg(errorInfo);
        state = BehaviourTree::AState::Failure;
        return;
    }

    sessionBlackboard->webView->InjectJavascript(code);
    state = BehaviourTree::AState::Success;
}

BehaviourTree::Action* BA_WebViewErrorToolTip::clone() {
    return new BA_WebViewErrorToolTip(*this);
}

BA_WebViewErrorToolTip* BA_WebViewErrorToolTip::Create() {
    return new BA_WebViewErrorToolTip;
}
