#include "NKSession.h"
#include "NewFramework/Console/ConsoleVariable.h"
#include "NewFramework/Networking/NKAPI/NKManager.h"

namespace {
    ConsoleVariable g_allow_graphing_cvar("link.session.logging", "Flag if we should allow LiNK session logging", "0");
}

NKSession* NKSession::GetActiveSession() {
    if (!s_ActiveSession)
        s_ActiveSession = NKManager::GetManager()->GetSessionModule();
    return s_ActiveSession;
}

NKSession::NKSession() : impl(this) {
    atexit(NKSession::CleanUp);
}

void NKSession::CleanUp() {
    if (s_ActiveSession) {
        delete s_ActiveSession;
        s_ActiveSession = nullptr;
    }
}

const NKAccessToken& NKSession::GetAccessToken() const {
    return impl.accessToken;
}

const NKResponseUser& NKSession::GetUserDetails() const {
    return impl.responseUser;
}

void NKSession::EnableLogging(bool enabled) const {
    cvar::set_integer(&g_allow_graphing_cvar, enabled);
}

bool NKSession::LoggingEnabled() const {
    return g_allow_graphing_cvar.intValue != 0;
}

int NKSession::GetAppID() const {
    return impl.appID;
}

int NKSession::GetSkuID() const {
    return impl.skuID;
}

NKSessionState NKSession::GetState() const {
    return impl.state;
}

eNKLinkServerCluster NKSession::GetServerCluster() const {
    return impl.serverCluster;
}

void NKSession::Close(bool hasCache) {
    impl.Close(hasCache);
}

void NKSession::Initialise(const int& appID, const int& skuID, const std::string& privateKey,
                           CEventManager* eventManager, eNKLinkServerCluster serverCluster,
                           LoginCallback loginCallback) {
    impl.Initialise(appID, skuID, privateKey, eventManager, serverCluster, loginCallback);
}

void NKSession::Update(const float& elapsed) {
    impl.Update(elapsed);
}

void NKSession::SetAppInterface(I_NKSDKApp* appInterface) {
    impl.SetAppInterface(appInterface);
}

void NKSession::AddServiceInterface(const eNKLoginService& type, I_NKLoginService* service) {
    impl.AddServiceInterface(type, service);
}

void NKSession::Open(bool needSession, bool a3, bool a4) {
    impl.Open(needSession, a3, a4);
}

void NKSession::LoginUsingService(const eNKLoginService& loginServiceType, bool authWithEmail, bool a4, bool a5) {
    impl.LoginUsingService(loginServiceType, authWithEmail, a4, a5);
}

void NKSession::LinkService(const eNKLoginService& loginServiceType, bool authWithEmail) {
    impl.LinkService(loginServiceType, authWithEmail);
}

void NKSession::ShowUI() {
    impl.ShowUI();
}

const std::string& NKSession::GetPrivateKey() const {
    return impl.privateKey;
}

void NKSession::SetScreenFocalPoint(uint16_t x, uint16_t y) {
    impl.focalX = x;
    impl.focalY = y;
}

void NKSession::GetScreenFocalPoint(uint16_t& x, uint16_t& y) const {
    impl.GetScreenFocalPoint(x, y);
}

std::string NKSession::GetSupportedInterfaceTags() const {
    return impl.GetSupportedInterfaceTags();
}

std::vector<std::string> NKSession::GetLoggedInInterfaceIDs() const {
    return impl.GetLoggedInInterfaceIDs();
}

std::vector<std::string> NKSession::GetLoggedInInterfaceProviders() const {
    return impl.GetLoggedInInterfaceProviders();
}

bool NKSession::IsAccountUsingService(eNKLoginService type) const {
    return impl.IsAccountUsingService(type);
}

const NKError& NKSession::GetLastError() const {
    return impl.lastError;
}
