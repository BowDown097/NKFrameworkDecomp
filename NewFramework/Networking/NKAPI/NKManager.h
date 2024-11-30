#pragma once
#include "Impl/NKSessionImpl.h"
#include "Uncategorized/Blackboards.h"

class CBaseFileIO;
class CTextureManager;
class NKCrossPromo;
class NKModules;

class NKManager {
public:
    CBaseFileIO* fileIO{}; // 0x00
    std::uint32_t appID; // 0x08
    std::uint32_t skuID; // 0x0C
    std::string privateKey; // 0x10
    bool isSandbox; // 0x28
    bool initialised{}; // 0x30
    NKResponseUtilityTime serverTime; // 0x38
    time_t localTime; // 0x40
    time_t constructTime = time(nullptr); // 0x48

    static NKManager* GetManager();
    void Initialise(const int& appID, const int& skuID, const std::string& privateKey,
                    eNKLinkServerCluster serverCluster, CBaseFileIO* fileIO, CTextureManager* textureManager,
                    CEventManager* eventManager, const std::string& crossPromoKey, LoginCallback loginCallback);
    NKSession* GetSessionModule();
    void Update(const float& elapsed);
    NKCrossPromo* GetCrossPromo() const { return crossPromo; }
    NKModules* GetModules() const { return modules; }
    const std::string& GetPrivateKey() const { return privateKey; }
    bool IsSandbox() const { return isSandbox; }
    std::uint32_t GetAppID() const { return appID; }
    std::uint32_t GetSkuID() const { return skuID; }
    CBaseFileIO* GetFileIO() const { return fileIO; }
    void FetchNewServerTime(bool markUnavailable);
    std::int64_t UpdateServerTime();
    std::int64_t UpdateServerTimeResponse(const NKError* error, NKResponseUtilityTime time);
    std::int64_t GetLocalTimeMS() const;
    bool IsServerTimeAvailable() const { return serverTimeAvailable; }
    std::int64_t GetTimeMS() const;
    std::int64_t GetLocalTimeSinceLastServerTimeFetch() const { return GetLocalTimeMS() - localTime; }
    std::int64_t GetTimeTo(int year, int month, int day) const;
private:
    inline static NKManager* s_Manager;

    bool serverTimeAvailable{}; // 0x50
    bool serverTimeUpdating{}; // 0x51
    std::uint32_t updateRetries; // 0x54
    NKSession* session{}; // 0x58
    NKCrossPromo* crossPromo{}; // 0x60
    NKModules* modules{}; // 0x68
    SNKManagerBlackboard* blackboard{}; // 0x70
    std::string field_78; // 0x78
};
