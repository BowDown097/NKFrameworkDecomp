#include "NKAction_DownloadFile.h"
#include "NewFramework/Analytics/DGAnalytics.h"
#include "NewFramework/IO/File/BaseFileIO.h"
#include "NewFramework/Networking/NKAPI/NKManager.h"
#include "NewFramework/Platform/Shared/Logging.h"
#include "NewFramework/Utilities/Encoding.h"
#include "NewFramework/Utilities/StringHelper.h"
#include "NewFramework/Utilities/Utilities.h"
#include "Uncategorized/Blackboards.h"
#include "Uncategorized/StephenEncryption.h"

#ifndef NDEBUG
#include <iostream>
#endif

BehaviourTree::Action* BA_FileClientCallback::clone() {
    return new BA_FileClientCallback(*this);
}

BA_FileClientCallback* BA_FileClientCallback::Create(NKFileClientCallback_t callback) {
    return new BA_FileClientCallback(callback);
}

void BA_FileClientCallback::Start(BehaviourTree::IBlackboard* blackboard) {
    state = BehaviourTree::AState::Running;
    this->blackboard = dynamic_cast<SNKFileClientBlackboard*>(blackboard);

    std::string filePath = this->blackboard->savePath + "/" + this->blackboard->filename;
    std::string action;

    switch (this->blackboard->callbackAction) {
        case FileClientCallbackAction::Download:
            action = "download";
            break;
        case FileClientCallbackAction::Upload:
            action = "upload";
            break;
        case FileClientCallbackAction::CheckConflicts:
            action = "check conflicts for";
            break;
    }

    if (!callback.empty()) {
        if (!filePath.empty()) {
    #ifndef NDEBUG // speculative. action and filePath are unused, except for a size check on filePath here with nothing else
            std::cout << action << " " << filePath << std::endl;
    #endif
        }

        std::string file = this->blackboard->filename + this->blackboard->postfix;
        NKError* error = this->blackboard->error.type != NKErrorType::VALUE0 ? &this->blackboard->error : nullptr;
        callback(this->blackboard->file.storage.success, this->blackboard->savePath, file, error);
    }

    state = BehaviourTree::AState::Success;
    this->blackboard->fileClientImpl->ProcessBHTree(this->blackboard->action.lock(), this->blackboard);
}

BehaviourTree::Action* BA_DownloadFileHeader::clone() {
    return new BA_DownloadFileHeader(*this);
}

BA_DownloadFileHeader* BA_DownloadFileHeader::Create(CBaseFileIO* fileIO, bool requestPublic) {
    return new BA_DownloadFileHeader(fileIO, requestPublic);
}

void BA_DownloadFileHeader::ConstructMessage(std::string& data) {
    NKMessageStorageLoad storageLoad;
    storageLoad.filename = blackboard->filename;
    storageLoad.owner = blackboard->nkapiID;
    storageLoad.includeBody = false;
    data = NKJSON::Serialise<NKMessageStorageLoad>(storageLoad);
}

std::string BA_DownloadFileHeader::GetHttpCallbackKey() {
    return "NK-pre-load";
}

std::string BA_DownloadFileHeader::GetUrl() {
    if (!blackboard->url.empty()) {
        return blackboard->url;
    } else if (requestPublic && blackboard->filePermissions == eNKFileClientFilePermissions::PUBLIC) {
        return NKEndpoints::GetPublicFileUrl(
            blackboard->serverCluster, blackboard->appIDStr, blackboard->nkapiID, blackboard->filename);
    } else {
        return NKEndpoints::GetBase(blackboard->serverCluster) + NKEndpoints::GetDownloadFile();
    }
}

void BA_DownloadFileHeader::HandleMessage() {
    if (state == BehaviourTree::AState::Failure) {
        blackboard->error = NKError();
        return;
    }

    std::string filePath = blackboard->savePath + "/" + blackboard->filename;
    auto it = blackboard->fileClientImpl->metadataMap.find(filePath);

    SNKManagerBlackboard* managerBlackboard = NKManager::GetManager()->blackboard;
    managerBlackboard->field_80 = blackboard->file.storage.etag;
    managerBlackboard->field_60 = it != blackboard->fileClientImpl->metadataMap.end() ? it->second.etag : "";
    managerBlackboard->field_78 = fileIO->FileExists(filePath, fileIO->cachePolicy);
    blackboard->field_1C8 = false;

    bool fileExists = fileIO->FileExists(filePath, fileIO->cachePolicy);
    if (!blackboard->file.storage.etag.empty() && it != blackboard->fileClientImpl->metadataMap.end() &&
        blackboard->file.storage.etag == it->second.etag && fileExists) {
        blackboard->field_1C8 = true;
        state = BehaviourTree::AState::Success;
    } else if (blackboard->file.storage.success) {
        state = BehaviourTree::AState::Failure;
    } else {
        state = BehaviourTree::AState::Success;
    }
}

bool BA_DownloadFileHeader::IsRendered() {
    return !requestPublic || blackboard->filePermissions != eNKFileClientFilePermissions::PUBLIC;
}

bool BA_DownloadFileHeader::IsStorageResponse() {
    return IsRendered();
}

bool BA_DownloadFileHeader::SendsPostData() {
    return !requestPublic || blackboard->filePermissions != eNKFileClientFilePermissions::PUBLIC;
}

HTTP_METHOD BA_DownloadFileHeader::GetHTTPMethod() {
    return SendsPostData() ? METHOD_POST : METHOD_HEAD;
}

BehaviourTree::Action* BA_DownloadFile::clone() {
    return new BA_DownloadFile(*this);
}

BA_DownloadFile* BA_DownloadFile::Create(CBaseFileIO* fileIO, CEventManager* eventManager, bool writeEtag,
                                         bool requestPublic, bool isRendered, bool isStorageResponse) {
    return new BA_DownloadFile(fileIO, eventManager, writeEtag, requestPublic, isRendered, isStorageResponse);
}

void BA_DownloadFile::ConstructHeader(std::map<std::string, std::string>& headers) {
    std::string filePath = blackboard->savePath + "/" + blackboard->filename + blackboard->postfix;
    std::string etag = blackboard->fileClientImpl->GetCachedFileEtag(filePath);

    if (!etag.empty()) {
        headers["If-None-Match"] = etag;
    }
}

void BA_DownloadFile::ConstructMessage(std::string& data) {
    NKMessageStorageLoad storageLoad;
    storageLoad.filename = blackboard->filename;
    storageLoad.owner = blackboard->nkapiID;
    storageLoad.includeBody = true;
    data = NKJSON::Serialise<NKMessageStorageLoad>(storageLoad);
}

std::string BA_DownloadFile::GetHttpCallbackKey() {
    return requestPublic && blackboard->filePermissions == eNKFileClientFilePermissions::PUBLIC ? "NK-load-public" : "NK-load";
}

std::string BA_DownloadFile::GetUrl() {
    if (!blackboard->url.empty()) {
        return blackboard->url;
    } else if (requestPublic && blackboard->filePermissions == eNKFileClientFilePermissions::PUBLIC) {
        return NKEndpoints::GetPublicFileUrl(
            blackboard->serverCluster, blackboard->appIDStr, blackboard->nkapiID, blackboard->filename);
    } else {
        return NKEndpoints::GetBase(blackboard->serverCluster) + NKEndpoints::GetDownloadFile();
    }
}

bool BA_DownloadFile::IsRendered() {
    if (isRendered) {
        return true;
    }

    if (requestPublic) {
        return blackboard->filePermissions != eNKFileClientFilePermissions::PUBLIC;
    }

    return true;
}

bool BA_DownloadFile::IsStorageResponse() {
    return isStorageResponse;
}

bool BA_DownloadFile::SendsPostData() {
    return !requestPublic || blackboard->filePermissions != eNKFileClientFilePermissions::PUBLIC;
}

HTTP_METHOD BA_DownloadFile::GetHTTPMethod() {
    return SendsPostData() ? METHOD_POST : METHOD_GET;
}

void BA_DownloadFile::HandleMessage() {
    if (state == BehaviourTree::AState::Failure) {
        return;
    }

    if (!blackboard->file.upToDate) {
        std::string filePath = blackboard->savePath + "/" + blackboard->filename + blackboard->postfix;
        IFile* file = fileIO->OpenFile(filePath, fileIO->cachePolicy, eFileOpenMode::ReadWriteNew);

        if (!file) {
            throw std::runtime_error("Failed to open file for storing downloaded data");
        }

        std::string fileData = blackboard->file.storage.data;

        if (blackboard->stephen) {
            int crc = GetCRCFromData(fileData.c_str(), fileData.size());
            std::string dgdataHeader = "DGDATA" + StringHelper::Format("%08x", crc);
            fileData = dgdataHeader + fileData;

            CStephenEncryption stephenEncryption;
            stephenEncryption.EncryptStream(reinterpret_cast<uint8_t*>(fileData.data() + 14), fileData.size() - 14);
        }

        IFile::SFileIOResult writeResult = file->WriteStringWithResult(fileData, IFile::eWriteSyncBehaviour::Sync);
        if (writeResult.status == IFile::eFileIOStatus::Success) {
            if (writeEtag) {
                WriteEtagToCache(filePath, blackboard->file.storage, calc_nkdatahash_v1(fileData));
            }

            state = BehaviourTree::AState::Success;
        } else {
            blackboard->error = NKError(NKErrorType::VALUE10, writeResult.info, "Failed to write downloaded file data", "");

            DGAnalyticsData analyticsData("NKNetworkingError");
            analyticsData.AddPair("type", sNKError_HttpFileWrite);
            analyticsData.AddPair("info", StringHelper::Format(
                "Code: %d\nType: %s\nReason: %s\nFix: %s",
                static_cast<int>(blackboard->error.type),
                blackboard->error.typeStr.c_str(),
                blackboard->error.reason.c_str(),
                blackboard->error.fix.c_str()));
            analyticsData.AddPair("url", filePath);
            DGAnalytics::Instance()->SendDataEvent(analyticsData, true, AnalyticsEventGroups::Group::Framework, 2);

            state = BehaviourTree::AState::Failure;
        }

        delete file;
    } else {
        std::string filePath = blackboard->savePath + "/" + blackboard->filename + blackboard->postfix;
        if (fileIO->FileExists(filePath, fileIO->cachePolicy)) {
            state = BehaviourTree::AState::Success;
        } else {
            std::string error = StringHelper::Format(
                "Got cache file up to date for file %s, but file does not exist locally", blackboard->filename.c_str());
            LOG_ERROR(error.c_str()); ENFORCE_LINE(254);
            blackboard->fileClientImpl->ClearCachedFileEtag(filePath);
            blackboard->error = NKError(NKErrorType::VALUE4, error, "", "");
            state = BehaviourTree::AState::Failure;
        }
    }
}
