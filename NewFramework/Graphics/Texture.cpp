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

    for (int i = 0; i + sourceRect.width < sourceRect.xOffset && i < width;)
    {
        for (int j = 0; j + sourceRect.height < sourceRect.yOffset && j < height;)
        {
            patchFunc(sourceData, 4 * (sourceRect.xOffset * (j + sourceRect.height) + i + sourceRect.width),
                      destData, 4 * (destRect.xOffset * destRect.height + i + destRect.width));
            if ((++j + destRect.height) >= destRect.yOffset)
                break;
        }

        if ((destRect.width + ++i) >= destRect.xOffset)
            break;
    }
}

void CTexture::DeletePixelData()
{
    if (!pixelData)
        return;

    free(pixelData);
    pixelData = nullptr;
}

void CTexture::ReadData(int xOffset, int yOffset, int width, int height, uint8_t* dataOut)
{
    if (!pixelData || !dataOut || rect->width <= xOffset || width <= 0 || rect->height <= yOffset || height <= 0)
        return;

    uint8_t* theActualData = dataOut + 3;
    for (int i = 0, j = 0; j + xOffset < rect->width;)
    {
        uint8_t* pixel = &theActualData[i];
        for (int k = 1;; pixel += 4 * width, ++k)
        {
            int pixelInd = 4 * (j + xOffset + rect->width * (yOffset + k - 1));
            *(pixel - 3) = pixelData[pixelInd];
            *(pixel - 2) = pixelData[pixelInd + 1];
            *(pixel - 1) = pixelData[pixelInd + 2];
            *pixel = pixelData[pixelInd + 3];
            if (yOffset + k >= rect->height || k >= height)
                break;
        }

        ++j, i += 4;
        if (j >= width)
            break;
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

    for (int i = 4 * (yOffset * rect->width + xOffset), j = 0, k = 0; k != width;)
    {
        for (int l = i, m = j, n = 0; height != n;)
        {
            pixelData[l] = dataIn[m];
            ++n, m += 4 * width, l += 4 * rect->width;
            if (rect->height - yOffset == n)
                break;
        }

        ++k, j += 4, i += 4;
        if (k == rect->width - xOffset)
            break;
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
