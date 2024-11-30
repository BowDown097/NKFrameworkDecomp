#include "NKSessionImpl.h"

#include "NewFramework/Analytics/DGAnalytics.h"
#include "NewFramework/BaseApplication/AppClass/NKSDKApp.h"
#include "NewFramework/IO/File/BaseFileIO.h"
#include "NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_AuthenticateService.h"
#include "NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_CacheSession.h"
#include "NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_CheckErrorType.h"
#include "NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_CheckLoginServiceType.h"
#include "NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_CheckOnline.h"
#include "NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_CheckServiceActionType.h"
#include "NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_ClearSession.h"
#include "NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_GetUser.h"
#include "NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_HideWebView.h"
#include "NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_IsServiceAuthenticated.h"
#include "NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_OpenSessionUsingToken.h"
#include "NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_ReportErrorCallback.h"
#include "NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_ServiceAction.h"
#include "NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_SetError.h"
#include "NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_ShowWebView.h"
#include "NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_StoreStateCallback.h"
#include "NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_WebViewErrorToolTip.h"
#include "NewFramework/Networking/NKAPI/Impl/SessionActions/NKAction_WebviewRequestListener.h"
#include "NewFramework/Networking/NKAPI/NKSession.h"
#include "NewFramework/Platform/Shared/Logging.h"
#include "Uncategorized/Blackboards.h"
#include "Uncategorized/StuffToRefactor.h"

#include <boost/bind/bind.hpp>

void NKSessionImpl::Initialise(const int& appID, const int& skuID, const std::string& privateKey,
                               CEventManager* eventManager, eNKLinkServerCluster serverCluster,
                               LoginCallback loginCallback) {
    this->state = NKSessionState::VALUE6;
    this->appID = appID;
    this->skuID = skuID;
    this->privateKey = privateKey;
    this->eventManager = eventManager;
    this->serverCluster = serverCluster;
    this->accessToken = NKAccessToken();
    this->responseUser = NKResponseUser();
    this->loginCallback = loginCallback;

    if (appID > 0 && skuID > 0 && !privateKey.empty()) {
        this->appInfoValid = true;
    }
}

void NKSessionImpl::Open(bool needSession, bool a3, bool a6) {
    if (!appInfoValid) {
        return;
    }

    if (state == NKSessionState::VALUE1) {
        if (!loginCallback.empty()) {
            loginCallback(session, state, NKError());
        }
        return;
    }

    if (!needSession) {
        LoginUsingService(eNKLoginService::Steam, false, !a6, a3); // TODO: figure out how login service is acquired
        return;
    }

    BehaviourTree::Selector* selector = BehaviourTree::Selector::Create(BehaviourTree::LoopCondition::None);

    BA_IsServiceAuthenticated* isServiceAuthenticated = BA_IsServiceAuthenticated::Create(eNKLoginService::InBlackboard);
    selector->AddAction(isServiceAuthenticated);

    BA_AuthenticateService* authenticateService = BA_AuthenticateService::Create(
        eNKLoginService::InBlackboard, eNKServiceAuthMethod::Email);
    selector->AddAction(authenticateService);

    BehaviourTree::Selector* selector2 = BehaviourTree::Selector::Create(BehaviourTree::LoopCondition::None);

    BA_ServiceAction* loginAction = BA_ServiceAction::Create(eNKLoginService::InBlackboard, eNKServiceAction::Login);
    selector2->AddAction(loginAction);

    BehaviourTree::Sequence* sequence = BehaviourTree::Sequence::Create(BehaviourTree::LoopCondition::None);

    BehaviourTree::Conditional<BehaviourTree::ABool>* a3Conditional = new BehaviourTree::Conditional<BehaviourTree::ABool>;
    a3Conditional->condition = static_cast<BehaviourTree::ABool>(a3);
    sequence->AddAction(a3Conditional);

    BA_CheckErrorType* checkCannotFindLink = BA_CheckErrorType::Create(CANNOT_FIND_LINK);
    sequence->AddAction(checkCannotFindLink);

    BA_ServiceAction* createAction = BA_ServiceAction::Create(eNKLoginService::InBlackboard, eNKServiceAction::Create);
    sequence->AddAction(createAction);

    BA_ServiceAction* loginAction2 = BA_ServiceAction::Create(eNKLoginService::InBlackboard, eNKServiceAction::Login);
    sequence->AddAction(loginAction2);
    selector2->AddAction(sequence);

    BehaviourTree::Sequence* sequence2 = BehaviourTree::Sequence::Create(BehaviourTree::LoopCondition::None);

    BehaviourTree::Sequence* sequence3 = BehaviourTree::Sequence::Create(BehaviourTree::LoopCondition::None);

    BA_CheckOnline* checkOnline = BA_CheckOnline::Create(appInterface);
    sequence->AddAction(checkOnline);

    BehaviourTree::Selector* selector3 = BehaviourTree::Selector::Create(BehaviourTree::LoopCondition::None);

    BehaviourTree::Sequence* sequence4 = BehaviourTree::Sequence::Create(BehaviourTree::LoopCondition::None);

    BehaviourTree::Parallel* parallel = BehaviourTree::Parallel::Create(BehaviourTree::LoopCondition::None, true, true);

    std::string loginEndpoint = NKEndpoints::GetLoginWebView(serverCluster);
    BA_ShowWebView* showWebView = BA_ShowWebView::Create(appInterface, loginEndpoint, GetWebViewWidth(), GetWebViewHeight());
    parallel->AddAction(showWebView);

    BehaviourTree::Sequence* sequence5 = BehaviourTree::Sequence::Create(BehaviourTree::LoopCondition::None);

    BA_WebviewRequestListener* webviewListener = BA_WebviewRequestListener::Create();
    sequence5->AddAction(webviewListener);

    BehaviourTree::NotDecorator* serviceCheckNotDecorator = new BehaviourTree::NotDecorator;
    serviceCheckNotDecorator->child = BA_CheckLoginServiceType::Create(eNKLoginService::None);
    sequence5->AddAction(serviceCheckNotDecorator);

    BA_CheckServiceActionType* loginActionCheck = BA_CheckServiceActionType::Create(eNKServiceAction::Login);
    sequence5->AddAction(loginActionCheck);

    BehaviourTree::Selector* selector4 = BehaviourTree::Selector::Create(BehaviourTree::LoopCondition::None);

    BehaviourTree::Sequence* sequence6 = BehaviourTree::Sequence::Create(BehaviourTree::LoopCondition::None);
    sequence6->AddAction(selector);
    sequence6->AddAction(selector2);
    selector4->AddAction(sequence6);

    BehaviourTree::ForceState* errorToolTipFailure = new BehaviourTree::ForceState;
    errorToolTipFailure->child = BA_WebViewErrorToolTip::Create();
    errorToolTipFailure->forceState = BehaviourTree::AState::Failure;
    selector4->AddAction(errorToolTipFailure);
    sequence5->AddAction(selector4);

    BehaviourTree::Loop* sequence5Loop = new BehaviourTree::Loop;
    sequence5Loop->child = sequence5;
    sequence5Loop->loopCondition = BehaviourTree::LoopCondition::LoopOnFailure;
    parallel->AddAction(sequence5Loop);
    sequence4->AddAction(parallel);

    BA_HideWebView* hideWebView = BA_HideWebView::Create();
    sequence4->AddAction(hideWebView);

    BehaviourTree::ForceState* cacheSessionSuccess = new BehaviourTree::ForceState;
    cacheSessionSuccess->child = BA_CacheSession::Create(STUFF_TO_REFACTOR::s_FileIO);
    cacheSessionSuccess->forceState = BehaviourTree::AState::Success;
    sequence4->AddAction(cacheSessionSuccess);
    selector3->AddAction(sequence4);

    BA_HideWebView* hideWebView2 = BA_HideWebView::Create();
    selector3->AddAction(hideWebView2);
    sequence3->AddAction(selector3);

    BehaviourTree::ForceState* sequence3Success = new BehaviourTree::ForceState;
    sequence3Success->child = sequence3;
    sequence3Success->forceState = BehaviourTree::AState::Success;
    sequence2->AddAction(sequence3Success);

    BA_ReportErrorCallback* reportErrorCb = BA_ReportErrorCallback::Create(
        boost::bind(&NKSessionImpl::ReportError, this, boost::placeholders::_1));
    sequence2->AddAction(reportErrorCb);

    BA_StoreStateCallback* storeStateCb = BA_StoreStateCallback::Create(
        boost::bind(&NKSessionImpl::StoreState, this, boost::placeholders::_1));
    sequence2->AddAction(storeStateCb);

    auto openRequest = boost::make_shared<NKSessionImpl::SSessionOpenRequest>();
    openRequest->action = boost::shared_ptr<BehaviourTree::Composite>(sequence2);

    tasks.push_back(openRequest);
}

int NKSessionImpl::GetWebViewWidth() const {
    return appInterface->GetScreenW() * 0.85f * appInterface->GetViewScale();
}

int NKSessionImpl::GetWebViewHeight() const {
    return appInterface->GetScreenH() * 0.85f * appInterface->GetViewScale();
}

void NKSessionImpl::StoreState(const BehaviourTree::IBlackboard* blackboard) {
    if (!blackboard) {
        return;
    }

    const NKSessionBlackboard* sessionBlackboard = dynamic_cast<const NKSessionBlackboard*>(blackboard);
    NKSessionState bbState = NKSessionState::VALUE7;

    if (sessionBlackboard->accessToken.IsValid()) {
        bbState = sessionBlackboard->newUser ? NKSessionState::VALUE2 : NKSessionState::VALUE3;
        if (accessToken.IsValid()) {
            if (accessToken.token != sessionBlackboard->accessToken.token) {
                bbState = sessionBlackboard->newUser ? NKSessionState::VALUE5 : NKSessionState::VALUE6;
            } else {
                bbState = NKSessionState::VALUE3;
            }
        }
    }

    accessToken = sessionBlackboard->accessToken;
    responseUser = sessionBlackboard->responseUser;

    if (sessionBlackboard->accessToken.IsValid() && bbState != NKSessionState::VALUE3) {
        DGAnalyticsData analyticsData("nkSessionStateChange");
        analyticsData << "newUser" << sessionBlackboard->newUser;
        analyticsData << "service" << GetLoginServiceStringFromEnum(sessionBlackboard->loginServiceFromToken);
        analyticsData << "uiWasShown" << sessionBlackboard->uiWasShown;
        DGAnalytics::Instance()->SendDataEvent(analyticsData, true, AnalyticsEventGroups::Group::Framework, 2);
    }

    state = bbState >= NKSessionState::VALUE6 ? bbState : NKSessionState::VALUE1;

    if (!loginCallback.empty()) {
        loginCallback(session, bbState, lastError);
    }
}

void NKSessionImpl::LoginUsingService(const eNKLoginService& loginServiceType, bool authWithEmail, bool a4, bool a5) {
    BehaviourTree::Selector* selector = BehaviourTree::Selector::Create(BehaviourTree::LoopCondition::None);

    BA_IsServiceAuthenticated* isServiceAuthenticated = BA_IsServiceAuthenticated::Create(loginServiceType);
    selector->AddAction(isServiceAuthenticated);

    BehaviourTree::Parallel* parallel = BehaviourTree::Parallel::Create(BehaviourTree::LoopCondition::None, true, true);

    BehaviourTree::Sequence* sequence = BehaviourTree::Sequence::Create(BehaviourTree::LoopCondition::None);

    BehaviourTree::Wait* wait = new BehaviourTree::Wait;
    wait->min = 15;
    wait->max = 0;
    sequence->AddAction(wait);

    NKError error(NKErrorType::VALUE1, "Service Authentication Timeout", "AUTH_TIMEOUT", "");
    BA_SetError* setTimeoutError = BA_SetError::Create(error);
    sequence->AddAction(setTimeoutError);

    BehaviourTree::ForceState* sequenceFailure = new BehaviourTree::ForceState;
    sequenceFailure->child = sequence;
    sequenceFailure->forceState = BehaviourTree::AState::Failure;
    parallel->AddAction(sequenceFailure);

    BA_AuthenticateService* authenticateService = BA_AuthenticateService::Create(
        loginServiceType, authWithEmail ? eNKServiceAuthMethod::Email : eNKServiceAuthMethod::Platform);
    parallel->AddAction(authenticateService);
    selector->AddAction(parallel);

    BehaviourTree::Composite* loginComposite;
    if (!a4) {
        loginComposite = BehaviourTree::Sequence::Create(BehaviourTree::LoopCondition::None);

        BehaviourTree::Conditional<BehaviourTree::ABool>* a5Conditional = new BehaviourTree::Conditional<BehaviourTree::ABool>;
        a5Conditional->condition = static_cast<BehaviourTree::ABool>(a5);
        loginComposite->AddAction(a5Conditional);

        BA_ServiceAction* createAction = BA_ServiceAction::Create(loginServiceType, eNKServiceAction::Create);
        loginComposite->AddAction(createAction);

        BA_ServiceAction* loginAction = BA_ServiceAction::Create(loginServiceType, eNKServiceAction::Login);
        loginComposite->AddAction(loginAction);
    } else {
        loginComposite = BehaviourTree::Selector::Create(BehaviourTree::LoopCondition::None);

        BA_ServiceAction* loginAction = BA_ServiceAction::Create(loginServiceType, eNKServiceAction::Login);
        loginComposite->AddAction(loginAction);

        BehaviourTree::Sequence* sequence3 = BehaviourTree::Sequence::Create(BehaviourTree::LoopCondition::None);

        BehaviourTree::Conditional<BehaviourTree::ABool>* a5Conditional = new BehaviourTree::Conditional<BehaviourTree::ABool>;
        a5Conditional->condition = static_cast<BehaviourTree::ABool>(a5);
        sequence3->AddAction(a5Conditional);

        BA_CheckErrorType* checkCannotFindLink = BA_CheckErrorType::Create(CANNOT_FIND_LINK);
        sequence3->AddAction(checkCannotFindLink);

        BA_ServiceAction* createAction = BA_ServiceAction::Create(loginServiceType, eNKServiceAction::Create);
        sequence3->AddAction(createAction);

        BA_ServiceAction* loginAction2 = BA_ServiceAction::Create(loginServiceType, eNKServiceAction::Login);
        sequence3->AddAction(loginAction2);
        loginComposite->AddAction(sequence3);
    }

    BehaviourTree::Sequence* sequence4 = BehaviourTree::Sequence::Create(BehaviourTree::LoopCondition::None);

    BehaviourTree::Sequence* sequence5 = BehaviourTree::Sequence::Create(BehaviourTree::LoopCondition::None);

    BehaviourTree::Selector* selector3 = BehaviourTree::Selector::Create(BehaviourTree::LoopCondition::None);

    BehaviourTree::Sequence* sequence6 = BehaviourTree::Sequence::Create(BehaviourTree::LoopCondition::None);

    BA_OpenSessionUsingToken* openSessionUsingToken = BA_OpenSessionUsingToken::Create(STUFF_TO_REFACTOR::s_FileIO);
    sequence6->AddAction(openSessionUsingToken);

    BehaviourTree::ForceState* getUserSuccess = new BehaviourTree::ForceState;
    getUserSuccess->child = new BA_GetUser;
    getUserSuccess->forceState = BehaviourTree::AState::Success;
    sequence6->AddAction(getUserSuccess);

    BehaviourTree::Selector* selector4 = BehaviourTree::Selector::Create(BehaviourTree::LoopCondition::None);

    BehaviourTree::NotDecorator* checkNotLoggedInNotDecorator = new BehaviourTree::NotDecorator;
    checkNotLoggedInNotDecorator->child = BA_CheckErrorType::Create("ERR_NOT_LOGGED_IN");
    selector4->AddAction(checkNotLoggedInNotDecorator);

    BehaviourTree::ForceState* clearSessionFailure = new BehaviourTree::ForceState;
    clearSessionFailure->child = new BA_ClearSession;
    clearSessionFailure->forceState = BehaviourTree::AState::Failure;
    selector4->AddAction(clearSessionFailure);
    sequence6->AddAction(selector4);
    selector3->AddAction(sequence6);

    BehaviourTree::Sequence* sequence7 = BehaviourTree::Sequence::Create(BehaviourTree::LoopCondition::None);
    sequence7->AddAction(selector);
    sequence7->AddAction(loginComposite);
    selector3->AddAction(sequence7);
    sequence5->AddAction(selector3);

    BehaviourTree::ForceState* cacheSessionSuccess = new BehaviourTree::ForceState;
    cacheSessionSuccess->child = BA_CacheSession::Create(STUFF_TO_REFACTOR::s_FileIO);
    cacheSessionSuccess->forceState = BehaviourTree::AState::Success;
    sequence5->AddAction(cacheSessionSuccess);

    BehaviourTree::ForceState* sequence5Success = new BehaviourTree::ForceState;
    sequence5Success->child = sequence5;
    sequence5Success->forceState = BehaviourTree::AState::Success;
    sequence4->AddAction(sequence5Success);

    BA_ReportErrorCallback* reportErrorCb = BA_ReportErrorCallback::Create(
        boost::bind(&NKSessionImpl::ReportError, this, boost::placeholders::_1));
    sequence4->AddAction(reportErrorCb);

    BA_StoreStateCallback* storeStateCb = BA_StoreStateCallback::Create(
        boost::bind(&NKSessionImpl::StoreState, this, boost::placeholders::_1));
    sequence4->AddAction(storeStateCb);

    auto openRequest = boost::make_shared<NKSessionImpl::SSessionOpenRequest>();
    openRequest->action = boost::shared_ptr<BehaviourTree::Composite>(sequence4);

    tasks.push_back(openRequest);
}

void NKSessionImpl::LinkService(const eNKLoginService& loginServiceType, bool authWithEmail) {
    BehaviourTree::Selector* authSelector = BehaviourTree::Selector::Create(BehaviourTree::LoopCondition::None);

    BA_IsServiceAuthenticated* isServiceAuthenticated = BA_IsServiceAuthenticated::Create(loginServiceType);
    authSelector->AddAction(isServiceAuthenticated);

    BA_AuthenticateService* authenticateService = BA_AuthenticateService::Create(
        loginServiceType, authWithEmail ? eNKServiceAuthMethod::Email : eNKServiceAuthMethod::Platform);
    authSelector->AddAction(authenticateService);

    BehaviourTree::Sequence* masterSequence = BehaviourTree::Sequence::Create(BehaviourTree::LoopCondition::None);

    BehaviourTree::Sequence* linkSequence = BehaviourTree::Sequence::Create(BehaviourTree::LoopCondition::None);
    linkSequence->AddAction(authSelector);

    BA_ServiceAction* serviceAction = BA_ServiceAction::Create(loginServiceType, eNKServiceAction::Link);
    linkSequence->AddAction(serviceAction);

    BehaviourTree::ForceState* getUserSuccess = new BehaviourTree::ForceState;
    getUserSuccess->child = new BA_GetUser;
    getUserSuccess->forceState = BehaviourTree::AState::Success;
    linkSequence->AddAction(getUserSuccess);

    BehaviourTree::ForceState* linkSequenceSuccess = new BehaviourTree::ForceState;
    linkSequenceSuccess->child = linkSequence;
    linkSequenceSuccess->forceState = BehaviourTree::AState::Success;
    masterSequence->AddAction(linkSequenceSuccess);

    BA_ReportErrorCallback* reportErrorCb = BA_ReportErrorCallback::Create(
        boost::bind(&NKSessionImpl::ReportError, this, boost::placeholders::_1));
    masterSequence->AddAction(reportErrorCb);

    BA_StoreStateCallback* storeStateCb = BA_StoreStateCallback::Create(
        boost::bind(&NKSessionImpl::StoreState, this, boost::placeholders::_1));
    masterSequence->AddAction(storeStateCb);

    auto linkRequest = boost::make_shared<NKSessionImpl::SSessionLinkRequest>();
    linkRequest->action = boost::shared_ptr<BehaviourTree::Composite>(masterSequence);

    tasks.push_back(linkRequest);
}

void NKSessionImpl::ShowUI() {
    if (!appInfoValid) {
        return;
    }

    BehaviourTree::Sequence* sequence = BehaviourTree::Sequence::Create(BehaviourTree::LoopCondition::None);

    BehaviourTree::Sequence* sequence2 = BehaviourTree::Sequence::Create(BehaviourTree::LoopCondition::None);

    BA_CheckOnline* checkOnline = BA_CheckOnline::Create(appInterface);
    sequence2->AddAction(checkOnline);

    BehaviourTree::Selector* selector = BehaviourTree::Selector::Create(BehaviourTree::LoopCondition::None);

    BehaviourTree::Sequence* sequence3 = BehaviourTree::Sequence::Create(BehaviourTree::LoopCondition::None);

    BehaviourTree::Parallel* parallel = BehaviourTree::Parallel::Create(BehaviourTree::LoopCondition::None, true, true);

    std::string profileEndpoint = NKEndpoints::GetProfileWebView(serverCluster);
    BA_ShowWebView* showWebView = BA_ShowWebView::Create(appInterface, profileEndpoint, GetWebViewWidth(), GetWebViewHeight());
    parallel->AddAction(showWebView);

    BehaviourTree::Sequence* sequence4 = BehaviourTree::Sequence::Create(BehaviourTree::LoopCondition::None);

    BA_WebviewRequestListener* webviewListener = BA_WebviewRequestListener::Create();
    sequence4->AddAction(webviewListener);

    BehaviourTree::NotDecorator* serviceCheckNotDecorator = new BehaviourTree::NotDecorator;
    serviceCheckNotDecorator->child = BA_CheckLoginServiceType::Create(eNKLoginService::None);
    sequence4->AddAction(serviceCheckNotDecorator);

    BehaviourTree::NotDecorator* actionCheckNotDecorator = new BehaviourTree::NotDecorator;
    actionCheckNotDecorator->child = BA_CheckServiceActionType::Create(eNKServiceAction::None);
    sequence4->AddAction(actionCheckNotDecorator);

    BehaviourTree::Selector* selector2 = BehaviourTree::Selector::Create(BehaviourTree::LoopCondition::None);

    BehaviourTree::Sequence* sequence5 = BehaviourTree::Sequence::Create(BehaviourTree::LoopCondition::None);

    BehaviourTree::Selector* selector3 = BehaviourTree::Selector::Create(BehaviourTree::LoopCondition::None);

    BA_IsServiceAuthenticated* isServiceAuthenticated = BA_IsServiceAuthenticated::Create(eNKLoginService::InBlackboard);
    selector3->AddAction(isServiceAuthenticated);

    BA_AuthenticateService* authenticateService = BA_AuthenticateService::Create(
        eNKLoginService::InBlackboard, eNKServiceAuthMethod::Email);
    selector3->AddAction(authenticateService);
    sequence5->AddAction(selector3);

    BehaviourTree::Selector* selector4 = BehaviourTree::Selector::Create(BehaviourTree::LoopCondition::None);

    BehaviourTree::Sequence* sequence6 = BehaviourTree::Sequence::Create(BehaviourTree::LoopCondition::None);

    BA_ServiceAction* blackboardAction = BA_ServiceAction::Create(
        eNKLoginService::InBlackboard, eNKServiceAction::InBlackboard);
    sequence6->AddAction(blackboardAction);
    selector4->AddAction(sequence6);

    BehaviourTree::Sequence* sequence7 = BehaviourTree::Sequence::Create(BehaviourTree::LoopCondition::None);

    BA_CheckServiceActionType* loginActionCheck = BA_CheckServiceActionType::Create(eNKServiceAction::Login);
    sequence7->AddAction(loginActionCheck);

    BA_CheckErrorType* checkCannotFindLink = BA_CheckErrorType::Create(CANNOT_FIND_LINK);
    sequence7->AddAction(checkCannotFindLink);

    BA_ServiceAction* createAction = BA_ServiceAction::Create(eNKLoginService::InBlackboard, eNKServiceAction::Create);
    sequence7->AddAction(createAction);

    BA_ServiceAction* loginAction = BA_ServiceAction::Create(eNKLoginService::InBlackboard, eNKServiceAction::Login);
    sequence7->AddAction(loginAction);
    selector4->AddAction(sequence7);
    sequence5->AddAction(selector4);
    selector2->AddAction(sequence5);

    BehaviourTree::ForceState* errorToolTipFailure = new BehaviourTree::ForceState;
    errorToolTipFailure->child = BA_WebViewErrorToolTip::Create();
    errorToolTipFailure->forceState = BehaviourTree::AState::Failure;
    selector2->AddAction(errorToolTipFailure);
    sequence4->AddAction(selector2);

    BehaviourTree::Loop* loop = new BehaviourTree::Loop;
    loop->child = sequence4;
    loop->loopCondition = BehaviourTree::LoopCondition::LoopOnFailure;

    BehaviourTree::LoopOnCondition* loopOnCondition = BehaviourTree::LoopOnCondition::Create(
        boost::bind(&NKSessionImpl::CheckBBForLinkAction, this, boost::placeholders::_1), loop);
    parallel->AddAction(loopOnCondition);
    sequence3->AddAction(parallel);

    BA_HideWebView* hideWebView = BA_HideWebView::Create();
    sequence3->AddAction(hideWebView);

    BehaviourTree::ForceState* cacheSessionSuccess = new BehaviourTree::ForceState;
    cacheSessionSuccess->child = BA_CacheSession::Create(STUFF_TO_REFACTOR::s_FileIO);
    cacheSessionSuccess->forceState = BehaviourTree::AState::Success;
    sequence3->AddAction(cacheSessionSuccess);
    selector->AddAction(sequence3);

    BA_HideWebView* hideWebView2 = BA_HideWebView::Create();
    selector->AddAction(hideWebView2);
    sequence2->AddAction(selector);

    BehaviourTree::ForceState* sequence2Success = new BehaviourTree::ForceState;
    sequence2Success->child = sequence2;
    sequence2Success->forceState = BehaviourTree::AState::Success;
    sequence->AddAction(sequence2Success);

    BA_ReportErrorCallback* reportErrorCb = BA_ReportErrorCallback::Create(
        boost::bind(&NKSessionImpl::ReportError, this, boost::placeholders::_1));
    sequence->AddAction(reportErrorCb);

    BA_StoreStateCallback* storeStateCb = BA_StoreStateCallback::Create(
        boost::bind(&NKSessionImpl::StoreState, this, boost::placeholders::_1));
    sequence->AddAction(storeStateCb);

    auto showUIRequest = boost::make_shared<NKSessionImpl::SSessionShowUIRequest>();
    showUIRequest->action = boost::shared_ptr<BehaviourTree::Composite>(sequence);

    tasks.push_back(showUIRequest);
}

bool NKSessionImpl::CheckBBForLinkAction(BehaviourTree::IBlackboard* blackboard) const {
    if (blackboard) {
        NKSessionBlackboard* sessionBlackboard = dynamic_cast<NKSessionBlackboard*>(blackboard);
        if (sessionBlackboard) {
            return sessionBlackboard->serviceAction == eNKServiceAction::Link;
        }
    }

    return false;
}

void NKSessionImpl::Update(const float& elapsed) {
    while (!tasks.empty()) {
        boost::shared_ptr<SSessionTask> task = tasks.front();
        if (task->action->state == BehaviourTree::AState::Running) {
            task->action->Update(task->blackboard.get(), elapsed);

            if (task->action->state == BehaviourTree::AState::Running) {
                return;
            }

            if (session->LoggingEnabled()) {
                task->blackboard->OutputFullLog();
            }

            task->action->Stop(task->blackboard.get());
        } else if (!boost::dynamic_pointer_cast<SSessionOpenRequest>(task)) {
            std::vector<std::string> providers;

            for (auto it = loginServiceMap.begin(); it != loginServiceMap.end(); ++it) {
                I_NKLoginService* loginService = it->second;
                std::string providerString = loginService->GetProviderString();
                std::string userIdentifier = loginService->GetUserIdentifier();

                if (!providerString.empty() && !userIdentifier.empty()) {
                    providers.push_back(providerString + "_" + userIdentifier);
                }
            }

            task->blackboard = boost::make_shared<NKSessionBlackboard>(
                this, accessToken, responseUser, appID, skuID, privateKey, serverCluster, providers);

            task->action->Start(task->blackboard.get());

            if (task->action->state == BehaviourTree::AState::Running) {
                task->action->Update(task->blackboard.get(), elapsed);

                if (task->action->state == BehaviourTree::AState::Running) {
                    return;
                }
            }

            if (session->LoggingEnabled()) {
                task->blackboard->OutputFullLog();
            }

            task->action->Stop(task->blackboard.get());
        } else if (state != NKSessionState::VALUE1) {
            state = NKSessionState::VALUE0;

            std::vector<std::string> providers;

            for (auto it = loginServiceMap.begin(); it != loginServiceMap.end(); ++it) {
                I_NKLoginService* loginService = it->second;
                std::string providerString = loginService->GetProviderString();
                std::string userIdentifier = loginService->GetUserIdentifier();

                if (!providerString.empty() && !userIdentifier.empty()) {
                    providers.push_back(providerString + "_" + userIdentifier);
                }
            }

            task->blackboard = boost::make_shared<NKSessionBlackboard>(
                this, accessToken, responseUser, appID, skuID, privateKey, serverCluster, providers);

            task->action->Start(task->blackboard.get());

            if (task->action->state == BehaviourTree::AState::Running) {
                task->action->Update(task->blackboard.get(), elapsed);

                if (task->action->state == BehaviourTree::AState::Running) {
                    return;
                }
            }

            if (session->LoggingEnabled()) {
                task->blackboard->OutputFullLog();
            }

            task->action->Stop(task->blackboard.get());
        }

        tasks.pop_front();
    }
}

void NKSessionImpl::Close(bool hasCache) {
    if (!appInfoValid) {
        return;
    }

    tasks.clear();

    if (hasCache && STUFF_TO_REFACTOR::s_FileIO) {
        STUFF_TO_REFACTOR::s_FileIO->FileDelete(
            NKEndpoints::GetSessionCacheFilePath(serverCluster),
            STUFF_TO_REFACTOR::s_FileIO->cachePolicy);
    }

    state = NKSessionState::VALUE6;
    accessToken = NKAccessToken();
    responseUser = NKResponseUser();

    if (!loginCallback.empty()) {
        loginCallback(session, state, NKError());
    }
}

void NKSessionImpl::SetAppInterface(I_NKSDKApp* appInterface) {
    this->appInterface = appInterface;
}

void NKSessionImpl::AddServiceInterface(const eNKLoginService& type, I_NKLoginService* service) {
    loginServiceMap.emplace(type, service);
}

I_NKLoginService* NKSessionImpl::GetLoginService(const eNKLoginService& type) const {
    auto it = loginServiceMap.find(type);
    return it != loginServiceMap.end() ? it->second : nullptr;
}

void NKSessionImpl::GetScreenFocalPoint(uint16_t& x, uint16_t& y) const {
    x = focalX * appInterface->GetViewScale();
    y = focalY * appInterface->GetViewScale();
}

std::string NKSessionImpl::GetSupportedInterfaceTags() const {
    std::string result;

    for (auto it = loginServiceMap.begin(); it != loginServiceMap.end(); ++it) {
        if (it->second->IsServiceSupported()) {
            if (!result.empty()) {
                result += ",";
            }

            result += it->second->GetProviderString();
        }
    }

    return result;
}

std::vector<std::string> NKSessionImpl::GetLoggedInInterfaceIDs() const {
    std::vector<std::string> result;

    for (auto it = loginServiceMap.begin(); it != loginServiceMap.end(); ++it) {
        if (it->second->IsUserAuthenticated()) {
            result.push_back(it->second->GetProviderString() + "_" + it->second->GetUserIdentifier());
        }
    }

    return result;
}

std::vector<std::string> NKSessionImpl::GetLoggedInInterfaceProviders() const {
    std::vector<std::string> result;

    for (auto it = loginServiceMap.begin(); it != loginServiceMap.end(); ++it) {
        if (it->second->IsUserAuthenticated() && !it->second->GetUserIdentifier().empty()) {
            result.push_back(it->second->GetProviderString());
        }
    }

    return result;
}

bool NKSessionImpl::IsAccountUsingService(eNKLoginService type) const {
    return loginServiceMap.find(type) != loginServiceMap.end();
}

void NKSessionImpl::ReportError(const NKError& error) {
    lastError = error;

    if (error.IsValid()) {
        LOG_ERROR("NKSessionImpl Last error:\n\tError Type: %s\n\tReason:%s\n\tFix: %s",
                  error.typeStr.c_str(), error.reason.c_str(), error.fix.c_str()); ENFORCE_LINE(694);
    }
}
