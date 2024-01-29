#ifndef APPCLASS_H
#define APPCLASS_H
#include "NewFramework/BaseApplication/BasePointers.h"
#include <cfloat>

class CApp : public IBasePointers // IBasePointers occupies 0x08-0xD8
{
public:
    enum class eNetworkType;
    enum class eREACH_Availability;
    enum class eRequiredDLCState;

    struct SRealtimeNetworkSettings;

    CApp();
    virtual ~CApp();

    class CMemoryTracker* memoryTracker = nullptr; // 0xE0
    class CTimerTracker* timerTracker = nullptr; // 0xE8
    class CTimerTrackerGraph* timerTrackerGraph = nullptr; // 0xF0
    eREACH_Availability reachAvailability; // 0xF8
    bool quitFlag = false; // 0x105
    bool hackerAppURLFlag = false; // 0x107
    bool hackerBinaryFlag = false; // 0x108
    double field_110 = DBL_MAX; // 0x110

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

#endif // APPCLASS_H
