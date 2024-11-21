#include "Font.h"

#include "NewFramework/Graphics/Management/TextureManager.h"

CFont::CFont(const std::string& fontName, float fontSize, const boost::shared_ptr<const SFontDefinition>& definition, bool param_4, const CRGBA& color, float alpha) {
	mpDefinition = definition;
	mFontSize = fontSize;
	mName = fontName;
	mUnknownC = param_4;
	mColor = color;
	mAlpha = alpha;
	mUnknownE = 0;
}

SCharacterInfo CFont::GetCharacterInfo(uint32_t, bool) const {
	// Idk gotta impl this but this func is scary rn
	return {};
}

boost::shared_ptr<const SFontDefinition> CFont::GetDefinition() const {
	return mpDefinition;
}

float CFont::GetLineHeight() const {
	if (!mpDefinition) {
		return 0.0f;
	}

	auto scale = CTextureManager::GetSpriteScale(eTextureSize::Unknown);
	return scale * mpDefinition->mFontHeight * mFontSize;
}

bool CFont::HasCharacter(uint32_t) const {

}
