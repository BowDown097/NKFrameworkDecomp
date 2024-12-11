#pragma once
#include "Uncategorized/BehaviourTree.h"

enum class NKConflictResolutionType;

class BA_SetAppendFileSavePath : public BehaviourTree::Leaf {
public:
    BehaviourTree::Action* clone() override;
    static BA_SetAppendFileSavePath* Create(const std::string& postfix);
    void Start(BehaviourTree::IBlackboard* blackboard) override;
private:
    std::string postfix; // 0x10

    BA_SetAppendFileSavePath() = default;
    BA_SetAppendFileSavePath(const std::string& postfix)
        : postfix(postfix) {}
};

class BA_CheckResolutionMode : public BehaviourTree::Leaf {
public:
    BehaviourTree::Action* clone() override;
    static BA_CheckResolutionMode* Create(NKConflictResolutionType conflictResolutionType);
    void Start(BehaviourTree::IBlackboard* blackboard) override;
private:
    NKConflictResolutionType conflictResolutionType{}; // 0x10

    BA_CheckResolutionMode() = default;
    BA_CheckResolutionMode(NKConflictResolutionType conflictResolutionType)
        : conflictResolutionType(conflictResolutionType) {}
};

class BA_CheckFileExists : public BehaviourTree::Leaf {
public:
    BehaviourTree::Action* clone() override;
    static BA_CheckFileExists* Create(bool checkSuccess);
    void Start(BehaviourTree::IBlackboard* blackboard) override;
private:
    bool checkSuccess = true;
    BA_CheckFileExists(bool checkSuccess) : checkSuccess(checkSuccess) {}
};
