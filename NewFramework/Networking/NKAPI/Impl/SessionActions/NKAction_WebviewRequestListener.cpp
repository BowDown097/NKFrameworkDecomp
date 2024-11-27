#include "NKAction_WebviewRequestListener.h"
#include "Uncategorized/Blackboards.h"

BA_WebviewRequestListener* BA_WebviewRequestListener::Create() {
    return new BA_WebviewRequestListener;
}

void BA_WebviewRequestListener::Start(BehaviourTree::IBlackboard* blackboard) {
    state = BehaviourTree::AState::Running;

    this->blackboard = dynamic_cast<NKSessionBlackboard*>(blackboard);
    this->blackboard->LogMsg("Listening for webview app requests..");

    this->blackboard->serviceAction = eNKServiceAction::None;
    this->blackboard->loginService = eNKLoginService::None;

    if (!this->blackboard->webView) {
        std::string errorInfo = "No webview to listen to?";
        this->blackboard->error = NKError(NKErrorType::VALUE7, "Webview Request Listener Error", "", errorInfo);
        this->blackboard->LogMsg(errorInfo);
        state = BehaviourTree::AState::Failure;
        return;
    }

    this->blackboard->webView->AddListener(this);
}

void BA_WebviewRequestListener::Stop(BehaviourTree::IBlackboard* blackboard)
{
    BehaviourTree::Action::Stop(blackboard);

    if (this->blackboard && this->blackboard->webView) {
        this->blackboard->webView->RemoveListener(this);
    }
}

void BA_WebviewRequestListener::WebView_ServiceActionRequested(
    const eNKLoginService& loginServiceType, const eNKServiceAction& serviceAction)
{
    if (state != BehaviourTree::AState::Running) {
        return;
    }

    this->blackboard->LogMsg("Webview Listener: " + GetLoginServiceStringFromEnum(loginServiceType) +
                             " " + GetServiceActionStringFromEnum(serviceAction) + " requested.");
    this->blackboard->serviceAction = serviceAction;
    this->blackboard->loginService = loginServiceType;

    state = BehaviourTree::AState::Success;
}

BehaviourTree::Action* BA_WebviewRequestListener::clone() {
    return new BA_WebviewRequestListener(*this);
}
