#pragma once
#include "Uncategorized/BehaviourTree.h"

class CBaseFileIO;
struct SNKFileClientBlackboard;

enum class NKConflictResolutionType { None, Delete, Recreate };

using NKFileClientConflictCallback_t = boost::function4<void, const std::string&, const std::string&, const std::string&,
                                                        boost::function1<void, NKConflictResolutionType>>;

class BA_ConflictedFileCallback : public BehaviourTree::Leaf {
public:
    BehaviourTree::Action* clone() override;
    static BA_ConflictedFileCallback* Create(NKFileClientConflictCallback_t callback);
    void Start(BehaviourTree::IBlackboard* blackboard) override;
    void ResolveCallback(NKConflictResolutionType conflictResolutionType);
private:
    SNKFileClientBlackboard* blackboard{}; // 0x10
    NKFileClientConflictCallback_t callback; // 0x18

    BA_ConflictedFileCallback(NKFileClientConflictCallback_t callback)
        : callback(callback) {}
};

class BA_ResolveConflict : public BehaviourTree::Leaf {
public:
    BehaviourTree::Action* clone() override;
    static BA_ResolveConflict* Create(CBaseFileIO* fileIO);
    void Start(BehaviourTree::IBlackboard* blackboard) override;
private:
    SNKFileClientBlackboard* blackboard{}; // 0x10
    CBaseFileIO* fileIO; // 0x18

    BA_ResolveConflict(CBaseFileIO* fileIO) : fileIO(fileIO) {}
};
