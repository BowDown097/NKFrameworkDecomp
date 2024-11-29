#pragma once
#include "NKAction_SessionHttpRequest.h"

struct BA_GetUser : BA_HttpRequestAction {
    std::string HttpCallbackKey() override;
    std::map<std::string, std::string> ConstructHeader() override;
    NKMessage ConstructMessage() override;
    std::string ConstructUrl() override;
    void ParseResponse(const NKMessageResponse& response) override;
    void Start(BehaviourTree::IBlackboard* blackboard) override;
    BehaviourTree::Action* clone() override;
};
