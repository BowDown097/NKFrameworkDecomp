#include "NKRequestFactory.h"
#include "NewFramework/Analytics/DGAnalytics.h"
#include "NewFramework/Networking/NKAPI/Impl/NKMessages.h"
#include "NewFramework/Networking/NKAPI/NKManager.h"
#include "NewFramework/Networking/NKAPI/NKSession.h"
#include "NewFramework/Networking/NKAPI/NKSignature.h"
#include "NewFramework/Platform/Shared/Core.h"

int NKRequestFactory::GenerateRequestFullURL(const std::string& endpoint, std::string data, HTTP_METHOD method,
                                             IHttpCallback* cb, long timeout, long) {
    return GenerateRequest(endpoint, data, method, cb, timeout, timeout);
}

int NKRequestFactory::GenerateRequest(const std::string& endpoint, std::string data, HTTP_METHOD method,
                                      IHttpCallback* cb, long timeout, long lowSpeedTime) {
    NKManager* manager = NKManager::GetManager();

    NKMessage message;

    message.auth.session = manager->GetSessionModule()->GetAccessToken().token;
    message.auth.device = DGAnalytics::Instance()->GetNonLiNKID();
    message.auth.appID = manager->GetAppID();
    message.auth.skuID = manager->GetSkuID();

    message.data = data;
    message.nonce = NKSignature::GenerateNonce(manager->constructTime, CCore::GetDeviceBootTime());
    message.sig = NKSignature::Generate(message.auth.session, manager->GetPrivateKey(), message.data, message.nonce);

    std::string messageData = NKJSON::Serialise<NKMessage>(message);

    SHttpTimeoutOptions timeoutOptions;
    timeoutOptions.connectTimeout = 0;
    timeoutOptions.timeout = timeout != -1 ? timeout : 15;
    timeoutOptions.lowSpeedLimit = 1024;
    timeoutOptions.lowSpeedTime = lowSpeedTime != -1 ? lowSpeedTime : 10;

    std::map<std::string, std::string> headers;
    return NKEndpoints::sendMessage(endpoint, headers, messageData, "", method, cb, timeoutOptions);
}

int NKRequestFactory::GenerateRequestPartialURL(const std::string& endpoint, std::string data, HTTP_METHOD method,
                                                IHttpCallback* cb, long timeout, long) {
    std::string path = NKEndpoints::GetBase(NKManager::GetManager()->GetSessionModule()->GetServerCluster()) + endpoint;
    return GenerateRequest(path, data, method, cb, timeout, timeout);
}
