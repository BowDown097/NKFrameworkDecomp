#include "Texture.h"

void patch_texture(const STextureRect& sourceRect, const uint8_t* sourceData,
                   const STextureRect& destRect, uint8_t* destData,
                   uint32_t width, uint32_t height,
                   void (*patchFunc)(const uint8_t*, uint32_t, uint8_t*, uint32_t))
{
    if (!sourceData || !destData ||
        destRect.dimensions.width <= destRect.startX || destRect.dimensions.height <= destRect.startY)
    {
        return;
    }

    for (int x = 0; x + sourceRect.startX < sourceRect.dimensions.width &&
                    x + destRect.startX < destRect.dimensions.width &&
                    x < width; ++x)
    {
        for (int y = 0; y + sourceRect.startY < sourceRect.dimensions.height &&
                        y + destRect.startY < destRect.dimensions.height &&
                        y < height; ++y)
        {
            int sourceIndex = 4 * (sourceRect.dimensions.width * (y + sourceRect.startY) + (x + sourceRect.startX));
            int destIndex = 4 * (destRect.dimensions.width * (y + destRect.startY) + (x + destRect.startX));
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

void CTexture::ReadData(int startX, int startY, int width, int height, uint8_t* dataOut)
{
    if (!pixelData || !dataOut || dimensions.width <= startX || width <= 0 || dimensions.height <= startY || height <= 0)
        return;

    for (int x = 0; x + startX < dimensions.width && x < width; ++x)
    {
        for (int y = 0; y + startY < dimensions.height && y < height; ++y)
        {
            int sourceIndex = 4 * ((x + startX) + dimensions.width * (y + startY));
            int destIndex = 4 * (x + y * width);

            dataOut[destIndex] = pixelData[sourceIndex];
            dataOut[destIndex + 1] = pixelData[sourceIndex + 1];
            dataOut[destIndex + 2] = pixelData[sourceIndex + 2];
            dataOut[destIndex + 3] = pixelData[sourceIndex + 3];
        }
    }
}

void CTexture::PatchData(const STextureRect& sourceRect, const uint8_t* sourceData,
                         uint32_t startX, uint32_t startY, uint32_t width, uint32_t height,
                         void (*patchFunc)(const uint8_t*, uint32_t, uint8_t*, uint32_t))
{
    STextureRect destRect {
        .startX = startX,
        .startY = startY,
        .dimensions = STextureDimensions { .width = this->dimensions.width, .height = this->dimensions.height }
    };

    patch_texture(sourceRect, sourceData, destRect, pixelData, width, height, patchFunc);
}

void CTexture::UpdateData(int startX, int startY, int width, int height, const uint8_t* dataIn)
{
    if (!dataIn || !pixelData || dimensions.width <= startX || dimensions.height <= startY)
        return;

    for (int x = 0; x + startX < dimensions.width && x < width; ++x)
    {
        for (int y = 0; y + startY < dimensions.height && y < height; ++y)
        {
            int targetIndex = 4 * ((x + startX) + dimensions.width * (startY + y));
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
