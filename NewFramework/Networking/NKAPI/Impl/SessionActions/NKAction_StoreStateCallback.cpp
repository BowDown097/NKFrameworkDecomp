#include "NKAction_StoreStateCallback.h"

BA_StoreStateCallback* BA_StoreStateCallback::Create(boost::function1<void, const BehaviourTree::IBlackboard*> func) {
    return new BA_StoreStateCallback(func);
}

void BA_StoreStateCallback::Start(BehaviourTree::IBlackboard* blackboard) {
    func(blackboard);
    state = BehaviourTree::AState::Success;
}

BehaviourTree::Action* BA_StoreStateCallback::clone() {
    return new BA_StoreStateCallback(*this);
}
