#pragma once
#include "Uncategorized/BehaviourTree.h"

class BA_StoreStateCallback : public BehaviourTree::Leaf {
public:
    static BA_StoreStateCallback* Create(boost::function1<void, const BehaviourTree::IBlackboard*> func);
    void Start(BehaviourTree::IBlackboard* blackboard) override;
    BehaviourTree::Action* clone() override;
private:
    boost::function1<void, const BehaviourTree::IBlackboard*> func; // 0x10

    BA_StoreStateCallback() = default;
    BA_StoreStateCallback(boost::function1<void, const BehaviourTree::IBlackboard*> func)
        : func(func) {}
};
