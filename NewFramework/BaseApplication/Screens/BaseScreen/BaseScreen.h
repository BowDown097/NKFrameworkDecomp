#pragma once

#include <NewFramework/BaseApplication/Screens/ScreenData.h>
#include <NewFramework/BaseApplication/BasePointers.h>
#include <NewFramework/Utilities/Misc/AssetBag.h>

#include <boost/shared_ptr.hpp>

#include <string>
#include <sstream>
#include <vector>

enum class Device {
    PHONE = 0,
    TABLET = 1,
    UNKNOWN = 2
};

enum class eScreenState {
    UNINITIALIZED,
    INITIALIZED,
    SHOWN,
    UNKNOWN_3,
    HIDDEN,
    DESTROYING,
    DESTROYED
};

struct SGameTime;

class CBaseScreen : public IBasePointers {
    std::vector<CBaseScreen*> mChildren; //0x007C
    std::string mScreenName; //0x008C
    CBaseScreen* mParentScreen = nullptr; //0x00A4
    eScreenState mScreenState = eScreenState::UNINITIALIZED; //0x00A8
    boost::shared_ptr<CAssetBag> m_pAssetBag = nullptr; //0x00B0
public:
    CBaseScreen(std::string& screenName);
    virtual ~CBaseScreen();
    virtual void _PreloadAssets();
    virtual void Init(IScreenData* screenData);
    virtual void Uninit();
    virtual void Show();
    virtual void Hide();
    virtual void ApplicationWillResignActive();
    virtual void ApplicationWillTerminate();
    virtual void ApplicationDidBecomeActive();
#ifdef PLATFORM_HAS_SELECT_BUTTON
    virtual void SelectButtonPressed();
#endif
    virtual void BackButtonPressed();
    virtual void ScreenResized();
    virtual std::string ChildScreenClosed(std::string screenName, bool param_2);
    virtual void BatteryLevelDidChange();
    virtual void ProcessInit();
    virtual void ProcessUninit();
    virtual void Process(const SGameTime& gameTime);
    virtual void Input(const SGameTime& gameTime);
    virtual void Draw();
    virtual void DrawOverlay();
    virtual bool AllAssetsLoaded();
    virtual void SetupPointers(CBaseScreen* child);
    virtual void RemoveScreen(CBaseScreen* child);
    virtual void PrintTree(int param_1, std::stringstream outStream);

    void StartUsingAsset(CAssetBag::eAssetType& assetType, std::string& assetName);
};