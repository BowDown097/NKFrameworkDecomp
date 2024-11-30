#include "NKAction_SessionHttpRequest.h"
#include "NewFramework/Analytics/DGAnalytics.h"
#include "NewFramework/Networking/NKAPI/NKSignature.h"
#include "NewFramework/Networking/Protocols/HTTP/HttpCallbackFunctor.h"
#include "NewFramework/Utilities/StringHelper.h"
#include "Uncategorized/Blackboards.h"

void BA_HttpRequestAction::Start(BehaviourTree::IBlackboard* blackboard) {
    state = BehaviourTree::AState::Running;
    this->blackboard = dynamic_cast<NKSessionBlackboard*>(blackboard);
}

void BA_HttpRequestAction::Stop(BehaviourTree::IBlackboard* blackboard) {
    BehaviourTree::Action::Stop(blackboard);
    callback.reset();
}

void BA_HttpRequestAction::SendHttpRequest() {
    NKMessage message = ConstructMessage();
    std::map<std::string, std::string> headers = ConstructHeader();
    std::string url = ConstructUrl();
    HTTP_METHOD method = GetHTTPMethod();
    std::string messageData = NKJSON::Serialise<NKMessage>(message);

    std::shared_ptr<HttpCallbackFunctor> functor = std::make_shared<HttpCallbackFunctor>();
    functor->httpCallback = [this](const SHttpRequest& req) {
        if (req.field_B8 == 4) {
            HttpComplete(req);
        }
        else {
            HttpFailed(req);
        }
    };
    functor->field_40 = functor;
    callback = functor->field_40;

    std::string callbackKey = HttpCallbackKey();
    SHttpTimeoutOptions opts;
    opts.lowSpeedLimit = 0;
    opts.lowSpeedTime = 0;
    opts.connectTimeout = 0;
    opts.timeout = 15;

    int result = NKEndpoints::sendMessage(url, headers, messageData, callbackKey, method, nullptr, opts);
    if (result != 0) {
        return;
    }

    blackboard->LogMsg("Failed sending message to " + url);
    blackboard->error = NKError(NKErrorType::VALUE2, "Http Request Error", "", "Couldn't send Http request");
    state = BehaviourTree::AState::Failure;
}

void BA_HttpRequestAction::HttpComplete(const SHttpRequest& req) {
    if (state != BehaviourTree::AState::Running) {
        return;
    }

    std::string callbackKey = HttpCallbackKey();
    if (callbackKey != req.callbackKey) {
        return;
    }

    std::string downloadedData = req.GetDownloadedDataStr();
    NKMessageResponse messageResponse;

    if (!NKJSON::TryParse<NKMessageResponse>(messageResponse, downloadedData)) {
        std::string message = "HttpRequest: Couldn't parse response (Callback Key: " + req.callbackKey;
        message += ")";
        blackboard->LogMsg(message);

        DGAnalyticsData analyticsData("NKNetworkingError");
        analyticsData.AddPair("type", sNKError_HttpParse);
        analyticsData.AddPair("info", "couldn't parse response");
        analyticsData.AddPair("url", req.url);
        DGAnalytics::Instance()->SendDataEvent(analyticsData, true, AnalyticsEventGroups::Group::Framework, 2);

        blackboard->error = NKError(NKErrorType::VALUE3, "", "", "");
        state = BehaviourTree::AState::Failure;
        return;
    }

    if (messageResponse.error.type.empty()) {
        if (!NKSignature::Verify("", blackboard->privateKey, messageResponse.data, messageResponse.sig)) {
            std::string message = "HttpRequest: Couldn't verify response signature (Callback Key: " + req.callbackKey;
            message += ", Signature: ";
            message += messageResponse.sig;
            message += ")";

            blackboard->LogMsg(message);
            blackboard->error = NKError(NKErrorType::VALUE5, "Security Error", "", "Couldn't verify response signature");

            DGAnalyticsData analyticsData("NKNetworkingError");
            analyticsData.AddPair("type", sNKError_HttpSecurity);
            analyticsData.AddPair("info", "Invalid Payload");
            analyticsData.AddPair("url", req.url);
            DGAnalytics::Instance()->SendDataEvent(analyticsData, true, AnalyticsEventGroups::Group::Framework, 2);

            blackboard->error = NKError(NKErrorType::VALUE3, "", "", "");
            state = BehaviourTree::AState::Failure;
            return;
        }

        ParseResponse(messageResponse);
        state = BehaviourTree::AState::Success;
        return;
    }

    std::string message = "HttpRequest: Server reported an error: \n\t Type: " + messageResponse.error.type;
    message += "\n\t Reason: ";
    message += messageResponse.error.details.reason;
    message += "\n\t Fix: ";
    message += messageResponse.error.details.fix;
    blackboard->LogMsg(message);

    DGAnalyticsData analyticsData("NKNetworkingError");
    analyticsData.AddPair("type", sNKError_HttpLink);
    analyticsData.AddPair("info", messageResponse.error.type);
    analyticsData.AddPair("url", req.url);
    DGAnalytics::Instance()->SendDataEvent(analyticsData, true, AnalyticsEventGroups::Group::Framework, 2);

    blackboard->error = NKError(NKErrorType::VALUE3, "", "", "");
    blackboard->error.typeStr = messageResponse.error.type;
    blackboard->error.reason = messageResponse.error.details.reason;
    blackboard->error.fix = messageResponse.error.details.fix;

    state = BehaviourTree::AState::Failure;
}

void BA_HttpRequestAction::HttpFailed(const SHttpRequest& req) {
    if (state != BehaviourTree::AState::Running) {
        return;
    }

    std::string errorString = req.GetErrorString();
    blackboard->LogMsg("HttpRequest Failed: Error = " + errorString);
    blackboard->error = NKError(NKErrorType::VALUE2, "Http Error", "", errorString, req.statusCode);

    if (req.statusCode < 500) {
        if ((unsigned int)(req.error - 1) < 3) {
            state = BehaviourTree::AState::Failure;
            return;
        }

        if (req.error) {
            DGAnalyticsData analyticsData("NKNetworkingError");
            analyticsData.AddPair("type", sNKError_HttpError);
            analyticsData.AddPair("info", req.GetErrorString());
            analyticsData.AddPair("url", req.url);
            DGAnalytics::Instance()->SendDataEvent(analyticsData, true, AnalyticsEventGroups::Group::Framework, 2);
            state = BehaviourTree::AState::Failure;
            return;
        } else {
            DGAnalyticsData analyticsData("NKNetworkingError");
            analyticsData.AddPair("type", sNKError_HttpRequest);
            analyticsData.AddPair("info", StringHelper::Format("%d", req.statusCode));
            analyticsData.AddPair("url", req.url);
            DGAnalytics::Instance()->SendDataEvent(analyticsData, true, AnalyticsEventGroups::Group::Framework, 2);
            state = BehaviourTree::AState::Failure;
            return;
        }
    }

    blackboard->error.type = NKErrorType::VALUE3;
    std::string downloadedData = req.GetDownloadedDataStr();
    NKMessageResponse messageResponse;

    if (NKJSON::TryParse<NKMessageResponse>(messageResponse, downloadedData) && messageResponse.error.type.empty()) {
        DGAnalyticsData analyticsData("NKNetworkingError");
        analyticsData.AddPair("type", sNKError_HttpLink);
        analyticsData.AddPair("info", StringHelper::Format("%d", req.statusCode));
        analyticsData.AddPair("url", req.url);
        DGAnalytics::Instance()->SendDataEvent(analyticsData, true, AnalyticsEventGroups::Group::Framework, 2);
        state = BehaviourTree::AState::Failure;
        return;
    }

    if (!messageResponse.error.type.empty()) {
        if (messageResponse.error.type == CANNOT_FIND_LINK) {
            std::string message = "HttpRequest: Server reported an error: \n\t Type: " + messageResponse.error.type;
            message += "\n\t Reason: " + messageResponse.error.details.reason;
            message += "\n\t Fix: " + messageResponse.error.details.fix;
            blackboard->LogMsg(message);
            state = BehaviourTree::AState::Failure;
            return;
        }

        DGAnalyticsData analyticsData("NKNetworkingError");
        analyticsData.AddPair("type", sNKError_HttpLink);
        analyticsData.AddPair("info", StringHelper::Format("%d", req.statusCode) + " : " + messageResponse.error.type);
        analyticsData.AddPair("url", req.url);
        DGAnalytics::Instance()->SendDataEvent(analyticsData, true, AnalyticsEventGroups::Group::Framework, 2);

        blackboard->error.typeStr = messageResponse.error.type;
    }

    blackboard->error.reason = messageResponse.error.details.reason;
    blackboard->error.fix = messageResponse.error.details.fix;

    if (messageResponse.error.type.empty()) {
        blackboard->LogMsg("HttpRequest: Couldn't parse response (Callback Key: " + req.callbackKey + ")");

        DGAnalyticsData analyticsData("NKNetworkingError");
        analyticsData.AddPair("type", sNKError_HttpLink);
        analyticsData.AddPair("info", StringHelper::Format("%d", req.statusCode) + " : " + messageResponse.error.type);
        analyticsData.AddPair("url", req.url);
        DGAnalytics::Instance()->SendDataEvent(analyticsData, true, AnalyticsEventGroups::Group::Framework, 2);
    }
    else {
        std::string message = "HttpRequest: Server reported an error: \n\t Type: " + messageResponse.error.type;
        message += "\n\t Reason: " + messageResponse.error.details.reason;
        message += "\n\t Fix: " + messageResponse.error.details.fix;
        blackboard->LogMsg(message);
    }

    state = BehaviourTree::AState::Failure;
}
