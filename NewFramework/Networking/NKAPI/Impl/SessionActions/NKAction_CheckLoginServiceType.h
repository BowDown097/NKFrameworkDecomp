#pragma once
#include "Uncategorized/BehaviourTree.h"
#include "Uncategorized/NKLoginService.h"

class BA_CheckLoginServiceType : public BehaviourTree::Leaf {
public:
    static BA_CheckLoginServiceType* Create(const eNKLoginService& loginServiceType);
    std::string DebugString() override;
    void Start(BehaviourTree::IBlackboard* blackboard) override;
    BehaviourTree::Action* clone() override;
private:
    eNKLoginService loginServiceType = eNKLoginService::None; // 0x10

    BA_CheckLoginServiceType() = default;
    BA_CheckLoginServiceType(const eNKLoginService& loginServiceType)
        : loginServiceType(loginServiceType) {}
};
