#include "NKAction_AuthenticateService.h"
#include "NewFramework/Networking/NKAPI/Impl/NKSessionImpl.h"
#include "Uncategorized/Blackboards.h"

BA_AuthenticateService* BA_AuthenticateService::Create(
    const eNKLoginService& loginServiceType, const eNKServiceAuthMethod& authMethod) {
    return new BA_AuthenticateService(loginServiceType, authMethod);
}

std::string BA_AuthenticateService::DebugString() {
    return GetLoginServiceStringFromEnum(loginServiceType);
}

void BA_AuthenticateService::Start(BehaviourTree::IBlackboard* blackboard) {
    state = BehaviourTree::AState::Running;
    this->blackboard = dynamic_cast<NKSessionBlackboard*>(blackboard);

    eNKLoginService localLoginServiceType = loginServiceType != eNKLoginService::InBlackboard
        ? loginServiceType : this->blackboard->loginService;
    loginService = this->blackboard->sessionImpl->GetLoginService(localLoginServiceType);

    std::string message = "Authenticating " + GetLoginServiceStringFromEnum(localLoginServiceType) + ".. (Can show UI : ";
    message += authMethod == eNKServiceAuthMethod::Email ? "True" : "False";
    message += ")";
    this->blackboard->LogMsg(message);

    if (authMethod == eNKServiceAuthMethod::Email) {
        this->blackboard->field_C0 = false;
    }

    if (loginService) {
        if (!loginService->IsUserAuthenticated()) {
            loginService->Attach(this);
            loginService->AuthenticateUser(authMethod);
            if (state == BehaviourTree::AState::Running) {
                return;
            }
        }
        else {
            state = BehaviourTree::AState::Success;
        }
    } else {
        std::string fix = "The App isn't set up to use " + GetLoginServiceStringFromEnum(localLoginServiceType);
        this->blackboard->error = NKError(NKErrorType::VALUE7, "Developer Error", "", fix);
        state = BehaviourTree::AState::Failure;
    }

    message = GetLoginServiceStringFromEnum(localLoginServiceType) + " Authentication ";
    message += state == BehaviourTree::AState::Success ? "Succeeded" : "Failed";
    this->blackboard->LogMsg(message);
}

void BA_AuthenticateService::Stop(BehaviourTree::IBlackboard* blackboard) {
    BehaviourTree::Action::Stop(blackboard);

    this->blackboard->field_C0 = true;
    if (loginService) {
        loginService->Detach(this);
    }
}

void BA_AuthenticateService::OnServiceLogin(const eNKServiceLoginResult& loginResult) {
    if (state != BehaviourTree::AState::Running) {
        return;
    }

    eNKLoginService localLoginServiceType = loginServiceType != eNKLoginService::InBlackboard
        ? loginServiceType : blackboard->loginService;

    std::string message = GetLoginServiceStringFromEnum(localLoginServiceType) + " Authentication ";
    message += loginResult == eNKServiceLoginResult::Success ? "Succeeded" : "Failed";
    this->blackboard->LogMsg(message);

    switch (loginResult) {
        case eNKServiceLoginResult::Success:
            state = BehaviourTree::AState::Success;
            break;
        case eNKServiceLoginResult::Failure:
            blackboard->error = NKError(NKErrorType::VALUE1, message, "", "Please try again.");
            state = BehaviourTree::AState::Failure;
            break;
    }
}

BehaviourTree::Action* BA_AuthenticateService::clone() {
    return new BA_AuthenticateService(*this);
}
