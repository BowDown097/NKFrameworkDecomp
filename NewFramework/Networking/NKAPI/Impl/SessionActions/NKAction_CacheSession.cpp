#include "NKAction_CacheSession.h"
#include "NewFramework/IO/File/BaseFileIO.h"
#include "NewFramework/Utilities/StringHelper.h"
#include "NewFramework/Utilities/Utilities.h"
#include "Uncategorized/Blackboards.h"
#include "Uncategorized/StephenEncryption.h"

BA_CacheSession* BA_CacheSession::Create(CBaseFileIO* fileIO) {
    return new BA_CacheSession(fileIO);
}

void BA_CacheSession::Start(BehaviourTree::IBlackboard* blackboard) {
    state = BehaviourTree::AState::Running;

    NKSessionBlackboard* sessionBlackboard = dynamic_cast<NKSessionBlackboard*>(blackboard);
    sessionBlackboard->LogMsg("Caching session token for future use..");

    state = BehaviourTree::AState::Failure;
    if (fileIO) {
        std::string sessionCachePath = NKEndpoints::GetSessionCacheFilePath(sessionBlackboard->serverCluster);
        IFile* sessionCacheFile = fileIO->OpenFile(sessionCachePath, fileIO->documentPolicy, eFileOpenMode::ReadWriteNew);
        if (sessionCacheFile) {
            NKResponseLogin responseLogin;
            responseLogin.session.sessionID = sessionBlackboard->accessToken.token;
            responseLogin.session.expires = sessionBlackboard->accessToken.expires;
            responseLogin.user = sessionBlackboard->responseUser;

            std::string responseLoginData = NKJSON::Serialise<NKResponseLogin>(responseLogin);
            int responseLoginCrc = GetCRCFromData(responseLoginData.c_str(), responseLoginData.size());

            std::string dgdataHeader = StringHelper::Format("DGDATA%08x", responseLoginCrc);
            CStephenEncryption stephenEncryption;
            stephenEncryption.EncryptStream(reinterpret_cast<uint8_t*>(responseLoginData.data()), responseLoginData.size());
            responseLoginData = dgdataHeader + responseLoginData;

            sessionCacheFile->WriteString(responseLoginData, IFile::eWriteSyncBehaviour::Sync);
            sessionCacheFile->Close(IFile::eWriteSyncBehaviour::Sync);

            state = BehaviourTree::AState::Success;
            delete sessionCacheFile;
        }
    }

    sessionBlackboard->LogMsg(state == BehaviourTree::AState::Success ? "Success" : "Failure");
}

BehaviourTree::Action* BA_CacheSession::clone() {
    return new BA_CacheSession(*this);
}
