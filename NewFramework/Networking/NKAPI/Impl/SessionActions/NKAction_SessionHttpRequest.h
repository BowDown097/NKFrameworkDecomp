#pragma once
#include "NewFramework/Networking/Protocols/HTTP/HttpRequest.h"
#include "Uncategorized/BehaviourTree.h"

struct HttpCallbackFunctor;
struct NKMessage;
struct NKMessageResponse;
class NKSessionBlackboard;

class BA_HttpRequestAction : public BehaviourTree::Leaf {
public:
    void Start(BehaviourTree::IBlackboard* blackboard) override;
    void Stop(BehaviourTree::IBlackboard* blackboard) override;
    void SendHttpRequest();
    void HttpComplete(const SHttpRequest& req);
    void HttpFailed(const SHttpRequest& req);
    virtual std::string HttpCallbackKey() = 0;
    virtual NKMessage ConstructMessage() = 0;
    virtual std::map<std::string, std::string> ConstructHeader() = 0;
    virtual std::string ConstructUrl() = 0;
    virtual void ParseResponse(const NKMessageResponse& response) = 0;
    virtual HTTP_METHOD GetHTTPMethod() { return METHOD_POST; }
protected:
    NKSessionBlackboard* blackboard{}; // 0x10
    std::shared_ptr<HttpCallbackFunctor> callback; // 0x18
};
