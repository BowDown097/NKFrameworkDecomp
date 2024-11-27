#pragma once
#include "Uncategorized/BehaviourTree.h"
#include "Uncategorized/NKLoginWebview.h"

struct I_NKSDKApp;
class NKSessionBlackboard;

class BA_ShowWebView : public BehaviourTree::Leaf, protected I_NKLoginWebviewListener {
public:
    void Start(BehaviourTree::IBlackboard* blackboard) override;
    void Update(BehaviourTree::IBlackboard* blackboard, const float& elapsed) override;
    void Stop(BehaviourTree::IBlackboard* blackboard) override;
    void WebView_DidLoad() override;
    void WebView_ManualClose() override;
    void WebView_LoginFailed() override;
    void WebView_LoginSucceeded(const std::string& sessionToken, bool newUser, std::string url) override;
    void WebView_UpdateUser(const std::string& sessionToken) override;
    BehaviourTree::Action* clone() override;
    static BA_ShowWebView* Create(I_NKSDKApp* appInterface, const std::string& endpoint, const int& width, const int& height);
private:
    I_NKSDKApp* appInterface{}; // 0x18
    std::string endpoint; // 0x20
    int width{}; // 0x38
    int height{}; // 0x3C
    NKSessionBlackboard* blackboard{}; // 0x40
    float timeout; // 0x48
    bool webViewLoaded; // 0x4C

    BA_ShowWebView() = default;
    BA_ShowWebView(I_NKSDKApp* appInterface, const std::string& endpoint, const int& width, const int& height)
        : appInterface(appInterface), endpoint(endpoint), width(width), height(height) {}
};
