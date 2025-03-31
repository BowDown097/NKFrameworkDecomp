#include "AdInterface.h"

#include "NewFramework/Analytics/DGAnalytics.h"
#include "NewFramework/BaseApplication/AppClass/AppClass.h"
#include "NewFramework/Platform/Shared/Logging.h"
#include "NewFramework/Utilities/StringHelper.h"

#if USING_IRONSOURCE
# if __ANDROID__
# include "Android/IronSource/IronSource.h"
# elif __APPLE__
# include "iOS/IronSource/IronSource.h"
# endif
#endif

#if USING_TAPJOY
# if __ANDROID__
# include "Android/Tapjoy/Tapjoy.h"
# elif __APPLE__
# include "iOS/Tapjoy/Tapjoy.h"
# endif
#endif

std::string GetReadableState(eAdState eState) {
    switch (eState) {
    case ADSTATE_INIT:
        return "INIT";
    case ADSTATE_FETCHING:
        return "FETCH";
    case ADSTATE_NOFILL:
        return "NOFILL";
    case ADSTATE_ADREADY:
        return "ADREADY";
    case ADSTATE_PLAYING:
        return "PLAYING";
    default:
        return "UNKNOWN";
    }
}

namespace CMPInterface {
SServiceConsent GetServiceConsent(char* a2, const std::string& a3) {
    auto it = reinterpret_cast<std::map<std::string,SServiceConsent>*>(a2 + 40)->find(a3);
    return it != reinterpret_cast<std::map<std::string,SServiceConsent>*>(a2 + 40)->end() ? it->second : SServiceConsent{};
}
}

CAdRewardEvent::CAdRewardEvent(CAdRewardEvent* other)
    : sCurrency(other->sCurrency),
      field_38(other->field_38),
      watchedTime(other->watchedTime) {}

CAdRewardEvent::CAdRewardEvent(const std::string& sCurrency, std::string sReward, uint a4)
    : sCurrency(sCurrency),
      sReward(sReward),
      field_38(a4) {}

IAdListener::IAdListener(C_AdManager* pManager) {
    SetAdManager(pManager);
}

IAdListener::~IAdListener() {
    if (m_pManager) {
        m_pManager->RemoveListener(this);
    }
}

void IAdListener::SetAdManager(C_AdManager* pManager) {
    if (pManager) {
        m_pManager = pManager;
        pManager->AddListener(this);
    }
}

C_AdInterface::C_AdInterface(C_AdManager* pManager, std::string sName, const sCurrencyData& currencyData)
    : m_sName(sName),
      m_eWatchType(currencyData.eWatchType),
      m_sCurrency(currencyData.sCurrency),
      m_pManager(pManager) {}

void C_AdInterface::CompletedInit() {
    m_bInitialised = true;
    SetCurrentState(ADSTATE_INIT);
}

void C_AdInterface::SetCurrentState(eAdState eState) {
    std::string v2 = GetReadableState(m_eState);
    std::string v3 = GetReadableState(eState);
    // v2 and v3 unused...?
    m_eState = eState;
}

void C_AdInterface::AdStarted() {
    SetCurrentState(ADSTATE_PLAYING);
    if (m_pManager) {
        m_pManager->AdStarted();
    }
}

void C_AdInterface::AdEnded(bool a2) {
    ++m_endedCallbackCount;
    if (m_eState == ADSTATE_PLAYING) {
        SetCurrentState(ADSTATE_INIT);
    }
    if (m_pManager) {
        m_pManager->AdEnded(a2, m_eWatchType);
    }
}

void C_AdInterface::FetchingAds() {
    if (m_eState != ADSTATE_ADREADY) {
        SetCurrentState(ADSTATE_FETCHING);
    }
}

void C_AdInterface::AdsAvailableCallback() {
    ++m_availableCallbackCount;
    ++m_availabilityCallbackCount;
    SetCurrentState(ADSTATE_ADREADY);
    if (m_pManager) {
        m_pManager->AdsAvailableCallback();
    }
}

void C_AdInterface::AdsUnavailableCallback() {
    ++m_availabilityCallbackCount;
    SetCurrentState(ADSTATE_NOFILL);
    if (m_pManager) {
        m_pManager->AdsUnavailableCallback(this);
    }
}

void C_AdInterface::AdsFailedToShowCallback() {
    if (m_pManager) {
        m_pManager->AdsFailedToShowCallback();
    }
}

void C_AdInterface::AdRewardsCompleteCallback(uint a2) {
    field_A8 += a2;
    if (m_pManager) {
        m_pManager->AdRewardsCompleteCallback(m_sCurrency, m_sReward, a2);
    }
}

bool C_AdInterface::ShowAds(const tRewardTypeKey& sReward) {
    switch (m_eWatchType) {
    case WATCHTYPE_NONREWARDED:
        m_sReward = "";
        return ShowNonRewardedAd();
    case WATCHTYPE_REWARDED:
        m_sReward = sReward;
        return ShowRewardedAd();
    case WATCHTYPE_OFFERWALL:
        m_sReward = sReward;
        return _ShowOfferwall();
    default:
        ENFORCE_LINE(158); return NKAssert(false, "Unsupported Ad Format");
    }
}

bool C_AdInterface::FetchAds(const tRewardTypeKey& sReward, bool bShortRuntime) {
    if (m_eState != ADSTATE_INIT && m_eState != ADSTATE_NOFILL && m_fRuntime > 0.0) {
        return false;
    }

    _FetchAds(sReward);
    m_fRuntime = bShortRuntime ? 20.0 : 40.0;
    return true;
}

void C_AdInterface::Process(float fElapsed) {
    m_fRuntime -= fElapsed;
    _Process(fElapsed);
}

void C_AdInterface::SetPlacementNameMapping(const std::map<std::string, std::string>& placementNameMapping) {
    m_placementNameMapping = placementNameMapping;
}

bool C_AdInterface::HasBeenInitialised() {
    return m_bInitialised;
}

C_AdManager::C_AdManager(CApp* pApp, CEventManager* pEventManager)
    : m_pApp(pApp),
      m_pEventManager(pEventManager) {}

C_AdManager::~C_AdManager() {
    Unsubscribe(m_pEventManager);
}

void C_AdManager::AdStarted() {
    for (IAdListener* pListener : m_vecListeners) {
        pListener->AdStarted();
    }
}

void C_AdManager::AdEnded(bool a2, eAdWatchType eWatchType) {
    for (IAdListener* pListener : m_vecListeners) {
        pListener->AdEnded(a2, eWatchType);
    }

    CAdEndedEvent* pAdEndedEvent = new CAdEndedEvent;
    pAdEndedEvent->bCompleted = a2;
    pAdEndedEvent->eWatchType = eWatchType;

    m_pEventManager->QueueEvent(pAdEndedEvent);
}

void C_AdManager::AdsAvailableCallback() {
    for (IAdListener* pListener : m_vecListeners) {
        pListener->AdsAvailable();
    }
}

void C_AdManager::AdsUnavailableCallback(C_AdInterface* pInterface) {
    if (pInterface) {
        auto it = m_mapInterfaces.find(pInterface->m_sCurrency);
        for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
            if (*it2 == pInterface) {
                it->second.erase(it2);
            } else {
                ++it2;
            }
        }
    }

    for (IAdListener* pListener : m_vecListeners) {
        pListener->AdsUnavailable();
    }
}

void C_AdManager::AdsFailedToShowCallback() {
    for (IAdListener* pListener : m_vecListeners) {
        pListener->AdsFailedToShow();
    }
}

void C_AdManager::AdRewardsCompleteCallback(const std::string& a2, std::string a3, uint a4) {
    for (IAdListener* pListener : m_vecListeners) {
        pListener->AdsRewardComplete(a2, a4);
    }

    CAdRewardEvent* pAdRewardEvent = new CAdRewardEvent(a2, a3, a4);
    m_pEventManager->QueueEvent(pAdRewardEvent);
}

void C_AdManager::RemoveListener(IAdListener* pListener) {
    for (auto it = m_vecListeners.begin(); it != m_vecListeners.end();) {
        if (*it == pListener) {
            m_vecListeners.erase(it);
        } else {
            ++it;
        }
    }
}

void C_AdManager::AddListener(IAdListener* pListener) {
    m_vecListeners.push_back(pListener);
}

void C_AdManager::Process(float fElapsed) {
    for (auto it = m_mapInterfaces.begin(); it != m_mapInterfaces.end(); ++it) {
        for (C_AdInterface* pInterface : it->second) {
            pInterface->m_fRuntime -= fElapsed;
            pInterface->_Process(fElapsed);
        }
    }

    if (!m_dequeCurrencies.empty()) {
        m_fAdFetchTime -= fElapsed;
        if (m_fAdFetchTime <= 0.0) {
            std::string sCurrency = m_dequeCurrencies.front();
            m_dequeCurrencies.pop_front();
            if (!FetchAds(sCurrency, "")) {
                m_dequeCurrencies.push_back(sCurrency);
            }
            m_fAdFetchTime = 2.0;
        }
    }
}

bool C_AdManager::FetchAds(const std::string& sCurrency, const std::string& a3) {
    auto it = m_mapInterfaces.find(sCurrency);
    if (it == m_mapInterfaces.end()) {
        return false;
    }

    std::vector<C_AdInterface*> interfaces = it->second;
    if (interfaces.empty()) {
        return false;
    }

    for (C_AdInterface* pInterface : interfaces) {
        if (pInterface->m_eState == ADSTATE_ADREADY) {
            return true;
        } else if (pInterface->m_eState == ADSTATE_PLAYING) {
            return false;
        } else if (pInterface->m_eState == ADSTATE_INIT || pInterface->m_eState == ADSTATE_NOFILL) {
            CApp::eNetworkType eNetworkType = m_pApp->GetNetworkType();
            if (pInterface->m_fRuntime <= 0.0) {
                pInterface->_FetchAds(a3);
                pInterface->m_fRuntime = eNetworkType == CApp::eNetworkType::Unk1 ? 20.0 : 40.0;
                return true;
            }
        }
    }

    return false;
}

bool C_AdManager::AreAdsAvailable(const std::string& sCurrency) {
    return GetAdState(sCurrency) == ADSTATE_ADREADY;
}

eAdState C_AdManager::GetAdState(const std::string& sCurrency) {
    auto it = m_mapInterfaces.find(sCurrency);
    if (it == m_mapInterfaces.end() || it->second.empty()) {
        return ADSTATE_NULL;
    }

    eAdState eResult = ADSTATE_NULL;
    for (C_AdInterface* pInterface : it->second) {
        if (pInterface->m_eState > eResult) {
            eResult = pInterface->m_eState;
        }
    }

    return eResult;
}

bool C_AdManager::ShowAds(const std::string& sCurrency, const std::string& a3) {
    auto it = m_mapInterfaces.find(sCurrency);
    if (it == m_mapInterfaces.end()) {
        return false;
    }

    for (C_AdInterface* pInterface : it->second) {
        if (pInterface->m_eState == ADSTATE_ADREADY) {
            m_sInterfaceName = pInterface->m_sName;
            pInterface->ShowAds(a3);
            return true;
        }
    }

    return false;
}

void C_AdManager::AppGoingToBackground() {
    for (auto it = m_mapInterfaces.begin(); it != m_mapInterfaces.end(); ++it) {
        for (C_AdInterface* pInterface : it->second) {
            pInterface->AppGoingToBackground();
        }
    }
}

void C_AdManager::AppReturnFromBackground() {
    for (auto it = m_mapInterfaces.begin(); it != m_mapInterfaces.end(); ++it) {
        for (C_AdInterface* pInterface : it->second) {
            pInterface->AppReturnFromBackground();
        }
    }
}

void C_AdManager::CheckForRewards() {
    for (auto it = m_mapInterfaces.begin(); it != m_mapInterfaces.end(); ++it) {
        for (C_AdInterface* pInterface : it->second) {
            pInterface->_CheckForRewards();
        }
    }
}

void C_AdManager::AdEndedWithoutNotification() {
    for (auto it = m_mapInterfaces.begin(); it != m_mapInterfaces.end(); ++it) {
        for (C_AdInterface* pInterface : it->second) {
            if (pInterface->m_eState == ADSTATE_PLAYING) {
                pInterface->AdEnded(false);
            }
        }
    }
}

void C_AdManager::PopulateCurrentFillStats(const std::string&, std::map<std::string, std::string>&) {

}

void C_AdManager::SetupManager(std::string a2, IBasePointers* pBasePointers, int eProviders) {
    field_C8 = a2;
    m_pHttpRequestManager = pBasePointers->pHttpRequestManager;
    m_pEventManager = pBasePointers->pEventManager;
    m_eProviders = static_cast<eAdProviders>(eProviders);
    Subscribe(m_pEventManager);
}

void C_AdManager::HandleEvent(CConsentRequestComplete* pEvent) {
    if (DGAnalytics::Instance()->Get("bbfcb08f5bb52e06ab3e3f62579c9ad48379cfea")) {
        SServiceConsent serviceConsent = CMPInterface::GetServiceConsent(field_F8, "42vRvlulK96R-F");
    }
}

void C_AdManager::AddAdZone(const sCurrencyData& currencyData) {
    m_vecCurrencyData.push_back(currencyData);
}

void C_AdManager::AddAdSystem(const sCurrencyData& currencyData, C_AdInterface* pInterface) {
    auto it = m_mapInterfaces.emplace(currencyData.sCurrency, std::vector<C_AdInterface*>{});
    it.first->second.push_back(pInterface);
}









































































































































































































C_AdInterface* C_AdManager::AddAdSystem(const sCurrencyData& currencyData, sAdProviderData& providerData) {
    C_AdInterface* pResult{};
    if (!NKAssert(m_eProviders != ADPROVIDER_NULL, "Manager needs setup before use")) { ENFORCE_LINE(613);
        return pResult;
    }

    std::string sError;
    if (providerData.sName == "IronSource") {
        if ((m_eProviders & ADPROVIDER_IRONSOURCE) == ADPROVIDER_NULL) {
            sError = "IronSource is disabled";
        } else {
        #if USING_IRONSOURCE
            pResult = new C_IronSourceInterface(this, adProviderData, currencyData);
            m_mapInterfaces[currencyData.sCurrency].push_back(pResult);
        #endif
        }
    } else if (providerData.sName == "Tapjoy") {
        if ((m_eProviders & ADPROVIDER_TAPJOY) == ADPROVIDER_NULL) {
            sError = "Tapjoy is disabled";
        } else {
        #if USING_TAPJOY
            pResult = new C_TapjoyInterface(this, adProviderData, currencyData);
            m_mapInterfaces[currencyData.sCurrency].push_back(pResult);
        #endif
        }
    } else {
        sError = StringHelper::Format(
            "Provider '%s' is unrecognized (Currency:'%s')",
            providerData.sName.c_str(),
            currencyData.sCurrency.c_str());
    }
































    if (!sError.empty()) {
        ENFORCE_LINE(675); LOG_ERROR(
            "[ADS] ERROR: Could not add AdSystem '%s' for Currency '%s': %s",
            providerData.sName.c_str(),
            currencyData.sCurrency.c_str(),
            sError.c_str());
    }

    return pResult;
}

void C_AdManager::AdsInitialised(std::string) {

}

void C_AdManager::AddAdSystemFromJSON(JSON_PropertyReader& reader, const sCurrencyData& currencyData, bool bEnforceCoppa) {
    std::string sAdID;
    std::string sAppID;
    sAdProviderData adProviderData;

    bool bReadName = reader.ReadIfExists(adProviderData.sName, "name");
    bool bReadAppID = reader.ReadIfExists(sAppID, "appID");
    reader.ReadIfExists(sAdID, "adID");

    if (bReadAppID & bReadName) {
        bool bEnabled = true;
        reader.ReadIfExists(bEnabled, "enabled");

        bool bCoppa = false;
        reader.ReadIfExists(bCoppa, "coppa");

        if (bEnabled && bCoppa == bEnforceCoppa) {
            C_AdInterface* pInterface = AddAdSystem(currencyData, adProviderData);
            if (pInterface) {
                std::string sDefaultPlacementName;
                reader.ReadIfExists(sDefaultPlacementName, "defaultPlacementName");
                pInterface->m_sDefaultPlacementName = sDefaultPlacementName;

                std::map<std::string, std::string> placementNameMapping;
                json_spirit::mArray placementNameMappingJson;

                if (reader.ReadIfExists(placementNameMappingJson, "placementNameMapping")) {
                    for (size_t i = 0; i < placementNameMappingJson.size(); ++i) {
                        json_spirit::mObject placement;
                        if (GetValueFromArray(&placementNameMappingJson, placement, i)) {
                            JSON_PropertyReader placementReader;
                            placementReader.sourceObject = &placement;

                            std::string sLocal;
                            std::string sRemote;

                            placementReader.ReadIfExists(sLocal, "local");
                            placementReader.ReadIfExists(sRemote, "remote");
                            placementNameMapping.emplace(sLocal, sRemote);
                        }
                    }

                    pInterface->m_placementNameMapping = placementNameMapping;
                }
            }
        }
    }
}

void C_AdManager::ParseAdServerJSON(json_spirit::mObject& obj, std::string a3, sAdPrivacyData privacyData) {
    m_privacyData = privacyData;

    JSON_PropertyReader reader;
    reader.sourceObject = &obj;
    reader.ReadIfExists(m_bFetchOnLoad, "fetchOnLoad");

    for (const sCurrencyData& currencyData : m_vecCurrencyData) {
        reader.Unload();
        reader.sourceObject = &obj;

        json_spirit::mObject currencyObj;
        if (!reader.ReadIfExists(currencyObj, currencyData.sCurrency)) {
            continue;
        }

        reader.Unload();
        reader.sourceObject = &currencyObj;

        json_spirit::mArray providerList;
        if (reader.ReadIfExists(providerList, "providerList")) {
            for (size_t i = 0; i < providerList.size(); ++i) {
                reader.Unload();
                reader.sourceArray = &providerList;

                json_spirit::mObject provider;
                reader.Read(provider, i);

                reader.Unload();
                reader.sourceObject = &provider;

                json_spirit::mArray providers;
                if (!reader.ReadIfExists(providers, "providers")) {
                    std::string v35, v34, sName; // v35 and v34 are supposedly unused. i'm guessing they're used on other platforms? (same for below)
                    if (!reader.ReadIfExists(sName, "name")) {
                        continue;
                    }

                    AddAdSystemFromJSON(reader, currencyData, privacyData.field_8);
                }

                for (size_t j = 0; j < providers.size(); ++j) {
                    reader.Unload();
                    reader.sourceArray = &providers;

                    json_spirit::mObject innerProvider;
                    reader.Read(innerProvider, j);

                    reader.Unload();
                    reader.sourceObject = &innerProvider;
                    AddAdSystemFromJSON(reader, currencyData, privacyData.field_8);
                }
            }
        }

        std::string v35, v34, sName;
        if (reader.ReadIfExists(sName, "name")) {
            AddAdSystemFromJSON(reader, currencyData, privacyData.field_8);
        }
    }

    std::string v18 = a3; // i'm sure something is done with a3 here. probably #ifdef'd out on steam platform

    if (m_bFetchOnLoad && m_pApp->GetNetworkType() == CApp::eNetworkType::Unk1) {
        for (const sCurrencyData& currencyData : m_vecCurrencyData) {
            m_dequeCurrencies.push_back(currencyData.sCurrency);
        }
    }
}

bool C_AdManager::IsOfferwallSupported() {
    for (auto it = m_mapInterfaces.begin(); it != m_mapInterfaces.end(); ++it) {
        for (C_AdInterface* pInterface : it->second) {
            if (pInterface->m_eWatchType == WATCHTYPE_OFFERWALL) {
                return true;
            }
        }
    }

    return false;
}

void C_AdManager::PrintConsentInfos() {
    for (auto it = m_mapInterfaces.begin(); it != m_mapInterfaces.end(); ++it) {
        for (C_AdInterface* pInterface : it->second) {
            pInterface->PrintConsent();
        }
    }
}

void C_AdManager::ShowAdTrackingRequest() {

}

bool C_AdManager::AdTrackingAllowed() {
    return m_bAdTrackingAllowed;
}

bool C_AdManager::AdTrackingRequestNeeded() {
    return false;
}

void C_AdManager::InitConsentProvider(const std::string&, bool) {

}

char* C_AdManager::ShowConsentRequest(bool a2) {
    if (field_F8) {
        return reinterpret_cast<char* (*)(char*, bool)>(field_F8 + 24)(field_F8, a2);
    } else {
        return nullptr;
    }
}

std::string C_AdManager::GetConsentData() {
    if (field_F8) {
        return "";
    } else {
        return *reinterpret_cast<const std::string*>(0x10); // this is definitely wrong
    }
}

bool C_AdManager::ConsentRequestNeeded() {
    return field_F8 != nullptr;
}
