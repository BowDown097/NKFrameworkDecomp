#pragma once
#include "Uncategorized/BehaviourTree.h"

class BA_WebViewErrorToolTip : public BehaviourTree::Leaf {
public:
    void Start(BehaviourTree::IBlackboard* blackboard) override;
    BehaviourTree::Action* clone() override;
    static BA_WebViewErrorToolTip* Create();
private:
    BA_WebViewErrorToolTip() = default;
};
