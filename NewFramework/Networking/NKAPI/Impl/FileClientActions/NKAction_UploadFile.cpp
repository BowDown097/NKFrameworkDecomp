#include "NKAction_UploadFile.h"
#include "NewFramework/Analytics/DGAnalytics.h"
#include "NewFramework/Networking/NKAPI/NKEndpoints.h"
#include "NewFramework/Utilities/Encoding.h"
#include "NewFramework/Utilities/StringHelper.h"
#include "Uncategorized/Blackboards.h"

template<typename T>
void construct_upload_message(SNKFileClientBlackboard* blackboard, T& storage, std::string& data) {
    if (!compress(blackboard->data, storage.data)) {
        blackboard->error = NKError(NKErrorType::VALUE4, "Failed to compress file data", "", "");
        return;
    }

    if (!string_to_base64(storage.data, storage.data)) {
        blackboard->error = NKError(NKErrorType::VALUE4, "Failed to b64 encode file data", "", "");
        return;
    }

    storage.options.isPublic = blackboard->filePermissions == eNKFileClientFilePermissions::PUBLIC;
    storage.options.compress = false;
    storage.options.decode = false;
    storage.options.reducedRedundancy = false;
    storage.options.expiresIn = 0;

    data = NKJSON::Serialise(storage);
}

BehaviourTree::Action* BA_UploadFile::clone() {
    return new BA_UploadFile(*this);
}

BA_UploadFile* BA_UploadFile::Create(CBaseFileIO* fileIO, bool buffer, uint64_t maximumBufferDuration) {
    return new BA_UploadFile(fileIO, buffer, maximumBufferDuration);
}

void BA_UploadFile::ConstructMessage(std::string& data) {
    if (buffer) {
        NKMessageStorageSaveBuffer storageSaveBuffer;
        storageSaveBuffer.storageEngine = "AWS:S3";
        storageSaveBuffer.maximumBufferDuration = maximumBufferDuration;
        construct_upload_message(blackboard, storageSaveBuffer, data);
    } else {
        NKMessageStorageSave storageSave;
        construct_upload_message(blackboard, storageSave, data);
    }
}

std::string BA_UploadFile::GetHttpCallbackKey() {
    return "NK-save";
}

std::string BA_UploadFile::GetUrl() {
    std::string base = NKEndpoints::GetBase(blackboard->serverCluster);
    std::string storageSavePath = buffer ? NKEndpoints::GetStorageSaveBuffer() : NKEndpoints::GetStorageSave();
    return base + storageSavePath;
}

bool BA_UploadFile::IsRendered() {
    return true;
}

bool BA_UploadFile::IsStorageResponse() {
    return IsRendered();
}

bool BA_UploadFile::SendsPostData() {
    return true;
}

void BA_UploadFile::HandleMessage() {
    if (state == BehaviourTree::AState::Failure) {
        return;
    }

    std::string filePath = blackboard->savePath + "/" + blackboard->fileName;
    IFile* file = fileIO->OpenFile(filePath, fileIO->cachePolicy, eFileOpenMode::ReadWriteNew);

    if (!file) {
        throw std::runtime_error("Unable to open file for caching uploaded data");
    }

    IFile::SFileIOResult fileIOResult = file->WriteBytesWithResult(
        reinterpret_cast<uint8_t*>(blackboard->data.data()),
        blackboard->data.size(),
        IFile::eWriteSyncBehaviour::Sync);

    if (fileIOResult.status == IFile::eFileIOStatus::Success) {
        WriteEtagToCache(filePath, blackboard->file.storage, calc_nkdatahash_v1(blackboard->data.data()));
        state = BehaviourTree::AState::Success;
    } else {
        blackboard->error = NKError(NKErrorType::VALUE10, fileIOResult.info, "Failed to write uploaded file data", "");

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
}
