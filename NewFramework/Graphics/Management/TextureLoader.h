#pragma once
#include "NewFramework/Graphics/Texture.h"
#include "NewFramework/IO/File/BaseFileIO.h"
#include <boost/thread.hpp>

// TODO: ONE missing enum value. it's over. need to figure out what this is
enum class eTextureLoaderEvent { Loaded, Failure, Unloaded, Loading, Val4, Updated };

struct ITextureLoaderListener
{
    virtual void TextureEvent(eTextureLoaderEvent event, CTexture* texture);
};

class CTextureLoader
{
public:
    boost::shared_ptr<boost::thread> thread; // 0x08
    boost::mutex textureMutex; // 0x18
    boost::mutex listenerMutex; // 0x58
    void* field_98{}; // 0x98
    std::list<CTexture*> textures; // 0xA0
    std::list<ITextureLoaderListener*> listeners; // 0xB8
    int field_D0{}; // 0xD0
    CBaseFileIO* fileIO; // 0xD8

    CTextureLoader(CBaseFileIO* fileIO) : fileIO(fileIO) {}
    virtual void AddTextureToLoad(CTexture* texture);
    void BeginASyncLoad();
    virtual void BlockingAddTexture(CTexture* texture);
    void Process();
    void BroadcastEvent(eTextureLoaderEvent event, CTexture* texture);
    bool LoadTexture(CTexture* texture);
    bool LoadTexture(CTexture* texture, CFilePolicy& filePolicy);
    void ForceLoad(CTexture* texture);
    void UpdateTexture(CTexture* texture);
    void UnloadTexture(CTexture* texture);
    bool GetAllTexturesLoaded();
    virtual void TextureLoaded(CTexture* texture);
    virtual void TextureFailed(CTexture* texture);
    bool TexturesLoading();
    bool TexturesToLoad();
    void ThreadLoader();
    virtual void BeginThread();
    virtual void EndThread();
    void AddListener(ITextureLoaderListener* listener);
    void RemoveListener(ITextureLoaderListener* listener);
    void ClearListeners();
};
