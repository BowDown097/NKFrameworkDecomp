#pragma once
#include "Uncategorized/BehaviourTree.h"

class CBaseFileIO;

class BA_CacheSession : public BehaviourTree::Leaf {
public:
    static BA_CacheSession* Create(CBaseFileIO* fileIO);
    void Start(BehaviourTree::IBlackboard* blackboard) override;
    BehaviourTree::Action* clone() override;
private:
    CBaseFileIO* fileIO; // 0x10

    BA_CacheSession() = default;
    BA_CacheSession(CBaseFileIO* fileIO) : fileIO(fileIO) {}
};
