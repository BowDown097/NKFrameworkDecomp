#pragma once

#include <NewFramework/Graphics/Font/FontDefinition.h>
#include <Uncategorized/RGBA.h>

#include <boost/smart_ptr/shared_ptr.hpp>

#include <map>
#include <string>
#include <vector>

class CBaseFileIO;
class CFont;
class CTextureManager;

class IFontImporter {};
struct SLocalisedFontInfo {
	std::string mFontName{};
	std::string mFontFile{};
	float mC = 0.0f;
	bool mD = false;
	bool mE = false;
	bool mF = false;
	CRGBA mColor{};
	float mAlpha = 0.0f;
};

class CFontManager {
public:
	CFontManager(CTextureManager* textureManager, CBaseFileIO* fileIO);
	~CFontManager() = default;

	const boost::shared_ptr<CFont> AddFont(const SLocalisedFontInfo& fontInfo);
	void AddFonts(const std::vector<SLocalisedFontInfo>& fontInfos);
	void AddImporter(boost::shared_ptr<IFontImporter> importer);
	int DecFontRefCount(const std::string& name, const std::string& source);
	void FontCleanup();
	boost::shared_ptr<CFont> GetFont(const std::string& name);
	std::string GetFontTextureName(boost::shared_ptr<CFont> font);
	std::string GetFontTextureName(const std::string& fontName);
	const boost::shared_ptr<const SFontDefinition> ImportFontDefinition(const std::string& fontName, const bool unknown);
	int IncFontRefCount(const std::string& name, const std::string& source);
	void PrintRefCountInfo() const;
	bool ResumeFont(const std::string& fontName);
	void SetCharacterFallbackEnabled(bool enabled);
	void SuspendFont(const std::string& fontName);
	void UpdateFont(const SLocalisedFontInfo& fontInfo);

	std::vector<boost::shared_ptr<IFontImporter>> mFontImporters{};
	std::map<std::string, boost::shared_ptr<const SFontDefinition>> mFontDefinitions{};
};