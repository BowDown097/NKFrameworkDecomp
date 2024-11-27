#include "NKAction_ServiceAction.h"
#include "NewFramework/Analytics/DGAnalytics.h"
#include "NewFramework/Networking/NKAPI/Impl/NKSessionImpl.h"
#include "NewFramework/Networking/NKAPI/NKManager.h"
#include "NewFramework/Networking/NKAPI/NKSignature.h"
#include "NewFramework/Platform/Shared/Core.h"
#include "Uncategorized/Blackboards.h"

eNKServiceAction BA_ServiceAction::GetAction() const {
    return serviceAction == eNKServiceAction::InBlackboard ? blackboard->serviceAction : serviceAction;
}

eNKLoginService BA_ServiceAction::GetService() const {
    return loginServiceType == eNKLoginService::InBlackboard ? blackboard->loginService : loginServiceType;
}

std::string BA_ServiceAction::DebugString() {
    return GetLoginServiceStringFromEnum(loginServiceType) + " : " + GetServiceActionStringFromEnum(serviceAction);
}

std::string BA_ServiceAction::HttpCallbackKey() {
    return "NK-" + GetServiceActionStringFromEnum(GetAction()) +
           "-" + GetLoginServiceStringFromEnum(loginService->GetServiceType());
}

std::map<std::string, std::string> BA_ServiceAction::ConstructHeader() {
    return {};
}

NKMessage BA_ServiceAction::ConstructMessage() {
    NKMessage out;
    out.auth.session = blackboard->accessToken.token;
    out.auth.device = DGAnalytics::Instance()->GetNonLiNKID();
    out.auth.appID = blackboard->appID;
    out.auth.skuID = blackboard->skuID;

    std::string actionData = loginService->SerialisedData_For(serviceAction);
    out.nonce = NKSignature::GenerateNonce(NKManager::GetManager()->constructTime, CCore::GetDeviceBootTime());
    out.sig = NKSignature::Generate(blackboard->accessToken.token, blackboard->privateKey, actionData, out.nonce);

    return out;
}

std::string BA_ServiceAction::ConstructUrl() {
    return NKEndpoints::GetBase(blackboard->serverCluster) + loginService->UrlExtension_For(serviceAction);
}

void BA_ServiceAction::ParseResponse(const NKMessageResponse& response) {
    switch (GetAction()) {
        case eNKServiceAction::Link: {
            NKResponseLink responseLink;
            if (!NKJSON::TryParse<NKResponseLink>(responseLink, response.data)) {
                throw std::runtime_error("Failed to parse NKResponseLink");
            }

            DGAnalyticsData data("NKAccountLinked");
            data << "loginService" << GetLoginServiceStringFromEnum(GetService());
            DGAnalytics::Instance()->SendDataEvent(data, true, AnalyticsEventGroups::Group::Framework, 2);
            break;
        }
        case eNKServiceAction::Login: {
            NKResponseLogin responseLogin;
            if (!NKJSON::TryParse<NKResponseLogin>(responseLogin, response.data)) {
                throw std::runtime_error("Failed to parse NKResponseLogin");
            }

            blackboard->accessToken = NKAccessToken(responseLogin.session);
            blackboard->responseUser = responseLogin.user;
            blackboard->loginServiceFromToken = GetService();
            break;
        }
        case eNKServiceAction::Create: {
            NKResponseCreate responseCreate;
            if (!NKJSON::TryParse<NKResponseCreate>(responseCreate, response.data)) {
                throw std::runtime_error("Failed to parse NKResponseCreate");
            }

            blackboard->responseUser = responseCreate.user;
            blackboard->newUser = true;
            break;
        }
        default:
            break;
    }

    blackboard->LogMsg(GetLoginServiceStringFromEnum(GetService()) + " " +
                       GetServiceActionStringFromEnum(GetAction()) + " " +
                       "succeeded.");
}

void BA_ServiceAction::Start(BehaviourTree::IBlackboard* blackboard) {
    BA_HttpRequestAction::Start(blackboard);

    loginService = this->blackboard->sessionImpl->GetLoginService(GetService());
    this->blackboard->LogMsg("Trying to " + GetServiceActionStringFromEnum(GetAction()) +
                             " " + GetLoginServiceStringFromEnum(GetService()) + " account..");

    if (!loginService) {
        std::string fix = "The App isn't set up to use " + GetLoginServiceStringFromEnum(GetService());
        this->blackboard->error = NKError(NKErrorType::VALUE7, "Developer Error", "", fix);
        state = BehaviourTree::AState::Failure;
        return;
    }

    if (loginService->IsUserAuthenticated()) {
        SendHttpRequest();
        return;
    }

    this->blackboard->LogMsg("Couldn't proceed, user is not logged in to " + GetLoginServiceStringFromEnum(GetService()));
    state = BehaviourTree::AState::Failure;
}

BehaviourTree::Action* BA_ServiceAction::clone() {
    return new BA_ServiceAction(*this);
}

BA_ServiceAction* BA_ServiceAction::Create(const eNKLoginService& loginServiceType, const eNKServiceAction& serviceAction) {
    return new BA_ServiceAction(loginServiceType, serviceAction);
}
