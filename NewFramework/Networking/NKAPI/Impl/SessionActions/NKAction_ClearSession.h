#pragma once
#include "Uncategorized/BehaviourTree.h"

struct BA_ClearSession : BehaviourTree::Leaf {
    void Start(BehaviourTree::IBlackboard* blackboard) override;
    BehaviourTree::Action* clone() override;
};
