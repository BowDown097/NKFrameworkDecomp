#pragma once
#include "NewFramework/Math/MathematicTypes/VectorDataStructures.h"
#include <cstdint>
#include <vector>

struct RectStruct
{
    float field_0; // 0x00
    float field_4; // 0x04
    float field_8; // 0x08
    float field_C; // 0x0C
};

struct SpriteInformation
{
    RectStruct rect; // 0x00
    int field_10; // 0x10
    int field_14; // 0x14
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    float field_28;
    float field_2C;
    int field_30;
    int field_34;
    int field_38;
    float field_3C;
    int field_40;
    int field_44;
    int field_48;
    int field_4C;
    int field_50;
    int field_54;
    int field_58;
    int field_5C;
    int field_60;
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
    int field_7C;
    int field_80;
    int field_84;
    std::vector<CVec2> field_88; // 0x88
    std::vector<CVec2> field_A0; // 0xA0
    std::string field_B8; // 0xB8
    int field_D0; // 0xD0
    float field_D4; // 0xD4

    void SetTexCoords(float, float, float, float, uint32_t, uint32_t);
    void SetGridOfTexCoords(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
    void GetSpriteInfo(float, float, float, float, uint32_t, uint32_t, CVec2, CVec2, CVec2);
    void GetSpriteInfo(float, float, float, float, uint32_t, uint32_t, CVec2, CVec2, CVec2, CVec2);
    void GetRotatedSpriteInfoCCW90(int);
    void GetFlippedSpriteInfo_AroundVerticalAxis();
    void GetFlippedSpriteInfo_AroundHorizontalAxis();
    void MoveCoords(CVec2);
    void ScaleCoords(float factor);
    void RotateTexCoordsCCW90(uint32_t);
    void FlipAroundVerticalAxis();
    void FlipAroundHorizontalAxis();
    void SubSpriteInfo(RectStruct);
};

using SSpriteInfo = SpriteInformation;
