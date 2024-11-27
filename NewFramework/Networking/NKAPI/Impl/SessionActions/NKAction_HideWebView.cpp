#include "NKAction_HideWebView.h"
#include "Uncategorized/Blackboards.h"

void BA_HideWebView::Start(BehaviourTree::IBlackboard* blackboard) {
    state = BehaviourTree::AState::Running;

    this->blackboard = dynamic_cast<NKSessionBlackboard*>(blackboard);
    this->blackboard->LogMsg("Hiding WebView..");

    if (!this->blackboard->webView) {
        this->blackboard->LogMsg("No webview currently exists!");
        state = BehaviourTree::AState::Failure;
        return;
    }

    this->blackboard->webView->Update(0);
    this->blackboard->webView->AddListener(this);
    this->blackboard->webView->Hide();
}

void BA_HideWebView::Update(BehaviourTree::IBlackboard*, const float& elapsed) {
    if (state == BehaviourTree::AState::Running && this->blackboard->webView) {
        this->blackboard->webView->Update(elapsed);
    }
}

void BA_HideWebView::Stop(BehaviourTree::IBlackboard* blackboard) {
    BehaviourTree::Action::Stop(blackboard);

    if (this->blackboard && this->blackboard->webView) {
        this->blackboard->webView->RemoveListener(this);
    }
}

void BA_HideWebView::WebView_DidHide() {
    if (state == BehaviourTree::AState::Running) {
        state = BehaviourTree::AState::Success;
    }
}

BehaviourTree::Action* BA_HideWebView::clone() {
    return new BA_HideWebView(*this);
}

BA_HideWebView* BA_HideWebView::Create() {
    return new BA_HideWebView;
}
