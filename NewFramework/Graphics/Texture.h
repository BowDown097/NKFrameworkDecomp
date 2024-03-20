#ifndef CTEXTURE_H
#define CTEXTURE_H
#include "Uncategorized/ReferenceCounted.h"
#include <climits>
#include <cstdint>
#include <string>

enum class eTextureSize { Low, High, Tablet, Ultra };
enum class eTextureType { PNG, PVR, JPG, JPNG, Empty };

// unofficial name :(
// unsure of Loaded (i think it means loaded but not rendered, field_70 might give more clues)
enum class eTextureState { None, Loading, Created, Loaded, Failure };

struct STextureRect
{
    uint32_t width; // 0x00
    uint32_t height; // 0x04
    uint32_t xOffset; // 0x08
    uint32_t yOffset; // 0x0C
};

class CTexture : public CReferenceCounted
{
public:
    // still not sure if these names are right
    enum class ePixelFormat { Unknown, RGBA8, RGBA4, RGBA, RGB8, RGB565, LA8, LA };

    int field_8 = INT_MAX; // 0x08
    STextureRect* rect{}; // 0x10
    std::string id; // 0x18
    std::string path; // 0x30
    eTextureType type{}; // 0x48
    eTextureState state{}; // 0x4C
    void* field_50{}; // 0x50
    uint8_t* pixelData{}; // 0x58
    int field_60{}; // 0x60
    int platformData = 3; // 0x64
    int field_68{}; // 0x68, need information from CMemoryTracker::TextureEvent to know what this is, may be bytes
    eTextureSize size; // 0x6C
    bool field_70{}; // 0x70, need information from GLTextureLoader::TextureEvent to know what this is
    bool field_72{}; // 0x72, need information from CTextureLoader::LoadTexture to know what this is
    int repeat{}; // 0x74, controls repeating the texture in GLTextureLoader::TextureEvent
    ePixelFormat pixelFormat1 = ePixelFormat::RGBA8; // 0x78
    ePixelFormat pixelFormat2 = ePixelFormat::RGBA8; // 0x7C
    void* field_80{}; // 0x80, unused outside of SetPlatformData which is completely weird lol

    void DeletePixelData();
    void ReadData(int xOffset, int yOffset, int width, int height, uint8_t* dataOut);
    void PatchData(const STextureRect& sourceRect, uint8_t const* sourceData,
                   uint32_t startX, uint32_t startY, uint32_t width, uint32_t height,
                   void (*patchFunc)(uint8_t const*, uint32_t, uint8_t*, uint32_t));
    void UpdateData(int xOffset, int yOffset, int width, int height, uint8_t const* dataIn);
    void SetPlatformData(int platformData);
    static uint32_t GetTextureMemSize(uint32_t width, uint32_t height, ePixelFormat format);
    static std::string TextureSizeToString(const eTextureSize& size);
    static std::string TextureTypeToString(const eTextureType& type);
    static eTextureType ParseTextureType(const std::string& type);
};

#endif // CTEXTURE_H
