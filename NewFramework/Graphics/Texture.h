#pragma once
#include "Uncategorized/ReferenceCounted.h"
#include <cstdint>
#include <string>

enum class ePixelDataFormat {};
enum class ePixelDataType {};
enum class ePixelFormat { Unknown, RGBA8, RGBA4, RGBA, RGB8, RGB565, LA8, LA }; // still not sure if these names are right
enum class eTextureFilePolicy { Document, Cache, Unk1, Asset, Unk2, External };
enum class eTextureSize { Unknown, Low, High, Tablet, Ultra };
enum class eTextureState { None, Loading, Created, Loaded, Failure, Unloaded };
enum class eTextureType { PNG, PVR, JPG, JPNG, Empty };

// unofficial name
struct STextureDimensions
{
    uint32_t width; // 0x00
    uint32_t height; // 0x04
};

struct STextureRect
{
    uint32_t startX; // 0x00
    uint32_t startY; // 0x04
    STextureDimensions dimensions; // 0x08
};

class CTexture : public CReferenceCounted
{
public:
    STextureDimensions dimensions{}; // 0x10
    std::string id; // 0x18
    std::string path; // 0x30
    eTextureType type{}; // 0x48
    eTextureState state{}; // 0x4C
    void* field_50{}; // 0x50
    uint8_t* pixelData{}; // 0x58
    eTextureFilePolicy filePolicy = eTextureFilePolicy::Asset; // 0x60
    int platformData{}; // 0x64
    int pixels{}; // 0x68
    eTextureSize size; // 0x6C
    bool inUse{}; // 0x70
    bool applyAlpha{}; // 0x72
    bool wrap{}; // 0x74
    ePixelFormat pixelFormat1 = ePixelFormat::RGBA8; // 0x78
    ePixelFormat pixelFormat2 = ePixelFormat::RGBA8; // 0x7C
    void* field_80{}; // 0x80, unused outside of being read in SetPlatformData apparently?

    ~CTexture() override;
    void ReadData(int startX, int startY, int width, int height, uint8_t* dataOut);
    void PatchData(const STextureRect& sourceRect, const uint8_t* sourceData,
                   uint32_t startX, uint32_t startY, uint32_t width, uint32_t height,
                   void (*patchFunc)(const uint8_t*, uint32_t, uint8_t*, uint32_t));
    void UpdateData(int startX, int startY, int width, int height, const uint8_t* dataIn);
    void SetPlatformData(int platformData);
    static uint32_t GetTextureMemSize(uint32_t width, uint32_t height, ePixelFormat format);
    static std::string TextureSizeToString(const eTextureSize& size);
    static std::string TextureTypeToString(const eTextureType& type);
    static eTextureType ParseTextureType(const std::string& type);
};
