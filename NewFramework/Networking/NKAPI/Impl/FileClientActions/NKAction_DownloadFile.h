#pragma once
#include "NewFramework/Networking/NKAPI/Impl/NKFileClientImpl.h"
#include "NKAction_FileWebRequest.h"

class CBaseFileIO;
class CEventManager;

class BA_FileClientCallback : public BehaviourTree::Leaf {
public:
    BehaviourTree::Action* clone() override;
    static BA_FileClientCallback* Create(NKFileClientCallback_t callback);
    void Start(BehaviourTree::IBlackboard* blackboard) override;
private:
    NKFileClientCallback_t callback; // 0x10
    SNKFileClientBlackboard* blackboard{}; // 0x18

    BA_FileClientCallback(NKFileClientCallback_t callback)
        : callback(callback) {}
};

class BA_DownloadFileHeader : public BA_FileWebRequest {
public:
    BehaviourTree::Action* clone() override;
    static BA_DownloadFileHeader* Create(CBaseFileIO* fileIO, bool requestPublic);
    void ConstructMessage(std::string& data) override;
    void ConstructHeader(std::map<std::string, std::string>& headers) override {}
    std::string GetHttpCallbackKey() override;
    std::string GetUrl() override;
    void HandleMessage() override;
    bool IsRendered() override;
    bool IsStorageResponse() override;
    bool SendsPostData() override;
    HTTP_METHOD GetHTTPMethod() override;
private:
    CBaseFileIO* fileIO; // 0x20
    bool requestPublic; // 0x28

    BA_DownloadFileHeader(CBaseFileIO* fileIO, bool requestPublic)
        : fileIO(fileIO), requestPublic(requestPublic) {}
};

class BA_DownloadFile : public BA_FileWebRequest {
public:
    BehaviourTree::Action* clone() override;
    static BA_DownloadFile* Create(CBaseFileIO* fileIO, CEventManager* eventManager, bool writeEtag,
                                   bool requestPublic, bool isRendered, bool isStorageResponse);
    void ConstructHeader(std::map<std::string, std::string>& headers) override;
    void ConstructMessage(std::string& data) override;
    std::string GetHttpCallbackKey() override;
    std::string GetUrl() override;
    void HandleMessage() override;
    bool IsRendered() override;
    bool IsStorageResponse() override;
    bool SendsPostData() override;
    HTTP_METHOD GetHTTPMethod() override;
private:
    CBaseFileIO* fileIO; // 0x20
    CEventManager* eventManager; // 0x28
    bool writeEtag; // 0x30
    bool requestPublic; // 0x31
    bool isRendered; // 0x32
    bool isStorageResponse; // 0x33

    BA_DownloadFile(CBaseFileIO* fileIO, CEventManager* eventManager, bool writeEtag,
                    bool requestPublic, bool isRendered, bool isStorageResponse)
        : fileIO(fileIO),
          eventManager(eventManager),
          writeEtag(writeEtag),
          requestPublic(requestPublic),
          isRendered(isRendered),
          isStorageResponse(isStorageResponse) {}
};
