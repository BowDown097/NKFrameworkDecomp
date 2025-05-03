#pragma once

#include "Uncategorized/ReferenceCounted.h"
#include "Uncategorized/RGBA.h"
#include "Uncategorized/types.h"

#include <boost/smart_ptr/shared_ptr.hpp>
#include <map>

class CTexture;

struct SCharacterInfo {
    uint uCode; // 0x00
    int field_4; // 0x04
    int field_8; // 0x08
    float field_C; // 0x0C
    float field_10; // 0x10
    float field_14; // 0x14
    float field_18; // 0x18
    float field_1C; // 0x1C
    double field_20; // 0x20
    double field_28; // 0x28
    double field_30; // 0x30
    double field_38; // 0x38
    std::map<uint, float> field_40; // 0x40
};

struct SFontDefinition {
    std::vector<CTexture*> textures; // 0x00
    std::map<uint, SCharacterInfo> characterInfoMap; // 0x18
    std::string field_30; // 0x30
    int field_48; // 0x48
    float fHeight; // 0x4C
    bool bIsSDF; // 0x50
};

class CFont : public CReferenceCounted {
public:
    CFont(
        const std::string& sName, float fSize,
        boost::shared_ptr<const SFontDefinition> pDefinition,
        bool a4, const CRGBA& colour, float fAlpha);
    SCharacterInfo GetCharacterInfo(uint uCode, bool bUseFallback) const;
    boost::shared_ptr<const SFontDefinition> GetDefinition() const;
    float GetLineHeight() const;
    bool HasCharacter(uint uCode) const;
private:
    boost::shared_ptr<const SFontDefinition> m_pDefinition; // 0x10
    float m_fSize; // 0x20
    std::string m_sName; // 0x28
    bool field_40; // 0x40
    CRGBA m_colour; // 0x44
    float m_fAlpha; // 0x48
    uint m_uCodeFallback{}; // 0x4C
};
