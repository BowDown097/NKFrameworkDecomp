#pragma once

#include "Font.h"

class CBaseFileIO;
class CTextureManager;
struct IFontImporter;

struct SLocalisedFontInfo {
    std::string sName;
    std::string sSource;
    float fSize{};
    bool bLuminanceAlpha{};
    bool field_35{};
    bool field_36{};
    CRGBA colour;
    float fAlpha{};
};

class CFontManager {
public:
    CFontManager(CTextureManager* pTextureManager, CBaseFileIO* pFileIO);
    void AddImporter(boost::shared_ptr<IFontImporter> pImporter);
    boost::shared_ptr<CFont> GetFont(const std::string& sName);
    void AddFonts(const std::vector<SLocalisedFontInfo>& fontInfos);
    const boost::shared_ptr<CFont> AddFont(const SLocalisedFontInfo& fontInfo);
    const boost::shared_ptr<CFont> UpdateFont(const SLocalisedFontInfo& fontInfo);
    int DecFontRefCount(const std::string& sName, const std::string& sSource);
    void FontCleanup();
    int IncFontRefCount(const std::string& sName, const std::string& sSource);
    std::string GetFontTextureName(boost::shared_ptr<CFont> pFont);
    bool SuspendFont(const std::string& sName);
    bool ResumeFont(const std::string& sName);
    void PrintRefCountInfo() const;
    std::string GetFontTextureName(const std::string& sFontName);
    const boost::shared_ptr<const SFontDefinition> ImportFontDefinition(const std::string& sSource, bool bLuminanceAlpha);
    void SetCharacterFallbackEnabled(bool bEnabled);
private:
    std::vector<boost::shared_ptr<IFontImporter>> m_fontImporters; // 0x00
    std::map<std::string, boost::shared_ptr<const SFontDefinition>> m_fontDefinitionMap; // 0x18
    std::map<std::string, boost::shared_ptr<CFont>> m_fontMap; // 0x30
    CTextureManager* m_pTextureManager; // 0x48
    CBaseFileIO* m_pFileIO; // 0x50
    bool m_bCharacterFallbackEnabled{}; // 0x58
};
