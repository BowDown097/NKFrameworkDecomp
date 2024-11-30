#pragma once
#include "NewFramework/Networking/NKAPI/NKEndpoints.h"
#include "NewFramework/Networking/NKAPI/NKError.h"
#include "NKMessages.h"
#include "Uncategorized/BehaviourTree.h"
#include "Uncategorized/NKLoginService.h"
#include <deque>

class CEventManager;
class I_NKSDKApp;
class NKSession;
class NKSessionBlackboard;

enum class NKSessionState { VALUE0, VALUE1, VALUE2, VALUE3, VALUE4, VALUE5, VALUE6, VALUE7 };

using LoginCallback = boost::function3<void, NKSession*, NKSessionState, const NKError&>;

class NKSessionImpl {
    friend class NKSession;
public:
    struct SSessionTask {
        boost::shared_ptr<NKSessionBlackboard> blackboard; // 0x08
        boost::shared_ptr<BehaviourTree::Action> action; // 0x18
        virtual ~SSessionTask() = default;
    };

    struct SSessionLinkRequest : SSessionTask {};
    struct SSessionOpenRequest : SSessionTask {};
    struct SSessionShowUIRequest : SSessionTask {};

    NKSessionImpl(NKSession* session) : session(session) {}
    void Initialise(const int& appID, const int& skuID, const std::string& privateKey,
                    CEventManager* eventManager, eNKLinkServerCluster serverCluster,
                    LoginCallback loginCallback);
    void Open(bool needSession, bool, bool);
    int GetWebViewWidth() const;
    int GetWebViewHeight() const;
    void ReportError(const NKError& error);
    void StoreState(const BehaviourTree::IBlackboard* blackboard);
    void LoginUsingService(const eNKLoginService& loginServiceType, bool authWithEmail, bool, bool);
    void LinkService(const eNKLoginService& loginServiceType, bool authWithEmail);
    void ShowUI();
    bool CheckBBForLinkAction(BehaviourTree::IBlackboard* blackboard) const;
    void Update(const float& elapsed);
    void Close(bool hasCache);
    void SetAppInterface(I_NKSDKApp* appInterface);
    void AddServiceInterface(const eNKLoginService& type, I_NKLoginService* service);
    I_NKLoginService* GetLoginService(const eNKLoginService& type) const;
    void GetScreenFocalPoint(uint16_t& x, uint16_t& y) const;
    std::string GetSupportedInterfaceTags() const;
    std::vector<std::string> GetLoggedInInterfaceIDs() const;
    std::vector<std::string> GetLoggedInInterfaceProviders() const;
    bool IsAccountUsingService(eNKLoginService type) const;
private:
    I_NKSDKApp* appInterface{}; // 0x00
    NKSession* session; // 0x08
    bool appInfoValid{}; // 0x10
    uint16_t focalX{}; // 0x12
    uint16_t focalY{}; // 0x14
    int appID = -1; // 0x18
    int skuID = -1; // 0x1C
    std::string privateKey; // 0x20
    eNKLinkServerCluster serverCluster = eNKLinkServerCluster::NONE; // 0x38
    boost::function3<void, NKSession*, NKSessionState, const NKError&> loginCallback; // 0x40
    std::map<eNKLoginService, I_NKLoginService*> loginServiceMap; // 0x60
    NKSessionState state = NKSessionState::VALUE6; // 0x78
    NKAccessToken accessToken; // 0x80
    NKResponseUser responseUser{}; // 0xA0
    NKError lastError; // 0x160
    std::deque<boost::shared_ptr<SSessionTask>> tasks; // 0x1B8
    CEventManager* eventManager{}; // 0x1E8
};
