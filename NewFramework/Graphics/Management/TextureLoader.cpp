#include "TextureLoader.h"
#include "JPEG/JPEGTextures.h"
#include "JPNG/JPNGTextures.h"
#include "NewFramework/Platform/Shared/Core.h"
#include "NewFramework/Platform/Shared/Logging.h"
#include "NewFramework/ThirdParty/libpng/load_png.h"
#include "PVR/PVRTextures.h"

void CTextureLoader::AddTextureToLoad(CTexture* texture)
{
    boost::unique_lock<boost::mutex> lock(textureMutex);
    if (texture)
    {
        texture->state = eTextureState::Loading;
        textures.push_back(texture);
        if (!thread)
            BeginASyncLoad();
    }
}

void CTextureLoader::BeginASyncLoad()
{
    if (!thread)
        thread = boost::make_shared<boost::thread>(boost::bind(&CTextureLoader::ThreadLoader, this));
}

void CTextureLoader::BlockingAddTexture(CTexture* texture)
{
    boost::unique_lock<boost::mutex> lock(textureMutex);
    if (texture)
    {
        texture->state = eTextureState::Loading;
        textures.push_back(texture);
    }
}

void CTextureLoader::Process()
{
    boost::unique_lock<boost::mutex> lock(textureMutex);
    for (CTexture* texture : textures)
    {
        switch (texture->state)
        {
        case eTextureState::Loading:
            break;
        case eTextureState::Created:
        case eTextureState::Loaded:
        case eTextureState::Failure:
            if (texture->state == eTextureState::Created)
                TextureLoaded(texture);
            BroadcastEvent(
                texture->state == eTextureState::Failure ? eTextureLoaderEvent::Failure : eTextureLoaderEvent::Loaded,
                texture
            );
        case eTextureState::Unloaded:
            textures.remove(texture);
            break;
        default:
            LOG_ERROR("Unhandled texture state, potential memory leaks and thread locks ahoy!");
            break;
        }
    }
}

void CTextureLoader::BroadcastEvent(eTextureLoaderEvent event, CTexture* texture)
{
    boost::unique_lock<boost::mutex> lock(listenerMutex);
    for (ITextureLoaderListener* listener : listeners)
        listener->TextureEvent(event, texture);
}

bool CTextureLoader::LoadTexture(CTexture* texture)
{
    return LoadTexture(texture, fileIO->assetPolicy);
}

bool CTextureLoader::LoadTexture(CTexture* texture, CFilePolicy& filePolicy)
{
    textureMutex.lock();
    std::string typeString = texture->TextureTypeToString(texture->type);
    std::string path = texture->path;
    eTextureType type = texture->type;
    bool inUse = texture->inUse;
    uint8_t* pixelData = texture->pixelData;
    textureMutex.unlock();

    if (inUse && pixelData)
    {
        textureMutex.lock();
        texture->state = eTextureState::Created;
        textureMutex.unlock();
        return true;
    }

    if (typeString.empty())
        return false;

    if (!fileIO->FileExists(path, filePolicy))
    {
        textureMutex.lock();
        texture->state = eTextureState::Failure;
        textureMutex.unlock();
        return false;
    }

    uint8_t* loadedPixelData{};
    int width = 1, height = 1;
    uint32_t platformData{}, loadedPixels{};

    switch (type)
    {
    case eTextureType::PNG:
        loadedPixelData = load_png(fileIO, path.c_str(), filePolicy, &width, &height);
        loadedPixels = 4 * width * height;
        break;
    case eTextureType::PVR:
        loadedPixelData = LoadPVRTexture(path.c_str(), &width, &height, &platformData, &loadedPixels);
        break;
    case eTextureType::JPG:
        loadedPixelData = LoadJPEGTexture(fileIO, path, filePolicy, &width, &height, &loadedPixels);
        break;
    case eTextureType::JPNG:
        loadedPixelData = LoadJPNGTexture(fileIO, path, &width, &height, &loadedPixels);
        break;
    case eTextureType::Empty:
        break;
    default:
        LOG_ERROR("CTextureLoader::LoadTexture No File Type Supplied: %s", path.c_str());
        break;
    }

    if (!loadedPixelData)
    {
        textureMutex.lock();
        texture->state = eTextureState::Failure;
        textureMutex.unlock();
        return false;
    }

    if (texture->applyAlpha && loadedPixels / (width * height) == 4)
    {
        for (int i = 0; i < loadedPixels; i += 4)
        {
            uint8_t alpha = loadedPixelData[i + 3];
            loadedPixelData[i] = alpha * loadedPixelData[i] / 255;
            loadedPixelData[i + 1] = alpha * loadedPixelData[i + 1] / 255;
            loadedPixelData[i + 2] = alpha * loadedPixelData[i + 2] / 255;
        }
    }

    textureMutex.lock();

    texture->dimensions.width = width;
    texture->dimensions.height = height;
    texture->pixels = loadedPixels;
    texture->SetPlatformData(platformData);

    if (pixelData && !texture->inUse)
        free(pixelData);

    texture->pixelData = loadedPixelData;
    texture->state = eTextureState::Created;

    textureMutex.unlock();

    return true;
}

void CTextureLoader::ForceLoad(CTexture* texture)
{
    TextureLoaded(texture);
    BroadcastEvent(eTextureLoaderEvent::Loaded, texture);
}

void CTextureLoader::UpdateTexture(CTexture* texture)
{
    BroadcastEvent(eTextureLoaderEvent::Updated, texture);
}

void CTextureLoader::UnloadTexture(CTexture* texture)
{
    textureMutex.lock();

    if (textures.size() >= 2)
    {
        for (CTexture* t : textures)
        {
            if (t == texture)
            {
                textures.remove(t);
            }
        }
    }

    textureMutex.unlock();
    BroadcastEvent(eTextureLoaderEvent::Unloaded, texture);
}

bool CTextureLoader::GetAllTexturesLoaded()
{
    for (CTexture* texture : textures)
    {
        if (texture->state != eTextureState::Loaded)
        {
            if (!thread)
                LOG_ERROR("All textures not loaded and texture loading thread non-existant, this will probably get stuck, how did this happen?.");
            return false;
        }
    }

    return true;
}

void CTextureLoader::TextureLoaded(CTexture* texture)
{
    texture->state = eTextureState::Loaded;
}

bool CTextureLoader::TexturesLoading()
{
    boost::unique_lock<boost::mutex> lock(textureMutex);
    return thread != nullptr;
}

bool CTextureLoader::TexturesToLoad()
{
    boost::unique_lock<boost::mutex> lock(textureMutex);
    return !textures.empty();
}

void CTextureLoader::ThreadLoader()
{
    CCore::SetThreadName("Tex Loader");
    CCore::SetThreadID(2);
    BeginThread();
    textureMutex.lock();

    for (CTexture* texture : textures)
    {
        if (texture->state == eTextureState::Loading)
        {
            textureMutex.unlock();

            switch (texture->filePolicy)
            {
            case eTextureFilePolicy::Document:
                LoadTexture(texture, fileIO->documentPolicy);
                break;
            case eTextureFilePolicy::Cache:
                LoadTexture(texture, fileIO->cachePolicy);
                break;
            case eTextureFilePolicy::Asset:
                LoadTexture(texture, fileIO->assetPolicy);
                break;
            case eTextureFilePolicy::External:
                LoadTexture(texture, fileIO->externalPolicy);
                break;
            default:
                break;
            }

            textureMutex.lock();
        }

        textureMutex.unlock();
        textureMutex.lock();
    }

    BroadcastEvent(eTextureLoaderEvent::Loading, nullptr);
    CCore::UnsetThreadID();
    EndThread();
    textureMutex.unlock();
}

void CTextureLoader::EndThread()
{
    thread.reset();
}

void CTextureLoader::AddListener(ITextureLoaderListener* listener)
{
    boost::unique_lock<boost::mutex> lock(listenerMutex);
    listeners.push_back(listener);
}

void CTextureLoader::RemoveListener(ITextureLoaderListener* listener)
{
    boost::unique_lock<boost::mutex> lock(listenerMutex);
    for (ITextureLoaderListener* l : listeners)
    {
        if (l == listener)
        {
            listeners.remove(l);
        }
    }
}

void CTextureLoader::ClearListeners()
{
    boost::unique_lock<boost::mutex> lock(listenerMutex);
    listeners.clear();
}
