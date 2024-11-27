#pragma once
#include "Uncategorized/BehaviourTree.h"
#include "Uncategorized/NKLoginService.h"

class BA_CheckServiceActionType : public BehaviourTree::Leaf {
public:
    static BA_CheckServiceActionType* Create(const eNKServiceAction& serviceAction);
    std::string DebugString() override;
    void Start(BehaviourTree::IBlackboard* blackboard) override;
    BehaviourTree::Action* clone() override;
private:
    eNKServiceAction serviceAction = eNKServiceAction::None; // 0x10

    BA_CheckServiceActionType() = default;
    BA_CheckServiceActionType(const eNKServiceAction& serviceAction)
        : serviceAction(serviceAction) {}
};
