#pragma once

#include "NewFramework/Events/Observer.h"
#include "NewFramework/Utilities/JSON/JSON_PropertyReader/JSON_Read.h"

class C_AdManager;
class CApp;
struct IBasePointers;
struct IHttpRequestManager;

enum eAdProviders { // speculative name
    ADPROVIDER_NULL = 0x0,
    ADPROVIDER_IRONSOURCE = 0x10,
    ADPROVIDER_TAPJOY = 0x20
};

enum eAdState {
    ADSTATE_NULL,
    ADSTATE_INIT,
    ADSTATE_FETCHING,
    ADSTATE_NOFILL,
    ADSTATE_ADREADY,
    ADSTATE_PLAYING
};

enum eAdWatchType {
    WATCHTYPE_NONREWARDED,
    WATCHTYPE_REWARDED,
    WATCHTYPE_OFFERWALL
};

struct sAdPrivacyData {
    int64_t field_0 = 1; // 0x00
    int16_t field_8 = 1; // 0x08
};

struct sAdProviderData {
    std::string sName; // 0x00
#if USING_IRONSOURCE || USING_TAPJOY
    std::string sAppID; // 0x18
    std::string field_30; // 0x30
#endif
};

struct sCurrencyData {
    std::string sCurrency; // 0x00
    std::string field_18; // 0x18
    eAdWatchType eWatchType; // 0x30
};

struct SServiceConsent {
    std::string field_0; // 0x00
    bool field_18; // 0x18
    std::string field_20; // 0x20
    std::string field_38; // 0x38
};

class CAdEndedEvent : public IEvent {
public:
    bool bCompleted; // 0x08
    eAdWatchType eWatchType; // 0x0C
};

class CAdRewardEvent : public IEvent {
public:
    std::string sCurrency;
    std::string sReward;
    uint field_38{};
    uint watchedTime{};

    explicit CAdRewardEvent(CAdRewardEvent* other);
    CAdRewardEvent(const std::string& sCurrency, std::string sReward, uint a4);
};

class CConsentRequestComplete : public IEvent {};

class IAdListener {
public:
    explicit IAdListener(C_AdManager* pManager);
    virtual ~IAdListener();

    virtual void AdStarted() {}
    virtual void AdEnded(bool, eAdWatchType) {}
    virtual void AdsAvailable() {}
    virtual void AdsUnavailable() {}
    virtual void AdsFailedToShow() {}
    virtual void AdsRewardComplete(const std::string&, uint) {}

    void SetAdManager(C_AdManager* pManager);
protected:
    C_AdManager* m_pManager;
};

class C_AdInterface {
    friend class C_AdManager;
public:
    using tRewardTypeKey = std::string;

    C_AdInterface(C_AdManager* pManager, std::string sName, const sCurrencyData& currencyData);
    virtual ~C_AdInterface() = default;

    void CompletedInit();
    void SetCurrentState(eAdState eState);
    void AdStarted();
    void AdEnded(bool a2);
    void FetchingAds();
    void AdsAvailableCallback();
    void AdsUnavailableCallback();
    void AdsFailedToShowCallback();
    void AdRewardsCompleteCallback(uint a2);
    bool ShowAds(const tRewardTypeKey& sReward);
    bool FetchAds(const tRewardTypeKey& sReward, bool bShortRuntime);
    void Process(float fElapsed);
    void SetPlacementNameMapping(const std::map<std::string, std::string>& placementNameMapping);

    virtual bool HasBeenInitialised();
    virtual void PrintConsent() {}
    virtual void AppGoingToBackground() {}
    virtual void AppReturnFromBackground() {}
    virtual void ShowTestSuite() {}
protected:
    std::string m_sName; // 0x08
    bool m_bInitialised{}; // 0x20
    eAdWatchType m_eWatchType; // 0x24
    std::string m_sCurrency; // 0x28
    float m_fRuntime{}; // 0x40
    C_AdManager* m_pManager; // 0x48
    std::string m_sReward; // 0x50
    std::string m_sDefaultPlacementName; // 0x68
    std::map<std::string, std::string> m_placementNameMapping; // 0x80
    eAdState m_eState = ADSTATE_NULL; // 0x98
    uint m_availabilityCallbackCount{}; // 0x9C
    uint m_availableCallbackCount{}; // 0xA0
    uint m_endedCallbackCount{}; // 0xA4
    uint field_A8{}; // 0xA8
    float field_AC{}; // 0xAC

    virtual void _FetchAds(const tRewardTypeKey& sReward) {}
    virtual bool ShowNonRewardedAd() = 0;
    virtual bool ShowRewardedAd() = 0;
    virtual bool _ShowOfferwall() { return false; }
    virtual void _CheckForRewards() {}
    virtual void _Process(float fElapsed) {}
};

class C_AdManager : public CObserver<CConsentRequestComplete> {
    friend class C_AdInterface;
    friend class C_IronSourceInterface;
    friend class C_TapjoyInterface;
public:
    C_AdManager(CApp* pApp, CEventManager* pEventManager);
    ~C_AdManager() override;

    void AdStarted();
    void AdEnded(bool a2, eAdWatchType eWatchType);
    void AdsAvailableCallback();
    void AdsUnavailableCallback(C_AdInterface* pInterface);
    void AdsFailedToShowCallback();
    void AdRewardsCompleteCallback(const std::string& a2, std::string a3, uint a4);
    void RemoveListener(IAdListener* pListener);
    void AddListener(IAdListener* pListener);
    void Process(float fElapsed);
    bool FetchAds(const std::string& sCurrency, const std::string& a3);
    bool AreAdsAvailable(const std::string& sCurrency);
    eAdState GetAdState(const std::string& sCurrency);
    bool ShowAds(const std::string& sCurrency, const std::string& a3);
    void AppGoingToBackground();
    void AppReturnFromBackground();
    void CheckForRewards();
    void AdEndedWithoutNotification();
    void PopulateCurrentFillStats(const std::string&, std::map<std::string, std::string>&) {}
    void SetupManager(std::string a2, IBasePointers* pBasePointers, int eProviders);
    void HandleEvent(CConsentRequestComplete* pEvent) override;
    void AddAdZone(const sCurrencyData& currencyData);
    void AddAdSystem(const sCurrencyData& currencyData, C_AdInterface* pInterface);
    C_AdInterface* AddAdSystem(const sCurrencyData& currencyData, sAdProviderData& providerData);
    void AdsInitialised(std::string) {}
    void AddAdSystemFromJSON(JSON_PropertyReader& reader, const sCurrencyData& currencyData, bool bEnforceCoppa);
    void ParseAdServerJSON(json_spirit::mObject& obj, std::string a3, sAdPrivacyData privacyData);
    bool IsOfferwallSupported();
    void PrintConsentInfos();
    void ShowAdTrackingRequest() {}
    bool AdTrackingAllowed();
    bool AdTrackingRequestNeeded();
    void InitConsentProvider(const std::string&, bool) {}
    char* ShowConsentRequest(bool a2);
    std::string GetConsentData();
    bool ConsentRequestNeeded();
private:
    std::string m_sInterfaceName; // 0x10
    std::vector<IAdListener*> m_vecListeners; // 0x28
    std::map<std::string, std::vector<C_AdInterface*>> m_mapInterfaces; // 0x40
    std::string field_58; // 0x58
    std::vector<sCurrencyData> m_vecCurrencyData; // 0x70
    CApp* m_pApp; // 0x80
    std::deque<std::string> m_dequeCurrencies; // 0x90
    float m_fAdFetchTime = 2.0; // 0xC0
    std::string field_C8; // 0xC8
    IHttpRequestManager* m_pHttpRequestManager{}; // 0xE0
    CEventManager* m_pEventManager; // 0xE8
    eAdProviders m_eProviders = ADPROVIDER_NULL; // 0xF0
    bool m_bFetchOnLoad{}; // 0xF4
    bool m_bAdTrackingAllowed{}; // 0xF5
    char* field_F8{}; // 0xF8
    sAdPrivacyData m_privacyData; // 0x100
    std::string m_sAdQuality; // 0x110
};
