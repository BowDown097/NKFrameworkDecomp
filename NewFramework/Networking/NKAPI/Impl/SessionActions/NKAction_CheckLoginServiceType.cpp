#include "NKAction_CheckLoginServiceType.h"
#include "Uncategorized/Blackboards.h"

BA_CheckLoginServiceType* BA_CheckLoginServiceType::Create(const eNKLoginService& loginServiceType) {
    return new BA_CheckLoginServiceType(loginServiceType);
}

std::string BA_CheckLoginServiceType::DebugString() {
    return GetLoginServiceStringFromEnum(loginServiceType);
}

void BA_CheckLoginServiceType::Start(BehaviourTree::IBlackboard* blackboard) {
    NKSessionBlackboard* sessionBlackboard = dynamic_cast<NKSessionBlackboard*>(blackboard);
    state = sessionBlackboard->loginService == loginServiceType
        ? BehaviourTree::AState::Success : BehaviourTree::AState::Failure;

    std::string message = "Checking service type == " + GetLoginServiceStringFromEnum(loginServiceType) + "... ";
    message += state == BehaviourTree::AState::Success ? "True" : "False";
    sessionBlackboard->LogMsg(message);
}

BehaviourTree::Action* BA_CheckLoginServiceType::clone() {
    return new BA_CheckLoginServiceType(*this);
}
