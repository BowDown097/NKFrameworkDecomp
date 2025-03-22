#pragma once

#include <NewFramework/Graphics/Font/CharacterInfo.h>
#include <NewFramework/Graphics/Font/FontDefinition.h>
#include <Uncategorized/ReferenceCounted.h>
#include <Uncategorized/RGBA.h>

#include <boost/smart_ptr/shared_ptr.hpp>

#include <string>

class CFont : public CReferenceCounted {
public:
	CFont() = default;
	CFont(const std::string&, float, const boost::shared_ptr<const SFontDefinition>&, bool, const CRGBA&, float);
	[[nodiscard]] SCharacterInfo GetCharacterInfo(uint32_t, bool) const;
	[[nodiscard]] boost::shared_ptr<const SFontDefinition> GetDefinition() const;
	[[nodiscard]] float GetLineHeight() const;
	[[nodiscard]] bool HasCharacter(uint32_t) const;

	boost::shared_ptr<const SFontDefinition> mpDefinition = nullptr;
	float mFontSize = 0.0f; // Unsure
	std::string mName{};
	bool mUnknownC = false;
	CRGBA mColor{};
	float mAlpha = 0.0f; // Probably but still unsure
	int mUnknownE = 0;
};
