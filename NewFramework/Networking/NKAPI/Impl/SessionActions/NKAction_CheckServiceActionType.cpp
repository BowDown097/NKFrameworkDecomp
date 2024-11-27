#include "NKAction_CheckServiceActionType.h"
#include "Uncategorized/Blackboards.h"

BA_CheckServiceActionType* BA_CheckServiceActionType::Create(const eNKServiceAction& serviceAction) {
    return new BA_CheckServiceActionType(serviceAction);
}

std::string BA_CheckServiceActionType::DebugString() {
    return GetServiceActionStringFromEnum(serviceAction);
}

void BA_CheckServiceActionType::Start(BehaviourTree::IBlackboard* blackboard) {
    NKSessionBlackboard* sessionBlackboard = dynamic_cast<NKSessionBlackboard*>(blackboard);
    state = sessionBlackboard->serviceAction == serviceAction
        ? BehaviourTree::AState::Success : BehaviourTree::AState::Failure;

    std::string message = "Checking action type == " + GetServiceActionStringFromEnum(serviceAction) + "... ";
    message += state == BehaviourTree::AState::Success ? "True" : "False";
    sessionBlackboard->LogMsg(message);
}

BehaviourTree::Action* BA_CheckServiceActionType::clone() {
    return new BA_CheckServiceActionType(*this);
}
