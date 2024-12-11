#include "NKAction_FileConflict.h"
#include "NewFramework/Networking/NKAPI/Impl/NKFileClientImpl.h"
#include "NewFramework/Utilities/Encoding.h"
#include "Uncategorized/Blackboards.h"
#include <boost/bind/bind.hpp>

BehaviourTree::Action* BA_ConflictedFileCallback::clone() {
    return new BA_ConflictedFileCallback(*this);
}

BA_ConflictedFileCallback* BA_ConflictedFileCallback::Create(NKFileClientConflictCallback_t callback) {
    return new BA_ConflictedFileCallback(callback);
}

void BA_ConflictedFileCallback::Start(BehaviourTree::IBlackboard* blackboard) {
    state = BehaviourTree::AState::Running;
    this->blackboard = dynamic_cast<SNKFileClientBlackboard*>(blackboard);

    if (!callback.empty()) {
        static std::string s_sMinePostfix = ".mine";

        std::string mineFile = this->blackboard->fileName + s_sMinePostfix;
        std::string blackboardFile = this->blackboard->fileName + this->blackboard->postfix;
        auto resolveCallback = boost::bind(&BA_ConflictedFileCallback::ResolveCallback, this, boost::placeholders::_1);

        callback(this->blackboard->savePath, mineFile, blackboardFile, resolveCallback);
    } else {
        this->blackboard->error = NKError(NKErrorType::VALUE7, "BA_ConflictedFileCallback::m_Callback should not be null", "", "");
        state = BehaviourTree::AState::Failure;
    }

    this->blackboard->fileClientImpl->ProcessBHTree(this->blackboard->action.lock(), this->blackboard);
}

void BA_ConflictedFileCallback::ResolveCallback(NKConflictResolutionType conflictResolutionType) {
    blackboard->conflictResolutionType = conflictResolutionType;
    state = BehaviourTree::AState::Success;
    blackboard->fileClientImpl->ProcessBHTree(blackboard->action.lock(), blackboard);
}

BehaviourTree::Action* BA_ResolveConflict::clone() {
    return new BA_ResolveConflict(*this);
}

BA_ResolveConflict* BA_ResolveConflict::Create(CBaseFileIO* fileIO) {
    return new BA_ResolveConflict(fileIO);
}

void BA_ResolveConflict::Start(BehaviourTree::IBlackboard* blackboard) {
    state = BehaviourTree::AState::Running;
    this->blackboard = dynamic_cast<SNKFileClientBlackboard*>(blackboard);

    std::string filePath = this->blackboard->savePath + this->blackboard->fileName;
    switch (this->blackboard->conflictResolutionType) {
        case NKConflictResolutionType::Delete:
            state = BehaviourTree::AState::Success;
            if (fileIO->FileExists(filePath + this->blackboard->postfix, fileIO->cachePolicy)) {
                fileIO->FileDelete(filePath + this->blackboard->postfix, fileIO->cachePolicy);
            }
            break;
        case NKConflictResolutionType::Recreate:
            if (fileIO->FileExists(filePath + this->blackboard->postfix, fileIO->cachePolicy) &&
                fileIO->FileRename(filePath + this->blackboard->postfix, filePath, fileIO->cachePolicy)) {
                NKFileClientImpl::FileMetaData fileMetaData;
                fileMetaData.etag = this->blackboard->file.storage.etag;
                fileMetaData.dataHash = calc_nkdatahash_v1(this->blackboard->file.storage.data);
                this->blackboard->fileClientImpl->metadataMap[filePath] = fileMetaData;
                this->blackboard->fileClientImpl->SaveCachedFileData();
            }
            state = BehaviourTree::AState::Success;
            break;
        default:
            this->blackboard->error = NKError(NKErrorType::VALUE6, "File conflict has not been resolved", "", "");
            state = BehaviourTree::AState::Failure;
            break;
    }

    this->blackboard->fileClientImpl->ProcessBHTree(this->blackboard->action.lock(), this->blackboard);
}
