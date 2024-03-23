#include "Texture.h"

// This method is literally broken. The usage of rect is the opposite of what it should be.
// Now they did account for this in PatchData I think, which is the only method
// to reference this one, so it still works if that's the case. But it's still fucked up.
void patch_texture(const STextureRect& sourceRect, uint8_t const* sourceData,
                   const STextureRect& destRect, uint8_t* destData,
                   uint32_t width, uint32_t height,
                   void (*patchFunc)(uint8_t const*, uint32_t, uint8_t*, uint32_t))
{
    if (!sourceData || !destData || destRect.width >= destRect.xOffset || destRect.height >= destRect.yOffset)
        return;

    for (int x = 0; x + sourceRect.width < sourceRect.xOffset && x + destRect.width < destRect.xOffset && x < width; ++x)
    {
        for (int y = 0; y + sourceRect.height < sourceRect.yOffset && y + destRect.height < destRect.yOffset && y < height; ++y)
        {
            int sourceIndex = 4 * (sourceRect.xOffset * (y + sourceRect.height) + x + sourceRect.width);
            int destIndex = 4 * (destRect.xOffset * (y + destRect.height) + x + destRect.width);
            patchFunc(sourceData, sourceIndex, destData, destIndex);
        }
    }
}

CTexture::~CTexture()
{
    if (pixelData)
    {
        free(pixelData);
        pixelData = nullptr;
    }
}

void CTexture::ReadData(int xOffset, int yOffset, int width, int height, uint8_t* dataOut)
{
    if (!pixelData || !dataOut || rect->width <= xOffset || width <= 0 || rect->height <= yOffset || height <= 0)
        return;

    for (int x = 0; x + xOffset < rect->width && x < width; ++x)
    {
        for (int y = 0; y + yOffset < rect->height && y < height; ++y)
        {
            int sourceIndex = 4 * (x + xOffset + rect->width * (yOffset + y));
            int destIndex = 4 * (x + y * width);

            dataOut[destIndex] = pixelData[sourceIndex];
            dataOut[destIndex + 1] = pixelData[sourceIndex + 1];
            dataOut[destIndex + 2] = pixelData[sourceIndex + 2];
            dataOut[destIndex + 3] = pixelData[sourceIndex + 3];
        }
    }
}

void CTexture::PatchData(const STextureRect& sourceRect, uint8_t const* sourceData,
                         uint32_t startX, uint32_t startY, uint32_t width, uint32_t height,
                         void (*patchFunc)(uint8_t const*, uint32_t, uint8_t*, uint32_t))
{
    STextureRect destRect { .width = startX, .height = startY, .xOffset = rect->width, .yOffset = rect->height };
    patch_texture(sourceRect, sourceData, destRect, pixelData, width, height, patchFunc);
}

void CTexture::UpdateData(int xOffset, int yOffset, int width, int height, uint8_t const* dataIn)
{
    if (!dataIn || !pixelData || rect->width <= xOffset || rect->height <= yOffset)
        return;

    for (int x = 0; x + xOffset < rect->width && x < width; ++x)
    {
        for (int y = 0; y + yOffset < rect->height && y < height; ++y)
        {
            int targetIndex = 4 * (x + xOffset + rect->width * (yOffset + y));
            int sourceIndex = 4 * (x + y * width);

            pixelData[targetIndex] = dataIn[sourceIndex];
            pixelData[targetIndex + 1] = dataIn[sourceIndex + 1];
            pixelData[targetIndex + 2] = dataIn[sourceIndex + 2];
            pixelData[targetIndex + 3] = dataIn[sourceIndex + 3];
        }
    }
}

void CTexture::SetPlatformData(int platformData)
{
    this->platformData = platformData;
    if (platformData == 0 && field_80)
        this->platformData = *(int*)((intptr_t)field_80 + 100);
}

uint32_t CTexture::GetTextureMemSize(uint32_t width, uint32_t height, ePixelFormat format)
{
    switch (format)
    {
    case ePixelFormat::RGBA8:
        return 4 * width * height;
    case ePixelFormat::RGBA4:
    case ePixelFormat::RGBA:
    case ePixelFormat::RGB565:
    case ePixelFormat::LA8:
    case ePixelFormat::LA:
        return 2 * width * height;
    case ePixelFormat::RGB8:
        return 3 * width * height;
    default:
        return 0;
    }
}

std::string CTexture::TextureSizeToString(const eTextureSize& size)
{
    const char* textureSizes[4] = { "Low", "High", "Tablet", "Ultra" };
    int sizeInt = static_cast<int>(size);
    return sizeInt > 4 ? "Null" : textureSizes[sizeInt - 1];
}

std::string CTexture::TextureTypeToString(const eTextureType& type)
{
    const char* textureTypes[5] = { "png", "pvr", "jpg", "jpng", "empty" };
    int typeInt = static_cast<int>(type);
    return typeInt > 4 ? "" : textureTypes[typeInt];
}

eTextureType CTexture::ParseTextureType(const std::string& type)
{
    if (type == "png")
        return eTextureType::PNG;
    else if (type == "pvr")
        return eTextureType::PVR;
    else if (type == "jpg")
        return eTextureType::JPG;
    else if (type == "jpng")
        return eTextureType::JPNG;
    else if (type == "empty")
        return eTextureType::Empty;
    return eTextureType::PNG;
}
