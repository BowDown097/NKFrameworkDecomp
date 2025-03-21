#include "NKFileClientImpl.h"

#include "NewFramework/IO/File/BaseFileIO.h"
#include "NewFramework/Networking/NKAPI/Impl/FileClientActions/NKAction_AppendFileSavePath.h"
#include "NewFramework/Networking/NKAPI/Impl/FileClientActions/NKAction_DownloadFile.h"
#include "NewFramework/Networking/NKAPI/Impl/FileClientActions/NKAction_UploadFile.h"
#include "NewFramework/Networking/NKAPI/NKSession.h"
#include "NewFramework/Platform/Shared/Assert.h"
#include "NewFramework/Platform/Shared/Logging.h"
#include "NewFramework/Utilities/Encoding.h"
#include "Uncategorized/Blackboards.h"

#include <json_spirit/json_spirit.h>

static std::string s_sMetaDataFile = "cache.files";

std::string NKFileClientImpl::ConstructSavePath(const std::string& url) const {
    size_t slashIndex = url.rfind('/');
    std::string downloadDir = NKEndpoints::GetDownloadDirectory();
    if (slashIndex != std::string::npos) {
        downloadDir += string_to_md5("url:" + url.substr(0, slashIndex));
    }

    return downloadDir;
}

std::string NKFileClientImpl::ConstructSavePath(const std::string& appID, std::string nkapiID) const {
    if (nkapiID.empty() && NKSession::GetActiveSession()->GetState() == NKSessionState::VALUE1) {
        nkapiID = NKSession::GetActiveSession()->GetUserDetails().nkapiID;
    }

    std::stringstream ss;
    ss << NKSession::GetActiveSession()->GetAppID();
    return appID == ss.str()
        ? NKEndpoints::GetDownloadDirectory() + nkapiID
        : NKEndpoints::GetDownloadDirectory() + appID + "/" + nkapiID;
}

void NKFileClientImpl::LoadMetaData() {
    metadataMap.clear();
    if (!fileIO) {
        return;
    }

    std::string path = NKEndpoints::GetDownloadDirectory() + s_sMetaDataFile;
    IFile* file = fileIO->OpenFile(path, fileIO->cachePolicy, eFileOpenMode::Read);
    if (!file) {
        return;
    }

    std::string fileData;
    if (file->ReadString(fileData)) {
        json_spirit::mValue jsonValue;
        if (json_spirit::read(fileData, jsonValue)) {
            json_spirit::mObject& obj = jsonValue.get_obj();
            for (auto it = obj.begin(); it != obj.end(); ++it) {
                json_spirit::mObject& innerObj = it->second.get_obj();
                FileMetaData metadata;

                auto etagIt = innerObj.find("Etag");
                if (etagIt == innerObj.end()) {
                    throw std::runtime_error("Couldn't find element 'Etag'");
                }

                metadata.etag = strip_weak_etag_marker(etagIt->second.get_str());

                auto dataIt = innerObj.find("Data");
                if (dataIt == innerObj.end()) {
                    throw std::runtime_error("Couldn't find element 'Data'");
                }

                metadata.dataHash = dataIt->second.get_str();

                std::string cachedFileData;
                IFile* cachedFile = fileIO->OpenFile(it->first, fileIO->cachePolicy, eFileOpenMode::Read);
                if (cachedFile) {
                    if (cachedFile->ReadString(cachedFileData)) {
                        std::string fileHash = calc_nkdatahash_v1(cachedFileData);
                        if (fileHash == metadata.dataHash || fileHash.empty()) {
                            metadataMap[it->first] = metadata;
                        }
                    }
                    delete cachedFile;
                }
            }
        }
    }

    delete file;
}

const bool NKFileClientImpl::Initialise(CBaseFileIO* fileIO, CEventManager* eventManager) {
    if (!initialised) {
        this->fileIO = fileIO;
        this->eventManager = eventManager;
        LoadMetaData();
        initialised = fileIO != nullptr;
    }
    return initialised;
}

const bool NKFileClientImpl::IsDownloadingURL(const std::string& url) const {
    std::string savePath = ConstructSavePath(url);

    size_t slashIndex = url.rfind('/');
    std::string filename = slashIndex != std::string::npos ? url.substr(slashIndex + 1) : url;
    NKAssert(!filename.empty(), "Filename empty, failed to download"); ENFORCE_LINE(107);

    auto it = treeDataMap.find(savePath + "/" + filename);
    return it != treeDataMap.end() && it->second.callbackAction == FileClientCallbackAction::Download;
}

std::string NKFileClientImpl::GetCachedFileEtag(const std::string& name) const {
    auto it = metadataMap.find(name);
    return it != metadataMap.end() ? it->second.etag : std::string();
}

std::string NKFileClientImpl::GetCachedFileDataHash(const std::string& name) const {
    auto it = metadataMap.find(name);
    return it != metadataMap.end() ? it->second.dataHash : std::string();
}

const bool NKFileClientImpl::Download(
    const std::string& url,
    NKFileClientCallback_t callback,
    SHttpTimeoutOptions timeoutOpts,
    NKResponseRenderOptions renderOpts,
    bool stephen) {

    NKAssert(!url.empty(), "URL empty, failed to download"); ENFORCE_LINE(130);

    std::string savePath = ConstructSavePath(url);

    size_t slashIndex = url.rfind('/');
    std::string filename = slashIndex != std::string::npos ? url.substr(slashIndex + 1) : url;
    NKAssert(!filename.empty(), "Filename empty, failed to download"); ENFORCE_LINE(136);

    std::string fullPath = savePath + "/" + filename;
    if (treeDataMap.find(fullPath) != treeDataMap.end()) {
        return false;
    }

    boost::shared_ptr<SNKFileClientBlackboard> blackboard = boost::make_shared<SNKFileClientBlackboard>();
    blackboard->fileClientImpl = this;
    blackboard->stephen = stephen;
    blackboard->accessToken = NKSession::GetActiveSession()->GetAccessToken();
    blackboard->appID = NKSession::GetActiveSession()->GetAppID();
    blackboard->skuID = NKSession::GetActiveSession()->GetSkuID();
    blackboard->privateKey = NKSession::GetActiveSession()->GetPrivateKey();
    blackboard->serverCluster = NKSession::GetActiveSession()->GetServerCluster();
    blackboard->url = url;
    blackboard->filename = filename;
    blackboard->savePath = savePath;
    blackboard->filePermissions = eNKFileClientFilePermissions::PUBLIC;
    blackboard->start = clock();
    blackboard->timeoutOptions = timeoutOpts;

    BehaviourTree::Sequence* sequence = BehaviourTree::Sequence::Create(BehaviourTree::LoopCondition::None);

    BehaviourTree::Sequence* sequence2 = BehaviourTree::Sequence::Create(BehaviourTree::LoopCondition::None);

    BA_DownloadFile* downloadFile = BA_DownloadFile::Create( // this is very odd... is this right?
        fileIO, eventManager, true, true, renderOpts.field_0 & 1, renderOpts.field_0 & 256);
    sequence2->AddAction(downloadFile);

    BehaviourTree::ForceState* sequence2Success = new BehaviourTree::ForceState;
    sequence2Success->child = sequence2;
    sequence2Success->forceState = BehaviourTree::AState::Success;
    sequence->AddAction(sequence2Success);

    BA_FileClientCallback* fileClientCallback = BA_FileClientCallback::Create(callback);
    sequence->AddAction(fileClientCallback);

    boost::shared_ptr<BehaviourTree::Action> action(sequence);
    blackboard->action = action;
    blackboard->field_228 = false;
    blackboard->field_229 = false;

    TreeData treeData;
    treeData.action = action;
    treeData.blackboard = blackboard;
    treeData.callbackAction = FileClientCallbackAction::Download;

    treeDataMap.emplace(fullPath, treeData);

    ProcessBHTree(action, blackboard.get());

    return blackboard->error.type == NKErrorType::VALUE0;
}

const bool NKFileClientImpl::Download(
    const std::string& appID,
    const std::string& nkapiID,
    const std::string& filename,
    bool isRendered,
    bool isStorageResponse,
    eNKFileClientFilePermissions filePermissions,
    NKFileClientCallback_t callback,
    bool stephen) {

    if (filePermissions != eNKFileClientFilePermissions::PUBLIC &&
        NKSession::GetActiveSession()->GetState() != NKSessionState::VALUE1) {
        return false;
    }

    NKAssert(!filename.empty(), "Filename empty, failed to download"); ENFORCE_LINE(206);

    std::string savePath = ConstructSavePath(appID, nkapiID);
    std::string fullPath = savePath + "/" + filename;

    if (treeDataMap.find(fullPath) != treeDataMap.end()) {
        return false;
    }

    boost::shared_ptr<SNKFileClientBlackboard> blackboard = boost::make_shared<SNKFileClientBlackboard>();
    blackboard->fileClientImpl = this;
    blackboard->stephen = stephen;
    blackboard->accessToken = NKSession::GetActiveSession()->GetAccessToken();
    blackboard->appID = NKSession::GetActiveSession()->GetAppID();
    blackboard->skuID = NKSession::GetActiveSession()->GetSkuID();
    blackboard->privateKey = NKSession::GetActiveSession()->GetPrivateKey();
    blackboard->serverCluster = NKSession::GetActiveSession()->GetServerCluster();
    blackboard->appIDStr = appID;
    blackboard->nkapiID = nkapiID;
    blackboard->filename = filename;
    blackboard->savePath = savePath;
    blackboard->filePermissions = filePermissions;
    blackboard->start = clock();

    BehaviourTree::Sequence* sequence = BehaviourTree::Sequence::Create(BehaviourTree::LoopCondition::None);

    BehaviourTree::Sequence* sequence2 = BehaviourTree::Sequence::Create(BehaviourTree::LoopCondition::None);

    BA_DownloadFile* downloadFile = BA_DownloadFile::Create(fileIO, eventManager, true, true, isRendered, isStorageResponse);
    sequence2->AddAction(downloadFile);

    BehaviourTree::ForceState* sequence2Success = new BehaviourTree::ForceState;
    sequence2Success->child = sequence2;
    sequence2Success->forceState = BehaviourTree::AState::Success;
    sequence->AddAction(sequence2Success);

    BA_FileClientCallback* fileClientCallback = BA_FileClientCallback::Create(callback);
    sequence->AddAction(fileClientCallback);

    boost::shared_ptr<BehaviourTree::Action> action(sequence);
    blackboard->action = action;
    blackboard->field_228 = false;
    blackboard->field_229 = false;

    TreeData treeData;
    treeData.action = action;
    treeData.blackboard = blackboard;
    treeData.callbackAction = FileClientCallbackAction::Download;

    treeDataMap.emplace(fullPath, treeData);

    ProcessBHTree(action, blackboard.get());

    return blackboard->error.type == NKErrorType::VALUE0;
}

std::vector<std::string> NKFileClientImpl::GetCachedFileNames() const {
    std::vector<std::string> out;

    for (auto it = metadataMap.begin(); it != metadataMap.end(); ++it) {
        out.push_back(it->first);
    }

    return out;
}

void NKFileClientImpl::ClearCachedFileEtag(const std::string& name) {
    auto it = metadataMap.find(name);
    if (it != metadataMap.end()) {
        metadataMap.erase(it);
    }
}

void NKFileClientImpl::ProcessBHTree(boost::shared_ptr<BehaviourTree::Action> action, BehaviourTree::IBlackboard* blackboard) {
    SNKFileClientBlackboard* fileClientBlackboard = dynamic_cast<SNKFileClientBlackboard*>(blackboard);
    if (fileClientBlackboard->field_229) {
        fileClientBlackboard->field_228 = true;
        return;
    }

    do {
        fileClientBlackboard->field_228 = false;
        fileClientBlackboard->field_229 = true;

        if (action->state == BehaviourTree::AState::Idle) {
            action->Start(fileClientBlackboard);
        } else if (action->state == BehaviourTree::AState::Running) {
            action->Update(fileClientBlackboard, 0.1);
        }

        fileClientBlackboard->field_229 = false;
    } while (action->state == BehaviourTree::AState::Running || fileClientBlackboard->field_228);

    if (action->state == BehaviourTree::AState::Running) {
        return;
    }

    action->Stop(fileClientBlackboard);

    std::string fullPath = fileClientBlackboard->savePath + "/" + fileClientBlackboard->filename;

    auto treeDataIt = treeDataMap.find(fullPath);
    if (treeDataIt == treeDataMap.end()) {
        return;
    }

    auto uploadDataIt = uploadDataMap.find(fileClientBlackboard->filename);
    treeDataMap.erase(treeDataIt);
    if (uploadDataIt == uploadDataMap.end()) {
        return;
    }

    Upload(uploadDataIt->first,
           uploadDataIt->second.data,
           uploadDataIt->second.filePermissions,
           uploadDataIt->second.callback,
           uploadDataIt->second.conflictCallback,
           uploadDataIt->second.noConflictCheck,
           uploadDataIt->second.buffer,
           uploadDataIt->second.maximumBufferDuration);

    uploadDataMap.erase(uploadDataIt);
}

void NKFileClientImpl::Invalidate(const std::string& filename) {
    std::string path = NKEndpoints::GetDownloadDirectory() + filename;
    auto it = metadataMap.find(path);

    if (it != metadataMap.end()) {
        fileIO->FileDelete(path, fileIO->cachePolicy);
        metadataMap.erase(it);
        SaveCachedFileData();
    }
}

bool NKFileClientImpl::IsDownloading(const std::string& filename) const {
    std::stringstream ss;
    ss << NKSession::GetActiveSession()->GetAppID();
    std::string appIDStr = ss.str();

    std::string savePath = ConstructSavePath(appIDStr, NKSession::GetActiveSession()->GetUserDetails().nkapiID);
    std::string fullPath = savePath + "/" + filename;

    auto it = treeDataMap.find(fullPath);
    return it != treeDataMap.end() && it->second.callbackAction == FileClientCallbackAction::Download;
}

const bool NKFileClientImpl::Upload(
    const std::string& filename,
    const std::string& data,
    eNKFileClientFilePermissions filePermissions,
    NKFileClientCallback_t callback,
    NKFileClientConflictCallback_t conflictCallback,
    bool noConflictCheck) {

    std::vector<char> vec(data.begin(), data.end());
    return Upload(filename, vec, filePermissions, callback, conflictCallback, noConflictCheck, false, 0);
}

const bool NKFileClientImpl::Upload(
    const std::string& filename,
    IFile* file,
    eNKFileClientFilePermissions filePermissions,
    NKFileClientCallback_t callback,
    NKFileClientConflictCallback_t conflictCallback,
    bool noConflictCheck) {

    if (!file) {
        return false;
    }

    size_t fileLength = file->GetFileLength();
    std::vector<char> vec(fileLength);
    if (file->ReadBytes(reinterpret_cast<uint8_t*>(vec.data()), fileLength) != fileLength) {
        return false;
    }

    return Upload(filename, vec, filePermissions, callback, conflictCallback, noConflictCheck, false, 0);
}

const bool NKFileClientImpl::UploadBuffered(
    const std::string& filename,
    const std::string& data,
    eNKFileClientFilePermissions filePermissions,
    NKFileClientCallback_t callback,
    NKFileClientConflictCallback_t conflictCallback,
    uint64_t maximumBufferDuration,
    bool noConflictCheck) {

    std::vector<char> vec(data.begin(), data.end());
    return Upload(filename, vec, filePermissions, callback, conflictCallback, noConflictCheck, true, maximumBufferDuration);
}

const bool NKFileClientImpl::UploadBuffered(
    const std::string& filename,
    IFile* file,
    eNKFileClientFilePermissions filePermissions,
    NKFileClientCallback_t callback,
    NKFileClientConflictCallback_t conflictCallback,
    uint64_t maximumBufferDuration,
    bool noConflictCheck) {

    if (!file) {
        return false;
    }

    size_t fileLength = file->GetFileLength();
    std::vector<char> vec(fileLength);
    if (file->ReadBytes(reinterpret_cast<uint8_t*>(vec.data()), fileLength) != fileLength) {
        return false;
    }

    return Upload(filename, vec, filePermissions, callback, conflictCallback, noConflictCheck, true, maximumBufferDuration);
}

void NKFileClientImpl::QueueUpload(
    const std::string& filename,
    const std::vector<char>& data,
    eNKFileClientFilePermissions filePermissions,
    NKFileClientCallback_t callback,
    NKFileClientConflictCallback_t conflictCallback,
    bool noConflictCheck,
    bool buffer,
    uint64_t maximumBufferDuration) {

    QueuedUploadData uploadData;
    uploadData.data = data;
    uploadData.filePermissions = filePermissions;
    uploadData.callback = callback;
    uploadData.conflictCallback = conflictCallback;
    uploadData.noConflictCheck = noConflictCheck;
    uploadData.buffer = buffer;
    uploadData.maximumBufferDuration = maximumBufferDuration;

    uploadDataMap[filename] = uploadData;
}








































































































































void NKFileClientImpl::SaveCachedFileData() {
    std::string path = NKEndpoints::GetDownloadDirectory() + s_sMetaDataFile;
    IFile* file = fileIO->OpenFile(path, fileIO->cachePolicy, eFileOpenMode::ReadWriteNew);

    if (!file) {
        LOG_ERROR("Failed to open meta.data file for writing"); ENFORCE_LINE(583);
        return;
    }

    json_spirit::mObject obj;

    for (auto it = metadataMap.begin(); it != metadataMap.end(); ++it) {
        json_spirit::mObject inner;

        inner["Etag"] = it->second.etag;
        inner["Data"] = it->second.dataHash;

        obj.emplace(it->first, inner);
    }

    std::string objData = json_spirit::write(obj);
    file->WriteString(objData, IFile::eWriteSyncBehaviour::Sync);

    delete file;
}

const bool NKFileClientImpl::Upload(
    const std::string& filename,
    const std::vector<char>& data,
    eNKFileClientFilePermissions filePermissions,
    NKFileClientCallback_t callback,
    NKFileClientConflictCallback_t conflictCallback,
    bool noConflictCheck,
    bool buffer,
    uint64_t maximumBufferDuration) {

    if (NKSession::GetActiveSession()->GetState() != NKSessionState::VALUE1) {
        return false;
    }

    NKAssert(!filename.empty(), "Filename empty, failed to upload"); ENFORCE_LINE(618);

    std::stringstream ss;
    ss << NKSession::GetActiveSession()->GetAppID();
    std::string appIDStr = ss.str();

    std::string savePath = ConstructSavePath(appIDStr, NKSession::GetActiveSession()->GetUserDetails().nkapiID);
    std::string fullPath = savePath + "/" + filename;

    if (treeDataMap.find(fullPath) != treeDataMap.end()) {
        QueueUpload(filename, data, filePermissions, callback, conflictCallback, noConflictCheck, buffer, maximumBufferDuration);
        return true;
    }

    boost::shared_ptr<SNKFileClientBlackboard> blackboard = boost::make_shared<SNKFileClientBlackboard>();
    blackboard->fileClientImpl = this;
    blackboard->callbackAction = noConflictCheck ? FileClientCallbackAction::Upload : FileClientCallbackAction::CheckConflicts;
    blackboard->accessToken = NKSession::GetActiveSession()->GetAccessToken();
    blackboard->appID = NKSession::GetActiveSession()->GetAppID();
    blackboard->skuID = NKSession::GetActiveSession()->GetSkuID();
    blackboard->privateKey = NKSession::GetActiveSession()->GetPrivateKey();
    blackboard->serverCluster = NKSession::GetActiveSession()->GetServerCluster();
    blackboard->appIDStr = ss.str();
    blackboard->nkapiID = NKSession::GetActiveSession()->GetUserDetails().nkapiID;
    blackboard->filename = filename;
    blackboard->savePath = savePath;
    blackboard->filePermissions = filePermissions;
    blackboard->data = data;
    blackboard->timeoutOptions.lowSpeedLimit = 512;
    blackboard->timeoutOptions.lowSpeedTime = 20;

    BehaviourTree::Sequence* sequence = BehaviourTree::Sequence::Create(BehaviourTree::LoopCondition::None);

    BehaviourTree::Selector* selector = BehaviourTree::Selector::Create(BehaviourTree::LoopCondition::None);

    BehaviourTree::Sequence* sequence2 = BehaviourTree::Sequence::Create(BehaviourTree::LoopCondition::None);

    BA_DownloadFileHeader* downloadFileHeader = BA_DownloadFileHeader::Create(fileIO, false);
    sequence2->AddAction(downloadFileHeader);

    BehaviourTree::Sequence* sequence3 = BehaviourTree::Sequence::Create(BehaviourTree::LoopCondition::None);

    BehaviourTree::Conditional<BehaviourTree::ABool>* conditional = new BehaviourTree::Conditional<BehaviourTree::ABool>;
    conditional->condition = static_cast<BehaviourTree::ABool>(noConflictCheck);
    sequence3->AddAction(conditional);

    BA_UploadFile* uploadFile = BA_UploadFile::Create(fileIO, buffer, maximumBufferDuration);
    sequence3->AddAction(uploadFile);

    BehaviourTree::ForceState* sequence3Success = new BehaviourTree::ForceState;
    sequence3Success->child = sequence3;
    sequence3Success->forceState = BehaviourTree::AState::Success;
    sequence2->AddAction(sequence3Success);
    selector->AddAction(sequence2);

    BehaviourTree::Sequence* sequence4 = BehaviourTree::Sequence::Create(BehaviourTree::LoopCondition::None);

    BA_SetAppendFileSavePath* appendFileSavePath = BA_SetAppendFileSavePath::Create(".theirs");
    sequence4->AddAction(appendFileSavePath);

    BA_DownloadFile* downloadFile = BA_DownloadFile::Create(
        fileIO, eventManager, false, false, filePermissions != eNKFileClientFilePermissions::PUBLIC, true);
    sequence4->AddAction(downloadFile);

    BA_ConflictedFileCallback* conflictedFileCallback = BA_ConflictedFileCallback::Create(conflictCallback);
    sequence4->AddAction(conflictedFileCallback);

    BehaviourTree::Sequence* sequence5 = BehaviourTree::Sequence::Create(BehaviourTree::LoopCondition::None);

    BA_ResolveConflict* resolveConflict = BA_ResolveConflict::Create(fileIO);
    sequence5->AddAction(resolveConflict);

    BA_CheckResolutionMode* checkResolutionMode = BA_CheckResolutionMode::Create(NKConflictResolutionType::Delete);
    sequence5->AddAction(checkResolutionMode);

    BA_UploadFile* uploadFile2 = BA_UploadFile::Create(fileIO, buffer, maximumBufferDuration);
    sequence5->AddAction(uploadFile2);
    sequence4->AddAction(sequence5);
    selector->AddAction(sequence4);

    BehaviourTree::ForceState* selectorSuccess = new BehaviourTree::ForceState;
    selectorSuccess->child = selector;
    selectorSuccess->forceState = BehaviourTree::AState::Success;
    sequence->AddAction(selectorSuccess);

    BA_FileClientCallback* fileClientCallback = BA_FileClientCallback::Create(callback);
    sequence->AddAction(fileClientCallback);

    boost::shared_ptr<BehaviourTree::Action> action(sequence);
    blackboard->action = action;
    blackboard->field_228 = false;
    blackboard->field_229 = false;

    TreeData treeData;
    treeData.action = action;
    treeData.blackboard = blackboard;
    treeData.callbackAction = FileClientCallbackAction::Upload;

    treeDataMap.emplace(fullPath, treeData);

    ProcessBHTree(action, blackboard.get());

    return blackboard->error.type == NKErrorType::VALUE0;
}
