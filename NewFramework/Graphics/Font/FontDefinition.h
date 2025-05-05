#pragma once

#include <algorithm>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

class CTexture;

struct SCharacterInfo {
    uint32_t uCode; // 0x00
    float fX; // 0x04
    float fY; // 0x08
    float fWidth; // 0x0C
    float fHeight; // 0x10
    float fXOffset; // 0x14
    float fYOffset; // 0x18
    float fXAdvance; // 0x1C
    float field_20; // 0x20
    float field_24; // 0x24
    float field_28; // 0x28
    float field_2C; // 0x2C
    float field_30; // 0x30
    float field_34; // 0x34
    float field_38; // 0x38
    float field_3C; // 0x3C
    std::map<uint32_t, float> kerningMap; // 0x40
};

struct SFontDefinition {
    std::vector<CTexture*> textures; // 0x00
    std::map<uint32_t, SCharacterInfo> characterInfoMap; // 0x18
    std::string field_30; // 0x30
    uint32_t uSize; // 0x48
    float fLineHeight; // 0x4C
    bool bSDF; // 0x50

    // speculative: avoids repeating code
    inline auto MatchCharacter(uint32_t uCode) {
        return std::find_if(characterInfoMap.begin(), characterInfoMap.end(), [uCode](const auto& p) {
            return p.first >= uCode;
        });
    }

    inline auto MatchCharacter(uint32_t uCode) const {
        return const_cast<SFontDefinition*>(this)->MatchCharacter(uCode);
    }
};
