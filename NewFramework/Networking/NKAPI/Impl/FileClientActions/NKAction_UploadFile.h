#pragma once
#include "NKAction_FileWebRequest.h"

class CBaseFileIO;

class BA_UploadFile : public BA_FileWebRequest {
public:
    BehaviourTree::Action* clone() override;
    static BA_UploadFile* Create(CBaseFileIO* fileIO, bool buffer, uint64_t maximumBufferDuration);
    void ConstructHeader(std::map<std::string, std::string>& headers) override {}
    void ConstructMessage(std::string& data) override;
    std::string GetHttpCallbackKey() override;
    std::string GetUrl() override;
    void HandleMessage() override;
    bool IsRendered() override;
    bool IsStorageResponse() override;
    bool SendsPostData() override;
private:
    CBaseFileIO* fileIO; // 0x20
    bool buffer; // 0x28
    uint64_t maximumBufferDuration; // 0x30

    BA_UploadFile(CBaseFileIO* fileIO, bool buffer, uint64_t maximumBufferDuration)
        : fileIO(fileIO), buffer(buffer), maximumBufferDuration(maximumBufferDuration) {}
};
