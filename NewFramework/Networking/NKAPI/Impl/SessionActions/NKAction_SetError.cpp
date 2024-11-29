#include "NKAction_SetError.h"
#include "Uncategorized/Blackboards.h"

std::string BA_SetError::DebugString() {
    return error.typeStr;
}

void BA_SetError::Start(BehaviourTree::IBlackboard* blackboard) {
    NKSessionBlackboard* sessionBlackboard = dynamic_cast<NKSessionBlackboard*>(blackboard);
    sessionBlackboard->error = error;
    state = BehaviourTree::AState::Success;
}

BehaviourTree::Action* BA_SetError::clone() {
    return new BA_SetError(*this);
}

BA_SetError* BA_SetError::Create(const NKError& error) {
    return new BA_SetError(error);
}
