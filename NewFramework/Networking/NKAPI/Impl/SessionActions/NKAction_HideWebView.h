#pragma once
#include "Uncategorized/BehaviourTree.h"
#include "Uncategorized/NKLoginWebview.h"

class NKSessionBlackboard;

class BA_HideWebView : public BehaviourTree::Leaf, protected I_NKLoginWebviewListener {
public:
    void Start(BehaviourTree::IBlackboard* blackboard) override;
    void Update(BehaviourTree::IBlackboard* blackboard, const float& elapsed) override;
    void Stop(BehaviourTree::IBlackboard* blackboard) override;
    void WebView_DidHide() override;
    BehaviourTree::Action* clone() override;
    static BA_HideWebView* Create();
private:
    NKSessionBlackboard* blackboard{}; // 0x18
    BA_HideWebView() = default;
};
