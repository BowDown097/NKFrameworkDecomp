#include "NKAction_CheckErrorType.h"
#include "Uncategorized/Blackboards.h"

BA_CheckErrorType* BA_CheckErrorType::Create(const std::string& errorType) {
    return new BA_CheckErrorType(errorType);
}

std::string BA_CheckErrorType::DebugString() {
    return errorType;
}

void BA_CheckErrorType::Start(BehaviourTree::IBlackboard* blackboard) {
    NKSessionBlackboard* sessionBlackboard = dynamic_cast<NKSessionBlackboard*>(blackboard);
    state = sessionBlackboard->error.typeStr == errorType ? BehaviourTree::AState::Success : BehaviourTree::AState::Failure;

    std::string message = "Checking error type == " + errorType + ".. ";
    message += state == BehaviourTree::AState::Success ? "True" : "False";
    sessionBlackboard->LogMsg(message);
}

BehaviourTree::Action* BA_CheckErrorType::clone() {
    return new BA_CheckErrorType(*this);
}
