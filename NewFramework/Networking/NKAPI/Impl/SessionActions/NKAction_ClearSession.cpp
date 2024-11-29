#include "NKAction_ClearSession.h"
#include "Uncategorized/Blackboards.h"

void BA_ClearSession::Start(BehaviourTree::IBlackboard* blackboard) {
    NKSessionBlackboard* sessionBlackboard = dynamic_cast<NKSessionBlackboard*>(blackboard);
    sessionBlackboard->accessToken = NKAccessToken();
    sessionBlackboard->responseUser = NKResponseUser();
    state = BehaviourTree::AState::Success;
    sessionBlackboard->LogMsg("Clearing access token and user details");
}

BehaviourTree::Action* BA_ClearSession::clone() {
    return new BA_ClearSession(*this);
}
