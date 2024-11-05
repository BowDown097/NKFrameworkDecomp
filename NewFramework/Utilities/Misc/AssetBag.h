#pragma once

#include <NewFramework/Graphics/Management/TextureLoader.h>
#include <NewFramework/BaseApplication/BasePointers.h>

#include <boost/thread/pthread/recursive_mutex.hpp>
#include <boost/unordered/unordered_map.hpp>
#include <string>

class CAssetBag : public ITextureLoaderListener {
public:
    enum struct eAssetType {
        TEXTURE,
        FONT
    };
    enum struct eAssetState {
        INVALID,
        PENDING,
        LOADED
    };
    enum struct eSuspendedState {
        INVALID,
        SUSPENDED,
        UNKNOWN_STATE,
        ACTIVE
    };

    explicit CAssetBag(IBasePointers* basePointers, const std::string& source);

    bool Ready();
    void Reset();
    void Resume(eAssetType assetType, const std::string& assetName);
    void StartUsing(const eAssetType& type, const std::string& assetName);
    void StopUsing(const eAssetType& type, const std::string& assetName, bool erase);

    virtual ~CAssetBag();

    CTextureManager* m_pTextureManager = nullptr;
    CTextureLoader* m_pTextureLoader = nullptr;
    CFontManager* m_pFontManager = nullptr;
    boost::unordered::unordered_map<eAssetType, boost::unordered_map<std::string, eAssetState>> mActiveAssets{};
    std::string mAssetSource{};
    boost::unordered::unordered_map<eAssetType, boost::unordered_map<std::string, eSuspendedState>> mSuspendedAssets{};
    boost::recursive_mutex mRecursiveMutex{};
};