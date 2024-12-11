#include "NKAction_FileWebRequest.h"
#include "NewFramework/Analytics/DGAnalytics.h"
#include "NewFramework/Networking/NKAPI/Impl/NKFileClientImpl.h"
#include "NewFramework/Networking/NKAPI/NKManager.h"
#include "NewFramework/Networking/NKAPI/NKSignature.h"
#include "NewFramework/Platform/Shared/Core.h"
#include "NewFramework/Utilities/Encoding.h"
#include "NewFramework/Utilities/StringHelper.h"
#include "Uncategorized/Blackboards.h"

// speculative: avoids repeating code
// (could be in HttpRequest.h, but only time this operation is done is here,
// if IDA's list of xrefs to the method strings is to be trusted)
inline std::string GetMethodString(HTTP_METHOD method) {
    switch (method) {
        case METHOD_GET:
            return "GET";
        case METHOD_POST:
            return "POST";
        case METHOD_HEAD:
            return "HEAD";
        default:
            return "UNKNOWN";
    }
}

void BA_FileWebRequest::Start(BehaviourTree::IBlackboard* blackboard) {
    state = BehaviourTree::AState::Running;
    this->blackboard = dynamic_cast<SNKFileClientBlackboard*>(blackboard);
    CheckChildState(nullptr);

    std::map<std::string, std::string> headers;
    std::string messageData;

    if (SendsPostData()) {
        NKMessage message;
        message.auth.session = this->blackboard->accessToken.token;
        message.auth.device = DGAnalytics::Instance()->GetNonLiNKID();
        message.auth.appID = this->blackboard->appID;
        message.auth.skuID = this->blackboard->skuID;
        ConstructMessage(message.data);

        if (this->blackboard->error.type == NKErrorType::VALUE0) {
            message.nonce = NKSignature::GenerateNonce(NKManager::GetManager()->constructTime, CCore::GetDeviceBootTime());
            message.sig = NKSignature::Generate(message.auth.session, this->blackboard->privateKey, message.data, message.nonce);
            messageData = NKJSON::Serialise<NKMessage>(message);
        }
    }

    if (this->blackboard->error.type == NKErrorType::VALUE0) {
        int code = NKEndpoints::sendMessage(
            GetUrl(), headers, messageData, GetHttpCallbackKey(),
            GetHTTPMethod(), this, this->blackboard->timeoutOptions);

        if (code == 0) {
            this->blackboard->error = NKError(NKErrorType::VALUE1, "Sending a message gave an invalid request ID", "", "");
        }
    }

    if (this->blackboard->error.type > NKErrorType::VALUE0) {
        state = BehaviourTree::AState::Failure;
    }

    this->blackboard->fileClientImpl->ProcessBHTree(this->blackboard->action.lock(), this->blackboard);
}

void BA_FileWebRequest::WriteEtagToCache(
    const std::string& key, const NKMessageResponseFileStorage& storage, const std::string& dataHash) {
    NKFileClientImpl::FileMetaData metadata;
    metadata.etag = storage.etag;
    metadata.dataHash = dataHash;

    blackboard->fileClientImpl->metadataMap[key] = metadata;
    blackboard->fileClientImpl->SaveCachedFileData();
}

void BA_FileWebRequest::HttpComplete(const SHttpRequest& request) {
    if (state != BehaviourTree::AState::Running) {
        blackboard->error = NKError(NKErrorType::VALUE1, "Invalid task state", "", "");
        state = BehaviourTree::AState::Failure;
        blackboard->fileClientImpl->ProcessBHTree(blackboard->action.lock(), blackboard);
        return;
    }

    if (GetHttpCallbackKey() != request.callbackKey) {
        blackboard->error = NKError(NKErrorType::VALUE1, "Request callback key didn't match", "", "");
        state = BehaviourTree::AState::Failure;
        blackboard->fileClientImpl->ProcessBHTree(blackboard->action.lock(), blackboard);
    } else {
        blackboard->start = request.start;
        blackboard->file = ParseResponseMessage(request);
        blackboard->fileClientImpl->ProcessBHTree(blackboard->action.lock(), blackboard);
    }
}

NKMessageResponseFile BA_FileWebRequest::ParseResponseMessage(const SHttpRequest& request) {
    NKMessageResponseFile out;
    out.storage.filename = blackboard->fileName;
    out.storage.url = GetUrl();

    if (request.statusCode == 304) {
        out.storage.success = true;
        out.storage.etag = GetEtagFromHeaders(request);
        out.upToDate = true;
        return out;
    }

    if (!IsRendered()) {
        out.storage.success = true;
        out.storage.etag = GetEtagFromHeaders(request);
        out.storage.data = request.GetDownloadedDataStr();

        if (out.storage.etag.empty()) {
            DGAnalyticsData analyticsData("MissingEtag");
            analyticsData.AddPair("url", out.storage.url);
            analyticsData.AddPair("headers", request.GetResponseHeaderBase64());
            analyticsData.AddPair("method", GetMethodString(request.method));
            DGAnalytics::Instance()->SendDataEvent(analyticsData, true, AnalyticsEventGroups::Group::Framework, 2);
        }
    } else {
        NKMessageResponse messageResponse;
        if (!NKJSON::TryParse<NKMessageResponse>(messageResponse, request.GetDownloadedDataStr())) {
            if (!messageResponse.error.type.empty()) {
                DGAnalyticsData analyticsData("NKNetworkingError");
                analyticsData.AddPair("type", sNKError_HttpLink);
                analyticsData.AddPair("info", messageResponse.error.type);
                analyticsData.AddPair("url", request.url);
                DGAnalytics::Instance()->SendDataEvent(analyticsData, true, AnalyticsEventGroups::Group::Framework, 2);
            } else {
                DGAnalyticsData analyticsData("NKNetworkingError");
                analyticsData.AddPair("type", sNKError_HttpParse);
                analyticsData.AddPair("info", request.url);
                analyticsData.AddPair("url", request.url);
                DGAnalytics::Instance()->SendDataEvent(analyticsData, true, AnalyticsEventGroups::Group::Framework, 2);
            }

            blackboard->error = NKError(NKErrorType::VALUE4, "", "", "");
            state = BehaviourTree::AState::Failure;
            return out;
        }

        if (!messageResponse.error.type.empty()) {
            DGAnalyticsData analyticsData("NKNetworkingError");
            analyticsData.AddPair("type", sNKError_HttpLink);
            analyticsData.AddPair("info", messageResponse.error.type);
            analyticsData.AddPair("url", request.url);
            DGAnalytics::Instance()->SendDataEvent(analyticsData, true, AnalyticsEventGroups::Group::Framework, 2);

            blackboard->error = NKError(NKErrorType::VALUE4, "", "", "");
            state = BehaviourTree::AState::Failure;
            return out;
        }

        if (NKSignature::Verify("", blackboard->privateKey, messageResponse.data, messageResponse.sig)) {
            if (IsStorageResponse() && !NKJSON::TryParse(out, messageResponse.data)) {
                throw std::runtime_error("Failed to parse NKMessageResponseFile");
            } else {
                out.storage.success = true;
                out.storage.etag = GetEtagFromHeaders(request);
                out.storage.data = messageResponse.data;

                if (out.storage.etag.empty()) {
                    DGAnalyticsData analyticsData("MissingEtag");
                    analyticsData.AddPair("url", out.storage.url);
                    analyticsData.AddPair("headers", request.GetResponseHeaderBase64());
                    analyticsData.AddPair("method", GetMethodString(request.method));
                    DGAnalytics::Instance()->SendDataEvent(analyticsData, true, AnalyticsEventGroups::Group::Framework, 2);
                }
            }
        } else {
            DGAnalyticsData analyticsData("NKNetworkingError");
            analyticsData.AddPair("type", sNKError_HttpSecurity);
            analyticsData.AddPair("info", "Invalid Payload");
            analyticsData.AddPair("url", request.url);
            DGAnalytics::Instance()->SendDataEvent(analyticsData, true, AnalyticsEventGroups::Group::Framework, 2);
            blackboard->error = NKError(NKErrorType::VALUE5, "Signatures don't match", "", "");
            state = BehaviourTree::AState::Failure;
        }
    }

    if (state != BehaviourTree::AState::Failure && IsStorageResponse()) {
        if (!base64_to_string(out.storage.data, out.storage.data)) {
            throw std::runtime_error("Failed to b64 decode file data");
        }

        if (!decompress(out.storage.data.c_str(), out.storage.data.size(), out.storage.data)) {
            throw std::runtime_error("Failed to decompress file data");
        }
    }

    return out;
}

void BA_FileWebRequest::HttpFailed(const SHttpRequest& request) {
    if (state != BehaviourTree::AState::Running) {
        blackboard->error = NKError(NKErrorType::VALUE1, "Invalid task state", "", "");
        state = BehaviourTree::AState::Failure;
        blackboard->fileClientImpl->ProcessBHTree(blackboard->action.lock(), blackboard);
        return;
    }

    state = BehaviourTree::AState::Failure;
    blackboard->error = NKError(NKErrorType::VALUE2, request.GetErrorString(), "", "", request.statusCode);

    if (request.statusCode < 500) {
        if (request.statusCode == 403) {
            blackboard->error = NKError(NKErrorType::VALUE0, "", "", "");
            blackboard->file.storage.filename = blackboard->fileName;
            blackboard->file.storage.data.clear();
            blackboard->file.storage.success = false;
            blackboard->file.storage.url = GetUrl();
            blackboard->file.storage.etag.clear();
            state = BehaviourTree::AState::Running;

            HandleMessage();
            blackboard->fileClientImpl->ProcessBHTree(blackboard->action.lock(), blackboard);
            return;
        }

        if ((unsigned int)(request.error - 1) >= 3) {
            if (request.error) {
                DGAnalyticsData analyticsData("NKNetworkingError");
                analyticsData.AddPair("type", sNKError_HttpError);
                analyticsData.AddPair("info", request.GetErrorString());
                analyticsData.AddPair("url", request.url);
                DGAnalytics::Instance()->SendDataEvent(analyticsData, true, AnalyticsEventGroups::Group::Framework, 2);
            } else {
                DGAnalyticsData analyticsData("NKNetworkingError");
                analyticsData.AddPair("type", sNKError_HttpRequest);
                analyticsData.AddPair("info", StringHelper::Format("%d", request.statusCode));
                analyticsData.AddPair("url", request.url);
                DGAnalytics::Instance()->SendDataEvent(analyticsData, true, AnalyticsEventGroups::Group::Framework, 2);
            }
        }
    } else {
        blackboard->error.type = NKErrorType::VALUE3;
        NKMessageResponse messageResponse;

        if (!NKJSON::TryParse<NKMessageResponse>(messageResponse, request.GetDownloadedDataStr()) ||
            !messageResponse.error.type.empty()) {
            if (!messageResponse.error.type.empty()) {
                DGAnalyticsData analyticsData("NKNetworkingError");
                analyticsData.AddPair("type", sNKError_HttpLink);
                std::string info = StringHelper::Format(":%d", request.statusCode) + " : " + messageResponse.error.type;
                analyticsData.AddPair("info", info);
                analyticsData.AddPair("url", request.url);
                DGAnalytics::Instance()->SendDataEvent(analyticsData, true, AnalyticsEventGroups::Group::Framework, 2);

                blackboard->error.typeStr = messageResponse.error.type;
            }

            if (!messageResponse.error.details.reason.empty()) {
                blackboard->error.reason = messageResponse.error.details.reason;
            }

            if (!messageResponse.error.details.fix.empty()) {
                blackboard->error.fix = messageResponse.error.details.fix;
            }
        }

        if (messageResponse.error.type.empty()) {
            DGAnalyticsData analyticsData("NKNetworkingError");
            analyticsData.AddPair("type", sNKError_HttpLink);
            analyticsData.AddPair("info", StringHelper::Format("%d", request.statusCode));
            analyticsData.AddPair("url", request.url);
            DGAnalytics::Instance()->SendDataEvent(analyticsData, true, AnalyticsEventGroups::Group::Framework, 2);
        }
    }

    blackboard->fileClientImpl->ProcessBHTree(blackboard->action.lock(), blackboard);
}

std::string BA_FileWebRequest::GetEtagFromHeaders(const SHttpRequest& request) {
    auto it = request.responseHeaders.find("etag");
    return it != request.responseHeaders.end() ? strip_weak_etag_marker(it->second) : "";
}
