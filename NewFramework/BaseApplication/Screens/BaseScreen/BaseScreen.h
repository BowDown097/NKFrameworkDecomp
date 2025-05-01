#pragma once

#include "NewFramework/BaseApplication/BasePointers.h"
#include "NewFramework/BaseApplication/Screens/ScreenData.h"
#include "NewFramework/Utilities/Bitmask.h"
#include "NewFramework/Utilities/Misc/AssetBag.h"

#include <boost/shared_ptr.hpp>

#include <string>
#include <sstream>
#include <vector>

enum class eScreenFlags {
    Unk0 = 1 << 0,
    Unk1 = 1 << 1,
    Unk2 = 1 << 2,
    Unk3 = 1 << 3,
    Unk4 = 1 << 4
};

enum class eScreenProperties {
    Unk0 = 1 << 0,
    Unk1 = 1 << 1,
    Unk2 = 1 << 2,
    Unk3 = 1 << 3,
    Unk4 = 1 << 4,
    Unk5 = 1 << 5,
    Unk6 = 1 << 6,
    Unk7 = 1 << 7
};

enum class eScreenState {
    Uninitialized,
    Initialized,
    Shown,
    Unloaded,
    Hidden,
    Destroying,
    Destroyed
};

class CBasePositionableObject;
class CColouredButton;
class CFont;
class CRGBA;
class CSprite;
class CSpriteButton;
class CTextObject;
class CVec2;
struct SGameTime;
enum class SPRITE_ALIGNMENT;

class CBaseScreen : public IBasePointers {
public:
    explicit CBaseScreen(const std::string& sScreenName);
    virtual ~CBaseScreen();

    virtual void Init(IScreenData* pData);
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
    virtual void ChildScreenClosed(std::string sScreenName, bool a3) {}
    virtual void BatteryLevelDidChange() {}
    virtual void BatteryStateDidChange() {}
    virtual void ProcessInit(const SGameTime& gameTime);
    virtual void ProcessUninit(const SGameTime& gameTime);
    virtual void Process(const SGameTime& gameTime) {}
    virtual void Input(const SGameTime& gameTime) {}
    virtual void Draw() {}
    virtual void DrawOverlay() {}
    virtual bool AllAssetsLoaded();
    virtual void SetupPointers(CBaseScreen* pScreen);
    virtual void RemoveScreen(CBaseScreen* pScreen);
    virtual void PrintTree(uint uTabs, std::stringstream& stream);

    CBaseScreen* GetScreenRecursive(const std::string& sScreenName);
    void GetScreenListRecursive(std::list<CBaseScreen*>& listScreens);
    void Unload(CBaseScreen* pScreen);
    void UnloadAllExcept(const std::vector<std::string>& screenNames);
    void UnloadAllExcept(CBaseScreen* pScreen);
    void UnloadAllExcept(std::vector<CBaseScreen*> screens);
    void SyncScreens();
    CBaseScreen* GetScreen(const std::string& sScreenName);
    void Load(CBaseScreen* pScreen, IScreenData* pData, float fZValue, const Bitmask<uint, eScreenFlags>& eFlags);
    void CallBaseMethod(void (CBaseScreen::* method)(void));
    void CallBackButtonPressed(void (CBaseScreen::* method)(void));
    bool IsScreenPropertyRecursive(eScreenProperties eProperties);
    CBaseScreen* IsChildPresent(const std::string& sScreenName);
    int GetChildCount(const std::string& sScreenName);
    void StartUsingAsset(const CAssetBag::eAssetType&, const std::string&);
    void StopUsingAsset(const CAssetBag::eAssetType&, const std::string&);
    void SetupSprite(
        CSprite** ppSprite, CBasePositionableObject* pObject, CVec2 vec2,
        const std::string& sTexture, const std::string& sSprite,
        SPRITE_ALIGNMENT alignX, SPRITE_ALIGNMENT alignY);
    void SetupButton(
        const std::string& a2, CSpriteButton** ppButton, CBasePositionableObject* pObject,
        CVec2 vec2, const std::string& sTexture, const std::string& sSprite,
        SPRITE_ALIGNMENT alignX, SPRITE_ALIGNMENT alignY, bool a10);
    void SetupColouredButton(
        const std::string& a2, CColouredButton** ppButton, CBasePositionableObject* pObject,
        const CVec2& a5, const CVec2& a6, const CRGBA& colour,
        SPRITE_ALIGNMENT alignX, SPRITE_ALIGNMENT alignY, bool a10);
    void SetupTextObject(
        CTextObject** ppTextObject, CBasePositionableObject* pObject, boost::shared_ptr<CFont> pFont,
        CVec2 vec2, SPRITE_ALIGNMENT alignX, SPRITE_ALIGNMENT alignY,
        const std::string& a8, const CRGBA& colour);
    void CreateSpriteOrCompound(
        CSprite** ppSprite, CBasePositionableObject* pObject, CVec2 vec2,
        const std::string& sTexture, const std::string& sSprite,
        SPRITE_ALIGNMENT alignX, SPRITE_ALIGNMENT alignY);
protected:
    virtual void _PreloadAssets() {}
    void _Unload();
    void _ProcessVector(const SGameTime& gameTime);
    void _Process(const SGameTime& gameTime);
    void _InputVector(const SGameTime& gameTime);
    void _Input(const SGameTime& gameTime);
    void _DrawVector();
    void _Draw();
    void _Cleanup();
private:
    std::list<CBaseScreen*> m_children; // 0xE0
    std::vector<CBaseScreen*> m_loadedChildren; // 0xF8
    Bitmask<uint, eScreenProperties> m_eProperties; // 0x110
    std::string m_sScreenName; // 0x118
    CBaseScreen* m_pParentScreen{}; // 0x130
    eScreenState m_eState{}; // 0x138
    float m_fZValue{}; // 0x13C
    bool m_bLoaded{}; // 0x140
    boost::shared_ptr<CAssetBag> m_pAssetBag; // 0x148

    friend bool CompareScreenZValue(const CBaseScreen* lhs, const CBaseScreen* rhs);
};
