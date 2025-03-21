#include "NKCrossPromo.h"
#include "NKFileClient.h"
#include "NKManager.h"
#include "NKModules.h"
#include "NKSession.h"
#include "NewFramework/Networking/NKAPI/Modules/NKModuleUtility.h"
#include <boost/bind/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

NKManager* NKManager::GetManager() {
    if (!s_Manager) {
        s_Manager = new NKManager;
    }

    return s_Manager;
}

NKManager::NKManager() {
    constructTime = time(nullptr);
}

NKManager::~NKManager() {
    if (session) {
        delete session;
    }
    if (modules) {
        delete modules;
    }
    if (blackboard) {
        delete blackboard;
    }
    if (crossPromo) {
        delete crossPromo;
    }
}

void NKManager::Initialise(const int& appID, const int& skuID, const std::string& privateKey,
                           eNKLinkServerCluster serverCluster, CBaseFileIO* fileIO, CTextureManager* textureManager,
                           CEventManager* eventManager, const std::string& crossPromoKey, LoginCallback loginCallback) {
    this->appID = appID;
    this->skuID = skuID;
    this->privateKey = privateKey;
    this->isSandbox = serverCluster != eNKLinkServerCluster::LIVE;
    this->fileIO = fileIO;
    this->blackboard = new SNKManagerBlackboard;
    this->modules = new NKModules;
    this->initialised = true;
    this->crossPromo = new NKCrossPromo(this, &NKFileClient::s_Instance, fileIO, textureManager, crossPromoKey);

    GetSessionModule()->Initialise(appID, skuID, privateKey, eventManager, serverCluster, loginCallback);
}

NKSession* NKManager::GetSessionModule() {
    if (!session) {
        session = new NKSession;
    }

    return session;
}

void NKManager::Update(const float& elapsed) {
    GetSessionModule()->Update(elapsed);
}

void NKManager::FetchNewServerTime(bool markUnavailable) {
    if (serverTimeUpdating) {
        return;
    }

    if (markUnavailable) {
        serverTimeAvailable = false;
    }

    updateRetries = 0;
    UpdateServerTime();
}

void NKManager::UpdateServerTime() {
    serverTimeUpdating = true;

    NKMessageUtilityTime time;
    modules->Utilities()->GetTime(time,
        boost::bind(&NKManager::UpdateServerTimeResponse, this, boost::placeholders::_1, boost::placeholders::_2));
}

void NKManager::UpdateServerTimeResponse(const NKError* error, NKResponseUtilityTime time) {
    serverTimeUpdating = false;

    if (error) {
        if (updateRetries++ <= 1) {
            UpdateServerTime();
        }
    } else {
        serverTime = time;
        localTime = GetLocalTimeMS();
        serverTimeAvailable = true;
    }
}

time_t NKManager::GetLocalTimeMS() const {
    boost::posix_time::ptime epoch(boost::gregorian::date(1970, 1, 1));
    boost::posix_time::ptime now = boost::posix_time::microsec_clock::universal_time();
    boost::posix_time::time_duration duration = now - epoch;
    return duration.total_milliseconds();
}

time_t NKManager::GetTimeMS() const {
    static uint32_t uUnavailableCounter;
    time_t result = GetLocalTimeMS();

    if (serverTimeAvailable) {
        uUnavailableCounter = 0;
        return serverTime.date.time + result - localTime;
    }
    else {
        uUnavailableCounter++;
    }

    return result;
}

time_t NKManager::GetTimeTo(int year, int month, int day) const {
    boost::posix_time::ptime epoch(boost::gregorian::date(1970, 1, 1));
    boost::posix_time::ptime target(boost::gregorian::date(year, month, day));
    boost::posix_time::time_duration duration = target - epoch;
    return duration.total_milliseconds();
}
