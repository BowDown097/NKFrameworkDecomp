#pragma once
#include "NewFramework/Networking/Protocols/HTTP/IHttpCallback.h"
#include "Uncategorized/BehaviourTree.h"

struct NKMessage;
struct NKMessageResponseFile;
struct NKMessageResponseFileStorage;
struct SNKFileClientBlackboard;

class BA_FileWebRequest : public BehaviourTree::Leaf, public IHttpCallback {
public:
    void Start(BehaviourTree::IBlackboard* blackboard) override;
    void WriteEtagToCache(const std::string& key, const NKMessageResponseFileStorage& storage, const std::string& data);
    void HttpComplete(const SHttpRequest& request) override;
    NKMessageResponseFile ParseResponseMessage(const SHttpRequest& request);
    void HttpFailed(const SHttpRequest& request) override;
    virtual void ConstructHeader(std::map<std::string, std::string>& headers) = 0;
    virtual void ConstructMessage(std::string& data) = 0;
    virtual std::string GetHttpCallbackKey() = 0;
    virtual std::string GetUrl() = 0;
    virtual void HandleMessage() = 0;
    virtual bool IsRendered() = 0;
    virtual bool IsStorageResponse() = 0;
    virtual bool SendsPostData() = 0;
    virtual HTTP_METHOD GetHTTPMethod() { return METHOD_POST; }
    std::string GetEtagFromHeaders(const SHttpRequest& request);
protected:
    SNKFileClientBlackboard* blackboard{}; // 0x18
};
