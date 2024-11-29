#include "NKAction_GetUser.h"
#include "NewFramework/Analytics/DGAnalytics.h"
#include "NewFramework/Networking/NKAPI/Impl/NKMessages.h"
#include "NewFramework/Networking/NKAPI/NKManager.h"
#include "NewFramework/Networking/NKAPI/NKSignature.h"
#include "NewFramework/Platform/Shared/Core.h"
#include "Uncategorized/Blackboards.h"

std::string BA_GetUser::HttpCallbackKey() {
    return "NK-User";
}

std::map<std::string, std::string> BA_GetUser::ConstructHeader() {
    return {};
}

NKMessage BA_GetUser::ConstructMessage() {
    NKMessage out;
    out.data = "{}";
    out.auth.session = blackboard->accessToken.token;
    out.auth.device = DGAnalytics::Instance()->GetNonLiNKID();
    out.auth.appID = blackboard->appID;
    out.auth.skuID = blackboard->skuID;
    out.nonce = NKSignature::GenerateNonce(NKManager::GetManager()->constructTime, CCore::GetDeviceBootTime());
    out.sig = NKSignature::Generate(blackboard->accessToken.token, blackboard->privateKey, out.data, out.nonce);
    return out;
}

std::string BA_GetUser::ConstructUrl() {
    return NKEndpoints::GetBase(blackboard->serverCluster) + NKEndpoints::GetUserCurrent();
}

void BA_GetUser::ParseResponse(const NKMessageResponse& response) {
    NKResponseUserCurrent userCurrent;
    if (!NKJSON::TryParse<NKResponseUserCurrent>(userCurrent, response.data)) {
        throw std::runtime_error("Failed to parse NKResponseUserCurrent");
    }

    blackboard->responseUser = userCurrent.user;
    blackboard->LogMsg("Success.");
}

void BA_GetUser::Start(BehaviourTree::IBlackboard* blackboard) {
    BA_HttpRequestAction::Start(blackboard);

    this->blackboard->LogMsg("Trying to get user information for this session");
    SendHttpRequest();
}

BehaviourTree::Action* BA_GetUser::clone() {
    return new BA_GetUser(*this);
}
