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
        ACTIVE,
        UNKNOWN_STATE,
        SUSPENDED
    };

    explicit CAssetBag(IBasePointers* basePointers, const std::string& source);

    bool Ready();
    void Reset();
    void Resume(eAssetType assetType, const std::string& assetName);
    void StartUsing(const eAssetType& type, const std::string&);

    virtual ~CAssetBag() = default;

    CTextureManager* m_pTextureManager = nullptr;
    CTextureLoader* m_pTextureLoader = nullptr;
    // Another field here iirc
    CFontManager* m_pFontManager = nullptr;
    boost::unordered::unordered_map<eAssetType, boost::unordered_map<std::string, eAssetState>> mActiveAssets{};
    boost::unordered::unordered_map<eAssetType, boost::unordered_map<std::string, eSuspendedState>> mSuspendedAssets{};
    // Unknown fields here
    std::string mAssetSource{};
    boost::recursive_mutex mRecursiveMutex{};
};