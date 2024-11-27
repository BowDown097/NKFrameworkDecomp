#pragma once
#include "NKAction_SessionHttpRequest.h"
#include "Uncategorized/NKLoginService.h"

class BA_ServiceAction : public BA_HttpRequestAction {
public:
    eNKServiceAction GetAction() const;
    eNKLoginService GetService() const;
    std::string DebugString() override;
    std::string HttpCallbackKey() override;
    std::map<std::string, std::string> ConstructHeader() override;
    NKMessage ConstructMessage() override;
    std::string ConstructUrl() override;
    void ParseResponse(const NKMessageResponse& response) override;
    void Start(BehaviourTree::IBlackboard* blackboard) override;
    BehaviourTree::Action* clone() override;
    static BA_ServiceAction* Create(const eNKLoginService& loginServiceType, const eNKServiceAction& serviceAction);
private:
    I_NKLoginService* loginService{}; // 0x28
    eNKServiceAction serviceAction = eNKServiceAction::None; // 0x30
    eNKLoginService loginServiceType = eNKLoginService::None; // 0x34

    BA_ServiceAction() = default;
    BA_ServiceAction(const eNKLoginService& loginServiceType, const eNKServiceAction& serviceAction)
        : serviceAction(serviceAction), loginServiceType(loginServiceType) {}
};
