#pragma once
#include "Uncategorized/BehaviourTree.h"

struct NKError;

class BA_ReportErrorCallback : public BehaviourTree::Leaf {
public:
    static BA_ReportErrorCallback* Create(boost::function1<void, const NKError&> func);
    void Start(BehaviourTree::IBlackboard* blackboard) override;
    BehaviourTree::Action* clone() override;
private:
    boost::function1<void, const NKError&> func; // 0x10

    BA_ReportErrorCallback() = default;
    BA_ReportErrorCallback(boost::function1<void, const NKError&> func)
        : func(func) {}
};
