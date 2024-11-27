#pragma once
#include "Uncategorized/BehaviourTree.h"
#include "Uncategorized/NKLoginService.h"

class BA_IsServiceAuthenticated : public BehaviourTree::Leaf {
public:
    static BA_IsServiceAuthenticated* Create(const eNKLoginService& loginServiceType);
    std::string DebugString() override;
    void Start(BehaviourTree::IBlackboard* blackboard) override;
    BehaviourTree::Action* clone() override;
private:
    I_NKLoginService* loginService; // 0x10
    eNKLoginService loginServiceType = eNKLoginService::None; // 0x18

    BA_IsServiceAuthenticated() = default;
    BA_IsServiceAuthenticated(const eNKLoginService& loginServiceType)
        : loginServiceType(loginServiceType) {}
};
