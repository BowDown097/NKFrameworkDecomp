#include "BaseScreen.h"

#include "NewFramework/Platform/Shared/Assert.h"

#include <boost/smart_ptr/make_shared_object.hpp>

CBaseScreen::CBaseScreen(std::string& screenName) : IBasePointers(), mScreenName(screenName) {}
CBaseScreen::~CBaseScreen() = default;
void CBaseScreen::_PreloadAssets() {}
void CBaseScreen::Init(IScreenData* screenData)
{
    if(this->m_pAssetBag == nullptr) {
        this->m_pAssetBag = boost::make_shared<CAssetBag>(this);
    }
    this->_PreloadAssets();
    this->mScreenState = eScreenState::INITIALIZED;
}
void CBaseScreen::Uninit()
{
    if(this->m_pAssetBag != nullptr) {
        this->m_pAssetBag->Reset();
    }
    this->mScreenState = eScreenState::DESTROYED;
}
void CBaseScreen::Show()
{
    this->mScreenState = eScreenState::SHOWN;
}
void CBaseScreen::Hide()
{
    this->mScreenState = eScreenState::HIDDEN;
}
void CBaseScreen::ApplicationWillResignActive()
{
    for(CBaseScreen* pChild : this->mChildren) {
        //TODO: Figure out what happens here
    }
}
void CBaseScreen::ApplicationWillTerminate()
{
    for(CBaseScreen* pChild : this->mChildren) {
        //TODO: Figure out what happens here
    }
}
void CBaseScreen::ApplicationDidBecomeActive()
{
    for(CBaseScreen* pChild : this->mChildren) {
        //TODO: Figure out what happens here
    }
}
void CBaseScreen::SelectButtonPressed()
{
    for(CBaseScreen* pChild : this->mChildren) {
        //TODO: Figure out what happens here
    }
}
void CBaseScreen::BackButtonPressed()
{
    for(CBaseScreen* pChild : this->mChildren) {
        //TODO: Figure out what happens here
    }
}
void CBaseScreen::ScreenResized()
{
    //Weird stack stuff here, might just be big params?
}
std::string CBaseScreen::ChildScreenClosed(std::string screenName, bool param_2)
{
    return screenName;
}
void CBaseScreen::BatteryLevelDidChange()
{

}
void CBaseScreen::ProcessInit()
{
    bool loaded = this->AllAssetsLoaded();
    if(loaded != false) {
        //TODO: Figure out the rest of this
    }
}
void CBaseScreen::ProcessUninit()
{
    bool loaded = this->AllAssetsLoaded();
    if(loaded == false) {
        this->mScreenState = eScreenState::DESTROYING;
    }
}
void CBaseScreen::Process(const SGameTime& pGameTime)
{

}
void CBaseScreen::Input(const SGameTime& pGameTime)
{

}
void CBaseScreen::Draw()
{

}
void CBaseScreen::DrawOverlay()
{

}
bool CBaseScreen::AllAssetsLoaded()
{
    if(this->m_pAssetBag != nullptr) {
        bool ready = this->m_pAssetBag->Ready();
        if(ready != false) {
            return true;
        }
    }
    return false;
}
void CBaseScreen::SetupPointers(CBaseScreen* child)
{
    child->mParentScreen = this;
    child->CopyFrom(*this);
}
void CBaseScreen::RemoveScreen(CBaseScreen* child)
{
	auto it = mChildren.begin();
	while (it != mChildren.end()) {
		if (*it == child) {
			mChildren.erase(it);
			break;
		}
		++it;
	}
}
void CBaseScreen::PrintTree(int param_1, std::stringstream outstream)
{
    //TODO: Figure out this code (its mostly useless afaik so like meh)
}

void CBaseScreen::StartUsingAsset(CAssetBag::eAssetType& assetType, std::string& assetName)
{
    NKAssert(m_pAssetBag != nullptr, "Asset Bag doesn't exist! Maybe you've not called BaseScreen::Init() yet. You should probably do that.");
    this->m_pAssetBag->StartUsing(assetType, assetName);
}