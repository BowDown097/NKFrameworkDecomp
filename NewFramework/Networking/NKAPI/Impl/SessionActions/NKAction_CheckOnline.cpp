#include "NKAction_CheckOnline.h"
#include "NewFramework/BaseApplication/AppClass/NKSDKApp.h"
#include "Uncategorized/Blackboards.h"

void BA_CheckOnline::Start(BehaviourTree::IBlackboard* blackboard) {
    NKSessionBlackboard* sessionBlackboard = dynamic_cast<NKSessionBlackboard*>(blackboard);
    state = appInterface && appInterface->IsOnline() ? BehaviourTree::AState::Success : BehaviourTree::AState::Failure;

    std::string message = "Checking if app is online: ";
    message += state == BehaviourTree::AState::Success ? "True" : "False";
    sessionBlackboard->LogMsg(message);
}

BehaviourTree::Action* BA_CheckOnline::clone() {
    return new BA_CheckOnline(*this);
}

BA_CheckOnline* BA_CheckOnline::Create(I_NKSDKApp* appInterface) {
    return new BA_CheckOnline(appInterface);
}
