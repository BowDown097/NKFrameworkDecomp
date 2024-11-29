#include "NKAction_OpenSessionUsingToken.h"
#include "NewFramework/IO/File/BaseFileIO.h"
#include "NewFramework/IO/File/IFile.h"
#include "NewFramework/Networking/NKAPI/Impl/NKMessages.h"
#include "NewFramework/Platform/Shared/Logging.h"
#include "NewFramework/Utilities/Utilities.h"
#include "Uncategorized/Blackboards.h"
#include "Uncategorized/StephenEncryption.h"

void BA_OpenSessionUsingToken::Start(BehaviourTree::IBlackboard* blackboard) {
    NKSessionBlackboard* sessionBlackboard = dynamic_cast<NKSessionBlackboard*>(blackboard);
    state = BehaviourTree::AState::Failure;

    if (fileIO) {
        std::string sessionCachePath = NKEndpoints::GetSessionCacheFilePath(sessionBlackboard->serverCluster);

        if (fileIO->FileExists(sessionCachePath, fileIO->cachePolicy)) {
            IFile* sessionCacheFile = fileIO->OpenFile(sessionCachePath, fileIO->cachePolicy, eFileOpenMode::Read);
            if (sessionCacheFile && ReadTokenFile(sessionCacheFile, sessionBlackboard)) {
                delete sessionCacheFile;
                fileIO->FileDelete(sessionCachePath, fileIO->cachePolicy);
            }
        }

        if (state != BehaviourTree::AState::Success && fileIO->FileExists(sessionCachePath, fileIO->documentPolicy)) {
            IFile* sessionCacheFile = fileIO->OpenFile(sessionCachePath, fileIO->documentPolicy, eFileOpenMode::Read);
            if (sessionCacheFile) {
                ReadTokenFile(sessionCacheFile, sessionBlackboard);
                delete sessionCacheFile;
            }
        }
    }

    std::string message = "Checking existing access token is valid and not expired..";
    message += state == BehaviourTree::AState::Success ? "True" : "False";
    sessionBlackboard->LogMsg(message);
}

const bool BA_OpenSessionUsingToken::ReadTokenFile(IFile* file, NKSessionBlackboard* blackboard) {
    std::string fileData;
    if (!file->ReadString(fileData)) {
        return false;
    }

    if (fileData.substr(0, 6) == "DGDATA") {
        uint32_t fileCrc;
        sscanf(fileData.substr(6, 8).c_str(), "%x", &fileCrc);

        CStephenEncryption stephenEncryption;
        fileData = fileData.substr(14);
        stephenEncryption.DecryptStream(reinterpret_cast<uint8_t*>(fileData.data()), fileData.size());

        if (GetCRCFromData(fileData.c_str(), fileData.size()) != fileCrc) {
            LOG_ERROR("Invalid File"); ENFORCE_LINE(54);
            return false;
        }
    }

    NKResponseLogin responseLogin;
    if (!NKJSON::TryParse<NKResponseLogin>(responseLogin, fileData)) {
        return false;
    }

    blackboard->accessToken = NKAccessToken(responseLogin.session);
    blackboard->responseUser = responseLogin.user;
    blackboard->loginServiceFromToken = eNKLoginService::Cache;

    state = BehaviourTree::AState::Success;
    return true;
}

BehaviourTree::Action* BA_OpenSessionUsingToken::clone() {
    return new BA_OpenSessionUsingToken(*this);
}

BA_OpenSessionUsingToken* BA_OpenSessionUsingToken::Create(CBaseFileIO* fileIO) {
    return new BA_OpenSessionUsingToken(fileIO);
}
