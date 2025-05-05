#pragma once

#include "FontDefinition.h"
#include "Uncategorized/ReferenceCounted.h"
#include "Uncategorized/RGBA.h"
#include "Uncategorized/types.h"

#include <boost/smart_ptr/shared_ptr.hpp>

class CTexture;

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
