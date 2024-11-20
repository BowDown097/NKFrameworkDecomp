#pragma once
#include <string>

enum class DeviceParadigm { Phone, Tablet };
enum class DeviceScale { Low, High, Tablet, Ultra };

class CGlobalVariables
{
public:
    int screenWidth = 480; // 0x00
    int screenHeight = 320; // 0x04
    int field_8 = 480; // 0x08
    int field_C = 320; // 0x0C
    int maxTextureSize; // 0x10
    float field_14 = 16.0 / 9.0; // 0x14
    float field_18 = 4.0 / 3.0; // 0x18
    float field_1C = 1.43; // 0x1C
    float field_20 = 4.0 / 3.0; // 0x20
    float field_24 = 7; // 0x24
    int field_28{}; // 0x28
    float field_2C = 1; // 0x2C
    float viewScale = 1; // 0x30
    double field_34 = 1.0 / 128.0; // 0x34
    DeviceScale deviceScale{}; // 0x3C
    DeviceParadigm deviceParadigm; // 0x40
    int scaledWidth; // 0x44
    int scaledHeight; // 0x48
    int field_4C = 512; // 0x4C
    void* field_50{}; // 0x50
    float field_58{}; // 0x58
    float field_5C{}; // 0x5C
    bool field_60{}; // 0x60

    void GetPlatformName(std::string& os, std::string& platform);
    static CGlobalVariables* Instance();
    float GetDefaultSpriteScale();
private:
    static inline CGlobalVariables* pInstance;
};
