#pragma once
#include "NewFramework/Networking/NKAPI/NKError.h"
#include "Uncategorized/BehaviourTree.h"

class BA_SetError : public BehaviourTree::Leaf {
public:
    std::string DebugString() override;
    void Start(BehaviourTree::IBlackboard* blackboard) override;
    BehaviourTree::Action* clone() override;
    static BA_SetError* Create(const NKError& error);
private:
    NKError error; // 0x10

    BA_SetError() = default;
    BA_SetError(const NKError& error) : error(error) {}
};
