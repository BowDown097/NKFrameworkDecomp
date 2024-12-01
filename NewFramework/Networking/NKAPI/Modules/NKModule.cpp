#include "NKModule.h"
#include "NewFramework/Analytics/DGAnalytics.h"
#include "NewFramework/Networking/NKAPI/NKManager.h"
#include "NewFramework/Networking/NKAPI/NKSession.h"
#include "NewFramework/Networking/NKAPI/NKSignature.h"
#include "NewFramework/Networking/Protocols/HTTP/HttpCallbackFunctor.h"
#include "NewFramework/Platform/Shared/Assert.h"
#include "NewFramework/Utilities/StringHelper.h"
#include "Uncategorized/NKRequestFactory.h"

NKModule::~NKModule() {
    DiscardActiveRequests();
}

void NKModule::DiscardActiveRequests() {
    map.clear();
}

bool NKModule::VerifyResponse(const std::string& data, const std::string& sig) {
    NKManager* manager = NKManager::GetManager();
    NKSession* session = manager->GetSessionModule();
    return NKSignature::Verify(session->GetAccessToken().token, manager->GetPrivateKey(), data, sig);
}

void NKModule::RequestAgain(RequestContext ctx) {
    Request(ctx.endpoint.path, ctx.data, ctx.method, ctx.callback, ctx.moduleCallback, ctx.timeout, ctx.retries, 0);
}

void NKModule::Request(NKEndpoints::Endpoint endpoint, std::string data, HTTP_METHOD method,
                       RequestContext::RequestCallback requestCb, boost::shared_ptr<boost::function_base> moduleCb,
                       long timeout, int retries, long lowSpeedTime) {
    std::shared_ptr<HttpCallbackFunctor> functor = std::make_shared<HttpCallbackFunctor>();
    functor->httpCallback = [this](const SHttpRequest& req) { ProcessResponse(req); };
    functor->field_40 = functor;

    int requestNumber = endpoint.isFullUrl
        ? NKRequestFactory::GenerateRequestFullURL(endpoint.path, data, method, functor.get(), timeout, lowSpeedTime)
        : NKRequestFactory::GenerateRequestPartialURL(endpoint.path, data, method, functor.get(), timeout, lowSpeedTime);

    RequestContext ctx;
    ctx.endpoint = endpoint;
    ctx.data = data;
    ctx.method = HTTP_METHOD::METHOD_POST;
    ctx.moduleCallback = moduleCb;
    ctx.callback = requestCb;
    ctx.functor = functor;
    ctx.retries = retries;
    ctx.timeout = timeout;

    map[requestNumber] = ctx;
}

void NKModule::Request(std::string path, std::string data, HTTP_METHOD method,
                       RequestContext::RequestCallback requestCb, boost::shared_ptr<boost::function_base> moduleCb,
                       long timeout, int retries, long lowSpeedTime) {
    NKModule::Request(NKEndpoints::Endpoint(path, false), data, method, requestCb, moduleCb, timeout, retries, lowSpeedTime);
}

void NKModule::ProcessResponse(const SHttpRequest& req) {
    RequestContext ctx = map[req.field_A8];
    map.erase(req.field_A8);
    ENFORCE_LINE(62); // following assert is supposed to be on line 63
    if (!NKAssert(ctx.functor != nullptr, "Expected valid HTTP functor when processing response")) {
        return;
    }

    NKError* error{};
    NKMessageResponse response;
    NKJSON::TryParse<NKMessageResponse>(response, req.GetDownloadedDataStr());

    if (req.error) {
        if ((unsigned int)(req.error - 1) >= 3) {
            DGAnalyticsData analyticsData("NKNetworkingError");
            analyticsData.AddPair("type", sNKError_HttpError);
            analyticsData.AddPair("info", req.GetErrorString());
            analyticsData.AddPair("url", req.url);
            DGAnalytics::Instance()->SendDataEvent(analyticsData, true, AnalyticsEventGroups::Group::Framework, 2);
        }

        if (response.error.details.reason.empty()) {
            response.error.details.reason = StringHelper::Format("HTTP Error: %s", req.GetErrorString().c_str());
        }

        error = new NKError(NKErrorType::VALUE2, response.error.details.reason, response.error.type, "", req.statusCode);
    } else if (req.statusCode != 200) {
        if (!response.error.details.reason.empty()) {
            if (!response.error.type.empty()) {
                DGAnalyticsData analyticsData("NKNetworkingError");
                analyticsData.AddPair("type", sNKError_HttpLink);
                analyticsData.AddPair("info", StringHelper::Format("%d", req.statusCode) + " : " + response.error.type);
                analyticsData.AddPair("url", req.url);
                DGAnalytics::Instance()->SendDataEvent(analyticsData, true, AnalyticsEventGroups::Group::Framework, 2);
            } else {
                DGAnalyticsData analyticsData("NKNetworkingError");
                analyticsData.AddPair("type", sNKError_HttpLink);
                analyticsData.AddPair("info", StringHelper::Format("%d", req.statusCode));
                analyticsData.AddPair("url", req.url);
                DGAnalytics::Instance()->SendDataEvent(analyticsData, true, AnalyticsEventGroups::Group::Framework, 2);
            }
        } else {
            DGAnalyticsData analyticsData("NKNetworkingError");
            analyticsData.AddPair("type", sNKError_HttpRequest);
            analyticsData.AddPair("info", StringHelper::Format("%d", req.statusCode));
            analyticsData.AddPair("url", req.url);
            DGAnalytics::Instance()->SendDataEvent(analyticsData, true, AnalyticsEventGroups::Group::Framework, 2);
            response.error.details.reason = StringHelper::Format("Request Error: Http Code %i", req.statusCode);
        }

        error = new NKError(NKErrorType::VALUE3, response.error.details.reason, response.error.type, "");
    } else if (!VerifyResponse(response.data, response.sig)) {
        if (response.error.details.reason.empty()) {
            response.error.details.reason = "Invalid Payload";
        }

        DGAnalyticsData analyticsData("NKNetworkingError");
        analyticsData.AddPair("type", sNKError_HttpSecurity);
        analyticsData.AddPair("info", response.error.details.reason);
        analyticsData.AddPair("url", req.url);
        DGAnalytics::Instance()->SendDataEvent(analyticsData, true, AnalyticsEventGroups::Group::Framework, 2);

        error = new NKError(NKErrorType::VALUE8, response.error.details.reason, response.error.type, "");
    }

    if (!error || error->reason != "ERR_API_IS_NOT_AVAILABLE" || ctx.retries > 9) {
        if (!ctx.callback.empty()) {
            ctx.callback(error, response, ctx);
        }
    } else {
        ++ctx.retries;
        RequestAgain(ctx);
    }
}
