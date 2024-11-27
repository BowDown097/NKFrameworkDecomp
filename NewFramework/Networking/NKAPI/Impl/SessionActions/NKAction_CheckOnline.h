#pragma once
#include "Uncategorized/BehaviourTree.h"

struct I_NKSDKApp;

class BA_CheckOnline : public BehaviourTree::Leaf {
public:
    void Start(BehaviourTree::IBlackboard* blackboard) override;
    BehaviourTree::Action* clone() override;
    static BA_CheckOnline* Create(I_NKSDKApp* appInterface);
private:
    I_NKSDKApp* appInterface; // 0x10

    BA_CheckOnline() = default;
    BA_CheckOnline(I_NKSDKApp* appInterface) : appInterface(appInterface) {}
};
