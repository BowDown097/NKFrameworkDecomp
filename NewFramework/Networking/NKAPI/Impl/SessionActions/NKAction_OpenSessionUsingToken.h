#pragma once
#include "Uncategorized/BehaviourTree.h"

class CBaseFileIO;
class IFile;
class NKSessionBlackboard;

class BA_OpenSessionUsingToken : public BehaviourTree::Leaf {
public:
    const bool ReadTokenFile(IFile* file, NKSessionBlackboard* blackboard);
    void Start(BehaviourTree::IBlackboard* blackboard) override;
    BehaviourTree::Action* clone() override;
    static BA_OpenSessionUsingToken* Create(CBaseFileIO* fileIO);
private:
    CBaseFileIO* fileIO; // 0x10

    BA_OpenSessionUsingToken() = default;
    BA_OpenSessionUsingToken(CBaseFileIO* fileIO) : fileIO(fileIO) {}
};
