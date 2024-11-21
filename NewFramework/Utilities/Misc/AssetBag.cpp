#include "AssetBag.h"

#include <NewFramework/Graphics/Font/FontManager.h>
#include <NewFramework/Graphics/Management/TextureManager.h>
#include <NewFramework/Platform/Shared/Logging.h>

CAssetBag::CAssetBag(IBasePointers* basePointers, const std::string& source)
	: m_pTextureManager{basePointers->textureManager},
		m_pTextureLoader{basePointers->textureLoader},
		m_pFontManager{basePointers->fontManager},
		mAssetSource{source}
{
	m_pTextureLoader->AddListener(this);
}

bool CAssetBag::Ready() {
	boost::unique_lock lock{mRecursiveMutex};
	for (auto& [type, map] : mActiveAssets) {
		for (auto& [name, state] : map) {
			if (state == eAssetState::PENDING) {
				return false;
			}
		}
	}
	for (auto& [type, map] : mSuspendedAssets) {
		for (auto& [name, state] : map) {
			if (state == eSuspendedState::SUSPENDED) {
				return false;
			}
		}
	}
	return true;
}

void CAssetBag::Reset() {
	boost::unique_lock lock{mRecursiveMutex};
	for (auto& [type, map] : mActiveAssets) {
		for (auto& [name, state] : map) {
			StopUsing(type, name, false);
		}
	}
	for (auto& [type, map] : mSuspendedAssets) {
		for (auto& [name, state] : map) {
			Resume(type, name);
		}
	}
	mActiveAssets.clear();
	mSuspendedAssets.clear();
}




void CAssetBag::StartUsing(const eAssetType& type, const std::string& name) {
	boost::unique_lock lock{mRecursiveMutex};
	auto& activeOfType = mActiveAssets[type];
	if (activeOfType.find(name) == activeOfType.end()) {
		return;
	}

	switch(type) {
		case eAssetType::TEXTURE: {
			if (2 < m_pTextureManager->IncTextureRefCount(name, mAssetSource)) {
				LOG_ERROR("Texture not found? \"%s\"");
				ENFORCE_LINE(65);
			} else {
				if (m_pTextureManager->IsTextureLoaded(name)) {
					activeOfType[name] = eAssetState::LOADED;
				}
			}
			break;
		}




		
		case eAssetType::FONT: {
			auto textureName = m_pFontManager->GetFontTextureName(name);
			if (2 < m_pFontManager->IncFontRefCount(name, mAssetSource)) {
				LOG_ERROR("Font not found? \"%s\"");
				ENFORCE_LINE(82);
			} else {
				if (m_pTextureManager->IsTextureLoaded(textureName)) {
					activeOfType[name] = eAssetState::LOADED;
				}
			}
			break;
		}
		default:
			break;
	}
}

CAssetBag::~CAssetBag() {
	Reset();
	m_pTextureLoader->RemoveListener(this);
}

void CAssetBag::Resume(eAssetType assetType, const std::string& assetName) {
	boost::unique_lock lock{mRecursiveMutex};
	auto& suspendedOfType = mSuspendedAssets[assetType];
	if (suspendedOfType.find(assetName) == suspendedOfType.end()) {
		return;
	}

	bool resumed = false;
	switch(assetType) {
		case eAssetType::TEXTURE:
			resumed = m_pTextureManager->ResumeTexture(assetName);
		break;
		case eAssetType::FONT:
			resumed = m_pFontManager->ResumeFont(assetName);
			break;
		default:
			break;
	};

	suspendedOfType[assetName] = resumed ? eSuspendedState::ACTIVE : eSuspendedState::SUSPENDED;
}

void CAssetBag::StopUsing(const eAssetType& type, const std::string& assetName, bool erase) {
	boost::unique_lock lock{mRecursiveMutex};

	auto& activeOfType = mActiveAssets[type];
	if (activeOfType.find(assetName) == activeOfType.end()) {
		return;
	}
	switch(type) {
		case eAssetType::TEXTURE:
			m_pTextureManager->DecTextureRefCount(assetName, mAssetSource);
		break;
		case eAssetType::FONT:
			m_pFontManager->DecFontRefCount(assetName, mAssetSource);
			break;
		default:
			break;
	}
	if (erase) {
		activeOfType.erase(assetName);
	}
}
