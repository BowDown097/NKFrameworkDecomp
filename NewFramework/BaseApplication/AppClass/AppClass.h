#pragma once

#include "NewFramework/BaseApplication/BasePointers.h"
#include "Uncategorized/Version.h"

#include <boost/thread.hpp>

enum class eREACH_Availability;

class CApp : public IBasePointers {
public:
    enum class eNetworkType {
        Unk0,
        Unk1
    };

    enum class eRequiredDLCState;

    struct SRealtimeNetworkSettings;

    class CMemoryTracker* pMemoryTracker; // 0xE0
    class CTimerTracker* pTimerTracker; // 0xE8
    class CTimerTrackerGraph* pTimerTrackerGraph; // 0xF0
    eREACH_Availability eReachAvailability{}; // 0xF8
    uint field_FC{}; // 0xFC
    uint field_100{}; // 0x100
    bool field_104{}; // 0x104
    bool bQuitFlag{}; // 0x105
    bool field_106{}; // 0x106
    bool bHackerAppURLFlag = true; // 0x107
    bool bHackerBinaryFlag = true; // 0x108
    bool field_109{}; // 0x109
    bool field_10A{}; // 0x10A
    bool field_10B{}; // 0x10B
    bool field_10C{}; // 0x10C
    bool field_10D{}; // 0x10D
    bool field_10E{}; // 0x10E
    bool field_10F{}; // 0x10F
    double field_110 = std::numeric_limits<double>::max(); // 0x110
    CVersion version; // 0x118
    std::string field_138; // 0x138
    std::string field_150; // 0x150
    int field_168{}; // 0x168
    bool bUsingAssetBundles = true; // 0x16C
    bool bIsActive = true; // 0x16D
    boost::thread* pRenderingThread; // 0x170
    boost::barrier* pRenderingBarrier; // 0x178
    boost::mutex* pRenderingMutex; // 0x180

    virtual std::string GetGameId() = 0;
    virtual ulong GetAgeGate();
    virtual void SetRequiredDLCState(eRequiredDLCState);
    virtual class CStoreInterface* CreateStore();
    virtual void _StartFrame();
    virtual ~CApp();
    void LoadRendering(void* pGlView);
    virtual void DrawView();
    void StartFrame();
    void EndFrame();
    void DebugDraw();
    virtual void ApplicationWillResignActive();
    virtual void ApplicationWillTerminate();
    virtual void ApplicationDidBecomeActive();
    virtual void ApplicationGainedAudioFocus();
    virtual void ApplicationLostAudioFocus();
    virtual void NativeSurfaceCreated();
    void ForceQuit();
    virtual void ConnectivityChanged(eREACH_Availability);
    virtual eREACH_Availability GetGoogleReachable();
    virtual eNetworkType GetNetworkType();
    virtual void BackButtonPressed();
    virtual void ScreenResized();
    void BatteryLevelDidChange();
    void BatteryStateDidChange();
    virtual void PreRenderInit();
    virtual bool LoadContent();
    virtual bool LoadContent(bool);
    void InitRealtimeNetworking(const SRealtimeNetworkSettings&);
    bool ReadSpriteInfo();
    virtual bool SetupFonts();
    virtual bool SetupSprites();
    std::string GetLanguageCode();
    virtual void InitValidation();
    class CLegacyNetworkQueue* BuildNetworkQueue(const SRealtimeNetworkSettings&);
    virtual void CustomDebugDraw();
    virtual bool NotesEnabled();
    virtual void DidReceiveMemoryWarning();
    virtual bool HandleURI(const std::string&);
    virtual bool GetTreatPlayerAsHacker();
    virtual eRequiredDLCState GetRequiredDLCState();
    virtual void DownloadMissingDLC();
    virtual void DLCFileDownloadFinished(bool, const std::string&, const std::string&, int);
private:
    static int m_gameTime;
};
