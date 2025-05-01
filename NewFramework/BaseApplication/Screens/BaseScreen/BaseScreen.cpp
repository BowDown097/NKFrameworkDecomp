#include "BaseScreen.h"

#include "NewFramework/BaseApplication/LegacyFeatures/LegacyFeatures.h"

bool CompareScreenZValue(const CBaseScreen* lhs, const CBaseScreen* rhs) {
    return lhs->m_fZValue > rhs->m_fZValue;
}

CBaseScreen::CBaseScreen(const std::string& sScreenName)
    : IBasePointers(),
      m_sScreenName(sScreenName) {}

CBaseScreen::~CBaseScreen() {
    for (CBaseScreen* pChild : m_children) {
        if (pChild) {
            delete pChild;
            pChild = nullptr;
        }
    }
}

void CBaseScreen::Init(IScreenData*) {
    if (!m_pAssetBag) {
        m_pAssetBag = boost::make_shared<CAssetBag>(this, m_sScreenName);
    }
    _PreloadAssets();
    m_eState = eScreenState::Initialized;
}

void CBaseScreen::Uninit() {
    if (m_pAssetBag) {
        m_pAssetBag->Reset();
    }
    m_eState = eScreenState::Destroyed;
}

void CBaseScreen::Show() {
    m_eState = eScreenState::Shown;
}

void CBaseScreen::Hide() {
    m_eState = eScreenState::Hidden;
}

void CBaseScreen::ApplicationWillResignActive() {
    for (CBaseScreen* pChild : m_children) {
    	pChild->ApplicationWillResignActive();
    }
}

void CBaseScreen::ApplicationWillTerminate() {
    for (CBaseScreen* pChild : m_children) {
    	pChild->ApplicationWillTerminate();
    }
}

void CBaseScreen::ApplicationDidBecomeActive() {
    for (CBaseScreen* pChild : m_children) {
    	pChild->ApplicationDidBecomeActive();
    }
}

#ifdef PLATFORM_HAS_SELECT_BUTTON
void CBaseScreen::SelectButtonPressed() {
    for (CBaseScreen* pChild : m_children) {
    	pChild->SelectButtonPressed();
    }
}
#endif

void CBaseScreen::BackButtonPressed() {
    if (!LegacyFeatures::bUseNewerBackButtonbehaviour) {
        for (CBaseScreen* pChild : m_children) {
            pChild->BackButtonPressed();
        }
    }
}

void CBaseScreen::ScreenResized() {
    for (CBaseScreen* pChild : m_children) {
        if (pChild && pChild->m_eState == eScreenState::Shown) {
            pChild->ScreenResized();
        }
    }
}

void CBaseScreen::ProcessInit(const SGameTime&) {
    if (!AllAssetsLoaded()) {
        return;
    }

    for (CBaseScreen* pChild : m_loadedChildren) {
        if (pChild->m_eState != eScreenState::Shown) {
            return;
        }
    }

    if (m_bLoaded) {
        Show();
    }

    if ((m_eProperties & eScreenProperties::Unk4) != 0) {
        /*for (CBaseScreen* pChild : pScreenManager->m_children) { // TODO: comment this out when CScreenManager is implemented
            if (pChild != this) {
                Unload(pChild);
            }
        }*/
    }
}

void CBaseScreen::ProcessUninit(const SGameTime&) {
    if (!AllAssetsLoaded()) {
        m_eState = eScreenState::Destroying;
    }
}

bool CBaseScreen::AllAssetsLoaded() {
    return m_pAssetBag != nullptr && m_pAssetBag->Ready();
}

void CBaseScreen::SetupPointers(CBaseScreen* pScreen) {
    pScreen->m_pParentScreen = this;
    pScreen->CopyFrom(*this);
}

void CBaseScreen::RemoveScreen(CBaseScreen* pScreen) {
    m_children.remove(pScreen);
}

void CBaseScreen::PrintTree(uint uTabs, std::stringstream& stream) {
    std::string tree;
    for (uint i = 0; i < uTabs; ++i) {
        tree += "\t";
    }

    tree += m_sScreenName;
    tree += "  ";

    if ((m_eProperties & eScreenProperties::Unk3) != 0) {
        tree += "I";
    }
    if ((m_eProperties & eScreenProperties::Unk1) != 0) {
        tree += "P";
    }
    if ((m_eProperties & eScreenProperties::Unk2) != 0) {
        tree += "D";
    }

    stream << tree << "\n";

    for (CBaseScreen* pChild : m_children) {
        pChild->PrintTree(++uTabs, stream);
    }
}

CBaseScreen* CBaseScreen::GetScreenRecursive(const std::string& sScreenName) {
    for (CBaseScreen* pChild : m_children) {
        if (pChild->m_sScreenName == sScreenName) {
            return pChild;
        }
        CBaseScreen* pScreen = pChild->GetScreenRecursive(sScreenName);
        if (pScreen) {
            return pScreen;
        }
    }
    return nullptr;
}

void CBaseScreen::GetScreenListRecursive(std::list<CBaseScreen*>& listScreens) {
    for (CBaseScreen* pChild : m_children) {
        listScreens.push_back(pChild);
        pChild->GetScreenListRecursive(listScreens);
    }
}

void CBaseScreen::Unload(CBaseScreen* pScreen) {
    pScreen->m_eState = eScreenState::Unloaded;
    for (CBaseScreen* pChild : pScreen->m_children) {
        Unload(pChild);
    }
}

void CBaseScreen::UnloadAllExcept(const std::vector<std::string>& screenNames) {
    for (CBaseScreen* pChild : m_children) {
        auto it = std::find(screenNames.begin(), screenNames.end(), pChild->m_sScreenName);
        if (it == screenNames.end()) {
            Unload(pChild);
        }
    }
}

void CBaseScreen::UnloadAllExcept(CBaseScreen* pScreen) {
    for (CBaseScreen* pChild : m_children) {
        if (pChild != pScreen) {
            Unload(pChild);
        }
    }
}

void CBaseScreen::UnloadAllExcept(std::vector<CBaseScreen*> screens) {
    for (CBaseScreen* pChild : m_children) {
        auto it = std::find(screens.begin(), screens.end(), pChild);
        if (it == screens.end()) {
            Unload(pChild);
        }
    }
}

void CBaseScreen::SyncScreens() {
    m_children.sort(&CompareScreenZValue);
    std::copy(m_children.begin(), m_children.end(), std::back_inserter(m_loadedChildren));
}

CBaseScreen* CBaseScreen::GetScreen(const std::string& sScreenName) {
    if (m_sScreenName == sScreenName) {
        return this;
    }

    for (CBaseScreen* pChild : m_children) {
        if (pChild->m_sScreenName == sScreenName) {
            return pChild;
        }
        CBaseScreen* pScreen = pChild->GetScreen(sScreenName);
        if (pScreen) {
            return pScreen;
        }
    }

    return nullptr;
}

void CBaseScreen::Load(CBaseScreen* pScreen, IScreenData* pData, float fZValue, const Bitmask<uint, eScreenFlags>& eFlags) {
    if ((eFlags & eScreenFlags::Unk0) != 0) {
        for (CBaseScreen* pChild : m_children) {
            Unload(pChild);
        }
    }

    if ((eFlags & eScreenFlags::Unk4) != 0) {
        pScreen->m_eProperties |= eScreenProperties::Unk4;
    }

    SetupPointers(pScreen);
    pScreen->m_fZValue = fZValue;

    if (pScreen->m_eState == eScreenState::Uninitialized) {
        pScreen->Init(pData);
    }

    m_children.push_back(pScreen);

    if ((eFlags & eScreenFlags::Unk2) == 0) {
        pScreen->m_bLoaded = true;
    }

    if ((eFlags & eScreenFlags::Unk1) == 0) {
        m_loadedChildren.push_back(pScreen);
    }
}

void CBaseScreen::CallBaseMethod(void (CBaseScreen::* method)(void)) {
    for (CBaseScreen* pChild : m_children) {
        (pChild->*method)();
    }
}

void CBaseScreen::CallBackButtonPressed(void (CBaseScreen::* method)(void)) {
    if (LegacyFeatures::bUseNewerBackButtonbehaviour) {
        bool bFoundChild = false;
        for (size_t i = 0; i < m_loadedChildren.size(); ++i) {
            CBaseScreen* pChild = m_loadedChildren[i];
            if (pChild->m_fZValue > 0 && !bFoundChild) {
                if ((m_eProperties & eScreenProperties::Unk5) == 0) {
                    BackButtonPressed();
                }
                bFoundChild = true;
                if (IsScreenPropertyRecursive(eScreenProperties::Unk3)) {
                    return;
                }
            }
            pChild->CallBackButtonPressed(method);
            if (pChild->IsScreenPropertyRecursive(eScreenProperties::Unk3)) {
                return;
            }
        }

        if (!bFoundChild && (m_eProperties & eScreenProperties::Unk5) == 0) {
            BackButtonPressed();
        }
    } else {
        for (CBaseScreen* pChild : m_children) {
            if ((pChild->m_eProperties & eScreenProperties::Unk5) == 0) {
                (pChild->*method)();
            }
            if (pChild->IsScreenPropertyRecursive(eScreenProperties::Unk3)) {
                return;
            }
        }
    }
}

bool CBaseScreen::IsScreenPropertyRecursive(eScreenProperties eProperties) {
    if ((m_eProperties & eProperties) != 0) {
        return true;
    }

    if (LegacyFeatures::bScreenRecursiveBlockingCheck) {
        for (size_t i = 0; i < m_loadedChildren.size(); ++i) {
            if (m_loadedChildren[i]->IsScreenPropertyRecursive(eProperties)) {
                return true;
            }
        }
    }

    return false;
}

CBaseScreen* CBaseScreen::IsChildPresent(const std::string& sScreenName) {
    for (CBaseScreen* pChild : m_children) {
        if (pChild->m_sScreenName == sScreenName) {
            return pChild;
        }
        CBaseScreen* pScreen = pChild->IsChildPresent(sScreenName);
        if (pScreen) {
            return pScreen;
        }
    }
    return nullptr;
}

int CBaseScreen::GetChildCount(const std::string& sScreenName) {
    int result = 0;
    for (CBaseScreen* pChild : m_children) {
        if (pChild->m_sScreenName == sScreenName) {
            ++result;
        }
        result += pChild->GetChildCount(sScreenName);
    }
    return result;
}




































































































































































































































































































































































































































































void CBaseScreen::StartUsingAsset(const CAssetBag::eAssetType& eAssetType, const std::string& sAssetName) {
    NKAssert(m_pAssetBag != nullptr, "Asset Bag doesn't exist! Maybe you've not called BaseScreen::Init() yet. You should probably do that."); ENFORCE_LINE(794);
    m_pAssetBag->StartUsing(eAssetType, sAssetName);
}

void CBaseScreen::StopUsingAsset(const CAssetBag::eAssetType& eAssetType, const std::string& sAssetName) {
    if (m_pAssetBag) {
        m_pAssetBag->StopUsing(eAssetType, sAssetName, true);
    }
}

/*void CBaseScreen::SetupSprite( // TODO: comment this out when CSprite and CTextureManager are implemented
    CSprite** ppSprite, CBasePositionableObject* pObject, CVec2 vec2,
    const std::string& sTexture, const std::string& sSprite,
    SPRITE_ALIGNMENT alignX, SPRITE_ALIGNMENT alignY) {

    *ppSprite = new CSprite(vec2, pTextureManager->GetSpriteInfoPtr(sTexture, sSprite), false);
    (*ppSprite)->SetAlignmentXY(alignX, alignY);

    if (pObject) {
        (*ppSprite)->SetParent(pObject);
    }
}*/

/*void CBaseScreen::SetupButton( // TODO: comment this out when CSpriteButton and CTextureManager are implemented
    const std::string& a2, CSpriteButton** ppButton, CBasePositionableObject* pObject,
    CVec2 vec2, const std::string& sTexture, const std::string& sSprite,
    SPRITE_ALIGNMENT alignX, SPRITE_ALIGNMENT alignY, bool a10) {

    *ppButton = new CSpriteButton(pInput, vec2, a2, pTextureManager->GetSpriteInfoPtr(sTexture, sSprite), a10);
    (*ppButton)->SetAlignmentXY(alignX, alignY);

    if (pObject) {
        (*ppButton)->SetParent(pObject);
    }
}*/

/*void CBaseScreen::SetupColouredButton( // TODO: comment this out when CColouredButton is implemented
    const std::string& a2, CColouredButton** ppButton, CBasePositionableObject* pObject,
    const CVec2& a5, const CVec2& a6, const CRGBA& colour,
    SPRITE_ALIGNMENT alignX, SPRITE_ALIGNMENT alignY, bool a10) {

    *ppButton = new CColouredButton(pInput, a2, a10, a5, a6);
    (*ppButton)->SetColour(colour, static_cast<eColouredCorner>(4));
    (*ppButton)->SetAlignmentXY(alignX, alignY);

    if (pObject) {
        (*ppButton)->SetParent(pObject);
    }
}*/

/*void CBaseScreen::SetupTextObject( // TODO: comment this out when CTextObject is implemented
    CTextObject** ppTextObject, CBasePositionableObject* pObject, boost::shared_ptr<CFont> pFont,
    CVec2 vec2, SPRITE_ALIGNMENT alignX, SPRITE_ALIGNMENT alignY,
    const std::string& a8, const CRGBA& colour) {

    *ppTextObject = new CTextObject(vec2, pFont, a8);
    (*ppTextObject)->SetAlignmentXY(alignX, alignY);
    (*ppTextObject)->SetParent(pObject);
    (*ppTextObject)->SetColour(colour, static_cast<eColouredCorner>(0));
    *((bool*)*ppTextObject + 25) = true;
}*/

/*void CBaseScreen::CreateSpriteOrCompound( // TODO: comment this out when CAnimationSystem, CCompoundSprite, CSprite, CTextureManager, and SCompoundSprite are implemented
    CSprite** ppSprite, CBasePositionableObject* pObject, CVec2 vec2,
    const std::string& sTexture, const std::string& sSprite,
    SPRITE_ALIGNMENT alignX, SPRITE_ALIGNMENT alignY) {

    std::string sJsonExt = ".json";
    if (sSprite.size() <= sJsonExt.size() || sSprite.rfind(sJsonExt) != (sSprite.size() - sJsonExt.size())) {
        *ppSprite = new CSprite(vec2, pTextureManager->GetSpriteInfoPtr(sTexture, sSprite), false);
    } else {
        std::string sPath;
        std::string sFilename;
        StringHelper::SplitPathAndFilename(sSprite, sPath, sFilename);

        SCompoundSprite* pCompound = pTextureManager->LoadCompound(sPath + "/", sFilename, true);
        CCompoundSprite* pCompoundSprite = new CCompoundSprite(vec2, pCompound, nullptr);
        pCompoundSprite->m_pAnimationSystem->PlayLooping(false);
        *ppSprite = pCompoundSprite;
    }

    (*ppSprite)->SetAlignmentXY(alignX, alignY);
    if (pObject) {
        (*ppSprite)->SetParent(pObject);
    }
}*/

void CBaseScreen::_Unload() {
    for (auto it = m_children.begin(); it != m_children.end(); it = m_children.erase(it)) {
        (*it)->_Unload();
    }

    Uninit();
    delete this;
}

void CBaseScreen::_ProcessVector(const SGameTime& gameTime) {
    SyncScreens();

    bool bFoundChild = false;
    for (size_t i = 0; i < m_loadedChildren.size(); ++i) {
        CBaseScreen* pChild = m_loadedChildren[i];
        if (pChild->m_fZValue > 0.0 && !bFoundChild) {
            CBaseScreen* pParentScreen = m_pParentScreen;
            std::string sScreenName = m_sScreenName;
            if ((m_eProperties & eScreenProperties::Unk6) == 0) {
                _Process(gameTime);
            }
            bFoundChild = true;
            if ((m_pParentScreen && m_sScreenName != sScreenName && !pParentScreen->GetScreen(sScreenName)) ||
                IsScreenPropertyRecursive(eScreenProperties::Unk0)) {
                return;
            }
        }
        pChild->_ProcessVector(gameTime);
        if (pChild->IsScreenPropertyRecursive(eScreenProperties::Unk0)) {
            return;
        }
    }

    if (!bFoundChild && (m_eProperties & eScreenProperties::Unk6) == 0) {
        _Process(gameTime);
    }
}

void CBaseScreen::_Process(const SGameTime& gameTime) {
    /*if (pScreenManager == this) { // TODO: comment this out when CScreenManager is implemented
        return;
    }*/

    if (m_eState == eScreenState::Unloaded) {
        Hide();
    }

    switch (m_eState) {
    case eScreenState::Initialized:
        ProcessInit(gameTime);
        break;
    case eScreenState::Shown:
        Process(gameTime);
        break;
    case eScreenState::Hidden:
    case eScreenState::Destroyed:
        ProcessUninit(gameTime);
        break;
    case eScreenState::Destroying:
        if (AllAssetsLoaded()) {
            m_eState = eScreenState::Hidden;
        }
        break;
    default: return;
    }
}

void CBaseScreen::_InputVector(const SGameTime& gameTime) { // TODO: comment out pScreenManager parts when CScreenManager is implemented
    SyncScreens();

    bool bFoundChild = false;
    for (size_t i = 0; i < m_loadedChildren.size(); ++i) {
        CBaseScreen* pChild = m_loadedChildren[i];
        if (pChild->m_fZValue > 0.0 && !bFoundChild) {
            if ((m_eProperties & eScreenProperties::Unk5) == 0 /*&& pScreenManager != this*/ && m_eState == eScreenState::Shown) {
                Input(gameTime);
            }
            bFoundChild = true;
            if (IsScreenPropertyRecursive(eScreenProperties::Unk3)) {
                return;
            }
        }
        pChild->_InputVector(gameTime);
        if (pChild->IsScreenPropertyRecursive(eScreenProperties::Unk3)) {
            return;
        }
    }

    if (!bFoundChild && (m_eProperties & eScreenProperties::Unk5) == 0 /*&& pScreenManager != this*/ && m_eState == eScreenState::Shown) {
        Input(gameTime);
    }
}

void CBaseScreen::_Input(const SGameTime& gameTime) { // TODO: comment out pScreenManager part when CScreenManager is implemented
    if (/*pScreenManager != this &&*/ m_eState == eScreenState::Shown) {
        Input(gameTime);
    }
}

void CBaseScreen::_DrawVector() { // TODO: comment out pScreenManager parts when CScreenManager is implemented
    int match = 0, i = 0;
    for (auto it = m_children.begin(); it != m_children.end(); ++i, ++it) {
        CBaseScreen* pChild = *it;
        if (pChild->IsScreenPropertyRecursive(eScreenProperties::Unk2)) {
            if (pChild->m_eState != eScreenState::Hidden && pChild->m_eState != eScreenState::Destroyed) {
                match = i;
            }
        }
    }

    int j = 0;
    bool bFoundChild = false;

    for (auto it = m_children.begin(); it != m_children.end(); ++j, ++it) {
        CBaseScreen* pChild = *it;
        if (pChild->m_fZValue < 0.0 && !bFoundChild) {
            bFoundChild = true;
            if ((m_eProperties & eScreenProperties::Unk7) == 0 /*&& pScreenManager != this*/ && m_eState == eScreenState::Shown) {
                Draw();
            }
        }
        if (j >= match) {
            pChild->_DrawVector();
        }
    }

    if (!bFoundChild && (m_eProperties & eScreenProperties::Unk7) == 0 /*&& pScreenManager != this*/ && m_eState == eScreenState::Shown) {
        Draw();
    }

    DrawOverlay();
}

void CBaseScreen::_Draw() { // TODO: comment out pScreenManager part when CScreenManager is implemented
    if (/*pScreenManager != this &&*/ m_eState == eScreenState::Shown) {
        Draw();
    }
}

void CBaseScreen::_Cleanup() {
    SyncScreens();
    if (m_eState == eScreenState::Hidden || m_eState == eScreenState::Destroyed) {
        m_pParentScreen->RemoveScreen(this);
    } else {
        for (size_t i = 0; i < m_loadedChildren.size(); ++i) {
            m_loadedChildren[i]->_Cleanup();
        }
    }
}
