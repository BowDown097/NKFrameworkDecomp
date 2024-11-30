#pragma once
#include "NewFramework/Networking/NKAPI/Impl/NKMessages.h"
#include "NewFramework/Networking/NKAPI/NKEndpoints.h"
#include "NewFramework/Networking/NKAPI/NKError.h"
#include <boost/function/function3.hpp>

struct HttpCallbackFunctor;

struct RequestContext {
    using RequestCallback = boost::function3<void, const NKError*, NKMessageResponse, RequestContext>;

    NKEndpoints::Endpoint endpoint; // 0x00
    std::string data; // 0x20
    HTTP_METHOD method = HTTP_METHOD::METHOD_POST; // 0x38
    boost::shared_ptr<boost::function_base> moduleCallback; // 0x40
    RequestCallback callback; // 0x50
    std::shared_ptr<HttpCallbackFunctor> functor; // 0x70
    long timeout = -1; // 0x80
    int retries{}; // 0x88
};

class NKModule {
public:
    virtual ~NKModule();
    void DiscardActiveRequests();
    void ProcessResponse(const SHttpRequest& req);
    bool VerifyResponse(const std::string& data, const std::string& sig);
    void RequestAgain(RequestContext ctx);
    void Request(NKEndpoints::Endpoint endpoint, std::string data, HTTP_METHOD method,
                 RequestContext::RequestCallback requestCb, boost::shared_ptr<boost::function_base> moduleCb,
                 long timeout = -1, int a8 = 0, long lowSpeedTime = -1);
    void Request(std::string path, std::string data, HTTP_METHOD method,
                 RequestContext::RequestCallback requestCb, boost::shared_ptr<boost::function_base> moduleCb,
                 long timeout = -1, int a8 = 0, long lowSpeedTime = -1);
private:
    std::map<int, RequestContext> map; // 0x08
};
