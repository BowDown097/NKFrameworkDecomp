#include "NKAction_IsServiceAuthenticated.h"
#include "NewFramework/Networking/NKAPI/Impl/NKSessionImpl.h"
#include "Uncategorized/Blackboards.h"

BA_IsServiceAuthenticated* BA_IsServiceAuthenticated::Create(const eNKLoginService& loginServiceType) {
    return new BA_IsServiceAuthenticated(loginServiceType);
}

std::string BA_IsServiceAuthenticated::DebugString() {
    return GetLoginServiceStringFromEnum(loginServiceType);
}

void BA_IsServiceAuthenticated::Start(BehaviourTree::IBlackboard* blackboard) {
    NKSessionBlackboard* sessionBlackboard = dynamic_cast<NKSessionBlackboard*>(blackboard);
    eNKLoginService localLoginServiceType = loginServiceType != eNKLoginService::InBlackboard
        ? loginServiceType : sessionBlackboard->loginService;
    loginService = sessionBlackboard->sessionImpl->GetLoginService(localLoginServiceType);

    if (loginService) {
        state = loginService->IsUserAuthenticated() ? BehaviourTree::AState::Success : BehaviourTree::AState::Failure;
    } else {
        state = BehaviourTree::AState::Failure;
        std::string fix = "The App isn't set up to use " + GetLoginServiceStringFromEnum(localLoginServiceType);
        sessionBlackboard->error = NKError(NKErrorType::VALUE7, "Developer Error", "", fix);
    }

    std::string message = "Checking " + GetLoginServiceStringFromEnum(localLoginServiceType) + " is Authenticated.. ";
    message += state == BehaviourTree::AState::Success ? "True" : "False";
    sessionBlackboard->LogMsg(message);
}

BehaviourTree::Action* BA_IsServiceAuthenticated::clone() {
    return new BA_IsServiceAuthenticated(*this);
}
