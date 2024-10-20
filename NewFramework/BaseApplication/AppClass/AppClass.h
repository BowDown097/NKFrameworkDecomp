#pragma once
#include "NewFramework/BaseApplication/BasePointers.h"
#include <cfloat>
#include <string>

class CVersion;

class CApp : public IBasePointers // IBasePointers occupies 0x08-0xD8
{
public:
    enum class eNetworkType;
    enum class eREACH_Availability;
    enum class eRequiredDLCState;

    struct SRealtimeNetworkSettings;

    CApp();
    virtual ~CApp();

    void* field_D8; // 0xD8
    class CMemoryTracker* memoryTracker{}; // 0xE0
    class CTimerTracker* timerTracker{}; // 0xE8
    class CTimerTrackerGraph* timerTrackerGraph{}; // 0xF0
    eREACH_Availability reachAvailability{}; // 0xF8
    int field_FC{}; // 0xFC
    int field_100; // 0x100
    bool field_104; // 0x104
    bool quitFlag; // 0x105
    bool field_106; // 0x106
    bool hackerAppURLFlag{}; // 0x107
    bool hackerBinaryFlag{}; // 0x108
    bool field_109{}; // 0x109
    int field_10C{}; // 0x10C
    double field_110 = DBL_MAX; // 0x110
    CVersion* field_118; // 0x118

    bool NotesEnabled();
    class CAgeGate* GetAgeGate();
    void SetRequiredDLCState(eRequiredDLCState);
    class CStoreInterface* CreateStore();
    void LoadRendering();
    void DrawView();
    void StartFrame();
    void EndFrame();
    void DebugDraw();
    void ApplicationWillResignActive();
    void ApplicationWillTerminate();
    void ApplicationDidBecomeActive();
    void ApplicationGainedAudioFocus();
    void ApplicationLostAudioFocus();
    void ForceQuit();
    void ConnectivityChanged(eREACH_Availability);
    eREACH_Availability GetGoogleReachable();
    eNetworkType GetNetworkType();
    void BackButtonPressed();
    void ScreenResized();
    void BatteryLevelDidChange();
    void BatteryStateDidChange();
    void PreRenderInit();
    bool LoadContent(bool);
    void InitRealtimeNetworking(const SRealtimeNetworkSettings&);
    bool ReadSpriteInfo();
    bool SetupFonts();
    bool SetupSprites();
    std::string GetLanguageCode();
    void InitValidation();
    class CLegacyNetworkQueue* BuildNetworkQueue(const SRealtimeNetworkSettings&);
    void CustomDebugDraw();
    void DidReceiveMemoryWarning();
    bool HandleURI(const std::string&);
    bool GetTreatPlayerAsHacker();
    eRequiredDLCState GetRequiredDLCState();
    void DownloadMissingDLC();
    void DLCFileDownloadFinished(bool, const std::string&, const std::string&, int);
private:
    static int m_gameTime;
};
