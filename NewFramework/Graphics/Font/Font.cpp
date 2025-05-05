#include "Font.h"

#include "NewFramework/Graphics/Management/TextureManager.h"

CFont::CFont(
    const std::string& sName, float fSize,
    boost::shared_ptr<const SFontDefinition> pDefinition,
    bool a4, const CRGBA& colour, float fAlpha)
    : m_pDefinition(pDefinition),
      m_fSize(fSize),
      m_sName(sName),
      field_40(a4),
      m_colour(colour),
      m_fAlpha(fAlpha) {}

SCharacterInfo CFont::GetCharacterInfo(uint uCode, bool bUseFallback) const {
    if (m_pDefinition) {
        auto it = m_pDefinition->MatchCharacter(uCode);
        if (it != m_pDefinition->characterInfoMap.end() && it->first == uCode) {
            return it->second;
        }
    }

    if (bUseFallback && m_uCodeFallback != 0 && m_uCodeFallback != uCode) {
        return GetCharacterInfo(m_uCodeFallback, true);
    } else {
        return SCharacterInfo();
    }
}

boost::shared_ptr<const SFontDefinition> CFont::GetDefinition() const {
    return m_pDefinition;
}

float CFont::GetLineHeight() const {
    if (m_pDefinition) {
        return CTextureManager::GetSpriteScale(m_pDefinition->textures.front()->size) * m_pDefinition->fLineHeight * m_fSize;
    } else {
        return 0.0f;
    }
}

bool CFont::HasCharacter(uint uCode) const {
    if (m_pDefinition) {
        auto it = m_pDefinition->MatchCharacter(uCode);
        return it != m_pDefinition->characterInfoMap.end() && it->first == uCode;
    } else {
        return false;
    }
}
