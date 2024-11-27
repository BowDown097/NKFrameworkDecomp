#pragma once
#include "Uncategorized/BehaviourTree.h"
#include "Uncategorized/NKLoginWebview.h"

class NKSessionBlackboard;

class BA_WebviewRequestListener : public BehaviourTree::Leaf, protected I_NKLoginWebviewListener {
public:
    static BA_WebviewRequestListener* Create();
    void Start(BehaviourTree::IBlackboard* blackboard) override;
    void Stop(BehaviourTree::IBlackboard* blackboard) override;
    void WebView_ServiceActionRequested(const eNKLoginService& loginServiceType, const eNKServiceAction& serviceAction) override;
    BehaviourTree::Action* clone() override;
private:
    NKSessionBlackboard* blackboard{}; // 0x18
    BA_WebviewRequestListener() = default;
};
