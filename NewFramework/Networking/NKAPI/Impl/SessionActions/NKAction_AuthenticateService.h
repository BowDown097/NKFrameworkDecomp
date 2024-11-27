#pragma once
#include "Uncategorized/BehaviourTree.h"
#include "Uncategorized/NKLoginService.h"

class NKSessionBlackboard;

class BA_AuthenticateService : public BehaviourTree::Leaf, public I_NKLoginServiceObserver {
public:
    static BA_AuthenticateService* Create(const eNKLoginService& loginServiceType, const eNKServiceAuthMethod& authMethod);
    std::string DebugString() override;
    void Start(BehaviourTree::IBlackboard* blackboard) override;
    void Stop(BehaviourTree::IBlackboard* blackboard) override;
    void OnServiceLogin(const eNKServiceLoginResult& loginResult) override;
    BehaviourTree::Action* clone() override;
private:
    eNKLoginService loginServiceType = eNKLoginService::None; // 0x20
    eNKServiceAuthMethod authMethod{}; // 0x24
    NKSessionBlackboard* blackboard; // 0x28

    BA_AuthenticateService() = default;
    BA_AuthenticateService(const eNKLoginService& loginServiceType, const eNKServiceAuthMethod& authMethod)
        : loginServiceType(loginServiceType), authMethod(authMethod) {}
};
