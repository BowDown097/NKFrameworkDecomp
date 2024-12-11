#include "NKAction_AppendFileSavePath.h"
#include "NewFramework/Networking/NKAPI/Impl/NKFileClientImpl.h"
#include "Uncategorized/Blackboards.h"

BehaviourTree::Action* BA_SetAppendFileSavePath::clone() {
    return new BA_SetAppendFileSavePath(*this);
}

BA_SetAppendFileSavePath* BA_SetAppendFileSavePath::Create(const std::string& postfix) {
    return new BA_SetAppendFileSavePath(postfix);
}

void BA_SetAppendFileSavePath::Start(BehaviourTree::IBlackboard* blackboard) {
    state = BehaviourTree::AState::Running;

    SNKFileClientBlackboard* fileClientBlackboard = dynamic_cast<SNKFileClientBlackboard*>(blackboard);
    fileClientBlackboard->postfix = postfix;

    state = BehaviourTree::AState::Success;

    fileClientBlackboard->fileClientImpl->ProcessBHTree(fileClientBlackboard->action.lock(), fileClientBlackboard);
}

BehaviourTree::Action* BA_CheckResolutionMode::clone() {
    return new BA_CheckResolutionMode(*this);
}

BA_CheckResolutionMode* BA_CheckResolutionMode::Create(NKConflictResolutionType conflictResolutionType) {
    return new BA_CheckResolutionMode(conflictResolutionType);
}

void BA_CheckResolutionMode::Start(BehaviourTree::IBlackboard* blackboard) {
    state = BehaviourTree::AState::Running;
    SNKFileClientBlackboard* fileClientBlackboard = dynamic_cast<SNKFileClientBlackboard*>(blackboard);
    state = fileClientBlackboard->conflictResolutionType == conflictResolutionType
        ? BehaviourTree::AState::Success : BehaviourTree::AState::Failure;
}

BehaviourTree::Action* BA_CheckFileExists::clone() {
    return new BA_CheckFileExists(*this);
}

BA_CheckFileExists* BA_CheckFileExists::Create(bool checkSuccess) {
    return new BA_CheckFileExists(checkSuccess);
}

void BA_CheckFileExists::Start(BehaviourTree::IBlackboard* blackboard) {
    state = BehaviourTree::AState::Running;
    SNKFileClientBlackboard* fileClientBlackboard = dynamic_cast<SNKFileClientBlackboard*>(blackboard);
    state = fileClientBlackboard->file.storage.success == checkSuccess
        ? BehaviourTree::AState::Success : BehaviourTree::AState::Failure;
}
