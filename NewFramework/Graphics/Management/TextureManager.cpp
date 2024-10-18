#include "TextureManager.h"
#include "NewFramework/Platform/Shared/Logging.h"

CTextureManager::CTextureManager(CTextureLoader* textureLoader, CBaseFileIO* fileIO)
    : textureLoader(textureLoader), fileIO(fileIO) {}

void CTextureManager::Update(const double&)
{
    if (textureLoader->TexturesToLoad())
        textureLoader->Process();
}

// TODO: I believe the implementation of this method is very different on other platforms,
// at the very least on Windows. Would be good to look and see what's different.
void CTextureManager::PrepareTextures(const std::string& texturesPath, ticpp::Element* spriteInfo)
{
    for (ticpp::Element* sprite = spriteInfo->FirstChildElement(false); sprite != nullptr;
         sprite = sprite->NextSiblingElement(false))
    {
        if (sprite->Value() != "Texture")
            continue;

        std::string name = sprite->GetAttribute<std::string>("name");
        std::string type = sprite->GetAttribute<std::string>("type");
        std::string targetFormat = sprite->GetAttribute<std::string>("target_format", false);
        std::string wrap = sprite->GetAttribute<std::string>("wrap", false);
        std::string premultiplyAlpha = sprite->GetAttribute<std::string>("premultiply_alpha", false);

        CTexture* texture = AddTexture(texturesPath, name, CTexture::ParseTextureType(type), premultiplyAlpha == "y");
        if (!texture)
            continue;

        texture->wrap = wrap == "true";
        if (targetFormat != "RGB_565" && !targetFormat.empty())
            LOG_ERROR("Unhandled target format: '%s'.", targetFormat.c_str());
        else
            texture->pixelFormat2 = CTexture::ePixelFormat::RGB565;
    }
}

CTexture* CTextureManager::AddTexture(const std::string& texturesPath, const std::string& name,
                                      const eTextureType& type, bool premultiplyAlpha)
{
    return AddTexture(texturesPath, name, type, premultiplyAlpha, fileIO->assetPolicy);
}

void CTextureManager::PrepareCells(const std::string& texturesPath, ticpp::Element* frameInfo, eTextureSize size)
{
    std::string textureName = frameInfo->GetAttribute<std::string>("texturename", false);
    std::string type = frameInfo->GetAttribute<std::string>("type");

    if (!textureName.empty())
        AddTexture(texturesPath, textureName, CTexture::ParseTextureType(type), false);

    frameInfo->GetAttribute<int>("texw", &currentFrame.texw, false);
    frameInfo->GetAttribute<int>("texh", &currentFrame.texh, false);
    frameInfo->GetAttribute<int>("flipx", &currentFrame.flipx, false);
    frameInfo->GetAttribute<int>("flipy", &currentFrame.flipy, false);

    for (ticpp::Element* texture = frameInfo->FirstChildElement(false); texture != nullptr;
         texture = texture->NextSiblingElement(false))
    {
        SSpriteInfo* spriteInfo = CreateSpriteInfoFromElement(texture, size);
        if (spriteInfo)
        {
            std::string spriteName = texture->GetAttribute<std::string>("name");
            AddSpriteInfoStruct(textureName, spriteName, spriteInfo);
        }
        else if (texture->Value() == "Animation")
        {
            SAnimationInfo* animation = CreateAnimationFromElement(texture, false, nullptr);
            AddAnimationStruct(textureName, "", animation); // TODO: replace "" with what seems to be field 0 of animation
        }
    }
}
