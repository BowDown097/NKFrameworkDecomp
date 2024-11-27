#pragma once
#include "Uncategorized/BehaviourTree.h"

class BA_CheckErrorType : public BehaviourTree::Leaf {
public:
    static BA_CheckErrorType* Create(const std::string& errorType);
    std::string DebugString() override;
    void Start(BehaviourTree::IBlackboard* blackboard) override;
    BehaviourTree::Action* clone() override;
private:
    std::string errorType; // 0x10

    BA_CheckErrorType() = default;
    BA_CheckErrorType(const std::string& errorType) : errorType(errorType) {}
};
