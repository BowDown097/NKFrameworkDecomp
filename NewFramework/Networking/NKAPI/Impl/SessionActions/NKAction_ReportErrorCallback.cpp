#include "NKAction_ReportErrorCallback.h"
#include "Uncategorized/Blackboards.h"

BA_ReportErrorCallback* BA_ReportErrorCallback::Create(boost::function1<void, const NKError&> func) {
    return new BA_ReportErrorCallback(func);
}

void BA_ReportErrorCallback::Start(BehaviourTree::IBlackboard* blackboard) {
    NKSessionBlackboard* sessionBlackboard = dynamic_cast<NKSessionBlackboard*>(blackboard);
    func(sessionBlackboard->error);
    state = BehaviourTree::AState::Success;
}

BehaviourTree::Action* BA_ReportErrorCallback::clone() {
    return new BA_ReportErrorCallback(*this);
}
