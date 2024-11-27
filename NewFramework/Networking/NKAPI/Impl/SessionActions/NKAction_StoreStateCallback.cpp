#include "NKAction_StoreStateCallback.h"

BA_StoreStateCallback* BA_StoreStateCallback::Create(boost::function1<void, const BehaviourTree::IBlackboard*> func)
{
    return new BA_StoreStateCallback(func);
}

void BA_StoreStateCallback::Start(BehaviourTree::IBlackboard* blackboard)
{
    func(blackboard);
    state = BehaviourTree::AState::Success;
}

BehaviourTree::Action* BA_StoreStateCallback::clone()
{
    BA_StoreStateCallback* out = new BA_StoreStateCallback;
    out->state = state;
    out->lastState = lastState;
    out->func = func;
    return out;
}
