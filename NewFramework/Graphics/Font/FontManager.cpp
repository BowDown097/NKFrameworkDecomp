#include "FontManager.h"

#include "NewFramework/Graphics/Management/TextureManager.h"
#include "NewFramework/Graphics/Render/RenderSystem.h"
#include "NewFramework/IO/File/BaseFileIO.h"
#include "NewFramework/Platform/Shared/Logging.h"
#include "NewFramework/Utilities/StringHelper.h"
#include "XmlFontImporter.h"

#include <boost/unordered/unordered_set.hpp>

IFile* FindFontFile(CBaseFileIO* pFileIO, eTextureSize eMaxSize, const std::string& sFolder, const std::string& sSource) {
    for (int i = static_cast<int>(eMaxSize); i > 0; --i) {
        std::string sTextureSize = CTexture::TextureSizeToString(static_cast<eTextureSize>(i));
        std::string sFontPath = sTextureSize + sFolder + "/" + sSource + ".fnt";

        if (pFileIO->FileExists(sFontPath, pFileIO->assetPolicy)) {
            return pFileIO->OpenFile(sFontPath, pFileIO->assetPolicy, eFileOpenMode::Read);
        }
    }

    return nullptr;
}

CFontManager::CFontManager(CTextureManager* pTextureManager, CBaseFileIO* pFileIO)
    : m_pTextureManager(pTextureManager),
      m_pFileIO(pFileIO) {

    boost::shared_ptr<CXmlFontImporter> pFontImporter = boost::make_shared<CXmlFontImporter>(m_pTextureManager);
    AddImporter(pFontImporter);

    CRenderSystem::AddShader("sdf_font");
    CRenderSystem::AddShader("sdf_font_outline");
}

int CFontManager::DecFontRefCount(const std::string& sName, const std::string&) {
    boost::shared_ptr<CFont> pFont = GetFont(sName);
    int count;

    if (pFont) {
        int mRefCount = pFont->mRefCount;

        count = mRefCount - 1;
        if (mRefCount <= 0) {
            count = 0;
        }
        if (mRefCount == INT_MAX) {
            count = INT_MAX;
        }

        pFont->mRefCount = count;

        if (count == 0) {
            FontCleanup();
            count = 0;
        }
    } else {
        count = -1;
    }

    return count;
}

int CFontManager::IncFontRefCount(const std::string& sName, const std::string&) {
    boost::shared_ptr<CFont> pFont = GetFont(sName);
    int count;

    if (pFont) {
        int mRefCount = pFont->mRefCount;

        count = 1;
        if (mRefCount != INT_MAX) {
            count = mRefCount + 1;
        }

        pFont->mRefCount = count;

        if (count == 1) {
            CRenderSystem::LoadTexture(GetFontTextureName(pFont));
            count = 1;
        }
    } else {
        count = -1;
    }

    return count;
}

void CFontManager::PrintRefCountInfo() const {
    std::vector<std::string> fontInfos;

    for (auto it = m_fontMap.begin(); it != m_fontMap.end(); ++it) {
        boost::shared_ptr<CFont> pFont = it->second;
        if (pFont && pFont->mRefCount < INT_MAX) {
            std::string sFontInfo = pFont->m_sName;
            if (sFontInfo.size() <= 48) {
                sFontInfo.append(" ", 49 - sFontInfo.size());
            }

            std::string sRefCount = pFont->mRefCount == INT_MAX
                ? "Not Ref Counted ?" : StringHelper::Format("%d", pFont->mRefCount);
            sFontInfo += "RefCount: " + sRefCount;

            fontInfos.push_back(sFontInfo);
        }
    }

    std::sort(fontInfos.begin(), fontInfos.end(), &StringHelper::CompareNoCase);
    std::string sLoadedFonts = "Loaded Fonts:\n{\n";

    for (const std::string& sFontInfo : fontInfos) {
        sLoadedFonts += "\t" + sFontInfo + "\n";
    }

#ifndef NDEBUG // speculative
    std::cout << sLoadedFonts << std::endl;
#endif
}

void CFontManager::SetCharacterFallbackEnabled(bool bEnabled) {
    m_bCharacterFallbackEnabled = bEnabled;
    uint uFallbackCharacter = bEnabled ? CFont::s_uDefaultFallbackCharacter : 0;

    for (auto it = m_fontMap.begin(); it != m_fontMap.end(); ++it) {
        const boost::shared_ptr<CFont>& pFont = it->second;
        if (pFont) {
            pFont->m_uFallbackCharacter = uFallbackCharacter;
        }
    }
}

boost::shared_ptr<CFont> CFontManager::GetFont(const std::string& sName) {
    auto it = m_fontMap.find(sName);
    if (it != m_fontMap.end()) {
        return it->second;
    } else {
        LOG_ERROR("Couldn't find font '%s' in map", sName.c_str()); ENFORCE_LINE(137);
        return nullptr;
    }
}

bool CFontManager::SuspendFont(const std::string& sName) {
    boost::shared_ptr<CFont> pFont = GetFont(sName);
    if (!pFont) {
        return false;
    }

    std::string sTextureName = GetFontTextureName(pFont);
    if (!m_pTextureManager->IsTextureLoaded(sTextureName)) {
        return false;
    }

    CRenderSystem::UnloadTexture(sTextureName);
    return true;
}

bool CFontManager::ResumeFont(const std::string& sName) {
    boost::shared_ptr<CFont> pFont = GetFont(sName);
    if (!pFont) {
        return false;
    }

    std::string sTextureName = GetFontTextureName(pFont);
    if (m_pTextureManager->IsTextureLoaded(sTextureName) || m_pTextureManager->GetRefCount(sTextureName) <= 0) {
        return false;
    }

    CRenderSystem::LoadTexture(sTextureName);
    return true;
}













































































































































const boost::shared_ptr<CFont> CFontManager::AddFont(const SLocalisedFontInfo& fontInfo) {
    boost::shared_ptr<const SFontDefinition> pFontDefinition = ImportFontDefinition(fontInfo.sSource, fontInfo.bLuminanceAlpha);
    if (pFontDefinition) {
        boost::shared_ptr<CFont> pFont = boost::make_shared<CFont>(
            fontInfo.sName, fontInfo.fSize, pFontDefinition, fontInfo.field_36, fontInfo.colour, fontInfo.fAlpha);
        pFont->m_uFallbackCharacter = m_bCharacterFallbackEnabled ? CFont::s_uDefaultFallbackCharacter : 0;
        m_fontMap[pFont->m_sName] = pFont;
        return pFont;
    } else {
        LOG_ERROR("Failed to import font definition '%s' for font '%s'", fontInfo.sSource.c_str(), fontInfo.sName.c_str()); ENFORCE_LINE(321);
        return nullptr;
    }
}

void CFontManager::AddFonts(const std::vector<SLocalisedFontInfo>& fontInfos) {
    if (fontInfos.empty()) {
        return;
    }

    for (const SLocalisedFontInfo& fontInfo : fontInfos) {
        auto it = m_fontDefinitionMap.find(fontInfo.sSource);
        if (it != m_fontDefinitionMap.end()) {
            m_fontDefinitionMap.erase(it);
        }
    }

    for (const SLocalisedFontInfo& fontInfo : fontInfos) {
        auto it = m_fontMap.find(fontInfo.sName);
        if (it == m_fontMap.end()) {
            AddFont(fontInfo);
        } else {
            UpdateFont(fontInfo);
        }
    }
}

void CFontManager::FontCleanup() {
    boost::unordered_set<std::string> fontTextures;
    boost::unordered_set<std::string> loadedFontTextures;

    if (!m_fontMap.empty()) {
        for (auto it = m_fontMap.begin(); it != m_fontMap.end(); ++it) {
            const boost::shared_ptr<CFont>& pFont = it->second;
            if (pFont) {
                std::string sTextureName = GetFontTextureName(pFont);
                fontTextures.insert(sTextureName);
                if (pFont->mRefCount < INT_MAX) {
                    loadedFontTextures.insert(sTextureName);
                }
            }
        }

        for (const std::string& sTexture : fontTextures) {
            if (!loadedFontTextures.contains(sTexture)) {
                CRenderSystem::UnloadTexture(sTexture);
            }
        }
    }
}

std::string CFontManager::GetFontTextureName(boost::shared_ptr<CFont> pFont) {
    return pFont ? pFont->GetDefinition()->textures.front()->id : "";
}

std::string CFontManager::GetFontTextureName(const std::string& sFontName) {
    return GetFontTextureName(GetFont(sFontName));
}

void CFontManager::AddImporter(boost::shared_ptr<IFontImporter> pImporter) {
    auto it = std::find(m_fontImporters.begin(), m_fontImporters.end(), pImporter);
    if (it == m_fontImporters.end()) {
        m_fontImporters.push_back(pImporter);
    }
}

const boost::shared_ptr<const SFontDefinition> CFontManager::ImportFontDefinition(
    const std::string& sSource, bool bLuminanceAlpha) {

    auto it = m_fontDefinitionMap.find(sSource);
    if (it != m_fontDefinitionMap.end() && it->second != nullptr) {
        return it->second;
    }

    IFile* pFontFile = FindFontFile(m_pFileIO, CTextureManager::GetTextureSize(), "Assets/Fonts/", sSource);
    if (!pFontFile) {
        LOG_ERROR("Unable to find font file %s", sSource.c_str()); ENFORCE_LINE(397);
        return nullptr;
    }

    boost::shared_ptr<const SFontDefinition> pFontDefinition;

    for (auto it = m_fontImporters.begin(); it != m_fontImporters.end() && !pFontDefinition; ++it) {
        const boost::shared_ptr<IFontImporter>& pFontImporter = *it;

        if (pFontImporter->CanImport(pFontFile->GetFilePath())) {
            pFontDefinition = pFontImporter->Import(pFontFile, bLuminanceAlpha);
        } else {
            std::string sFilePath = pFontFile->GetFilePath();
            LOG_ERROR("Could not import file at %s.", sFilePath.c_str()); ENFORCE_LINE(410);
        }
    }

    delete pFontFile;
    if (pFontDefinition) {
        m_fontDefinitionMap[sSource] = pFontDefinition;
    } else {
        LOG_ERROR("No font definition for %s", sSource.c_str()); ENFORCE_LINE(418);
    }

    return pFontDefinition;
}






const boost::shared_ptr<CFont> CFontManager::UpdateFont(const SLocalisedFontInfo& fontInfo) {
    boost::shared_ptr<CFont> pFont = GetFont(fontInfo.sName);
    boost::shared_ptr<const SFontDefinition> pFontDefinition = ImportFontDefinition(fontInfo.sSource, fontInfo.bLuminanceAlpha);

    if (!pFontDefinition) {
        LOG_ERROR("Failed to import font definition '%s' for font '%s'", fontInfo.sSource.c_str(), fontInfo.sName.c_str()); ENFORCE_LINE(434);
        return nullptr;
    }

    if (pFont) {
        pFont->m_pDefinition = pFontDefinition;
        pFont->m_fSize = fontInfo.fSize;
        pFont->field_40 = fontInfo.field_36;
        pFont->m_colour = fontInfo.colour;
        pFont->m_fAlpha = fontInfo.fAlpha;
    } else {
        pFont = boost::make_shared<CFont>(
            fontInfo.sName, fontInfo.fSize, pFontDefinition, fontInfo.field_36, fontInfo.colour, fontInfo.fAlpha);
        pFont->m_uFallbackCharacter = m_bCharacterFallbackEnabled ? CFont::s_uDefaultFallbackCharacter : 0;
        m_fontMap[pFont->m_sName] = pFont;
    }

    return pFont;
}
