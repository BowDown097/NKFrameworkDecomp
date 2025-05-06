#include "XmlFontImporter.h"

#include "NewFramework/Graphics/Management/TextureManager.h"
#include "NewFramework/Platform/Shared/Logging.h"




































CXmlFontImporter::CXmlFontImporter(CTextureManager* pTextureManager)
    : m_pTextureManager(pTextureManager) {}

const bool CXmlFontImporter::CanImport(const std::string& sPath) const {
    return sPath.rfind(".fnt") != std::string::npos;
}

const boost::shared_ptr<const SFontDefinition> CXmlFontImporter::Import(IFile* const pFile, const bool bLuminanceAlpha) {
    if (!pFile) {
        LOG_ERROR("Can't import font file, handle is NULL"); ENFORCE_LINE(50);
        return nullptr;
    }

    SFontDefinition definition;
    definition.bSDF = false;

    ticpp::Document doc;
    std::string sFileData;
    bool bReadFile = pFile->ReadString(sFileData);

    if (!bReadFile) {
        std::string sFilePath = pFile->GetFilePath();
        LOG_ERROR("Couldn't read contents of file '%s'", sFilePath.c_str()); ENFORCE_LINE(63);
        return nullptr;
    }

    doc.Parse(sFileData);

    ticpp::Element* pFontElement = doc.FirstChildElement("font");
    ticpp::Element* pInfoElement = pFontElement->FirstChildElement("info");
    ticpp::Element* pCommonElement = pFontElement->FirstChildElement("common");

    std::string sFace;
    float fScaleW = 1.0;
    float fScaleH = 1.0;

    pInfoElement->GetAttribute("face", &sFace);
    pInfoElement->GetAttribute("size", &definition.uSize);
    if (pInfoElement->HasAttribute("sdf")) {
        pInfoElement->GetAttribute("sdf", &definition.bSDF);
    }
    pCommonElement->GetAttribute("lineHeight", &definition.fLineHeight);
    pCommonElement->GetAttribute("scaleW", &fScaleW);
    pCommonElement->GetAttribute("scaleH", &fScaleH);

    ticpp::Element* pPagesElement = pFontElement->FirstChildElement("pages");
    std::vector<SPage> pages;

    for (ticpp::Element* pPageElement = pPagesElement->FirstChildElement("page", false);
         pPageElement != nullptr;
         pPageElement = pPageElement->NextSiblingElement("page", false)) {
        SPage page;

        pPageElement->GetAttribute("id", &page.id);
        pPageElement->GetAttribute("file", &page.sFile);

        eTextureType eType = CTexture::ParseTextureType(page.sFile.substr(page.sFile.rfind('.') + 1));
        std::string sFileName = page.sFile.substr(0, page.sFile.rfind('.'));

        page.pTexture = m_pTextureManager->AddTexture("Assets/Fonts/", sFileName, eType, false);
        if (bLuminanceAlpha) {
            page.pTexture->pixelFormat2 = ePixelFormat::LA;
        }

        page.fScaleW = fScaleW;
        page.fScaleH = fScaleH;

        pages.push_back(page);
        definition.textures.push_back(page.pTexture);
    }

    ticpp::Element* pCharsElement = pFontElement->FirstChildElement("chars");
    for (ticpp::Element* pCharElement = pCharsElement->FirstChildElement("char", false);
         pCharElement != nullptr; // TODO: not sure about the assignments beyond xadvance. will have to verify they're right
         pCharElement = pCharElement->NextSiblingElement("char", false)) {
        uint32_t id{};
        pCharElement->GetAttribute("id", &id);

        SCharacterInfo& characterInfo = definition.characterInfoMap[id];
        characterInfo.uCode = id;
        pCharElement->GetAttribute("x", &characterInfo.fX);
        pCharElement->GetAttribute("y", &characterInfo.fY);
        pCharElement->GetAttribute("width", &characterInfo.fWidth);
        pCharElement->GetAttribute("height", &characterInfo.fHeight);
        pCharElement->GetAttribute("xoffset", &characterInfo.fXOffset);
        pCharElement->GetAttribute("yoffset", &characterInfo.fYOffset);
        pCharElement->GetAttribute("xadvance", &characterInfo.fXAdvance);
        characterInfo.field_20 = characterInfo.fX / fScaleW;
        characterInfo.field_24 = characterInfo.fY / fScaleH;
        characterInfo.field_28 = characterInfo.fX / fScaleW;
        characterInfo.field_2C = (characterInfo.fHeight + characterInfo.fY) / fScaleH;
        characterInfo.field_30 = (characterInfo.fWidth + characterInfo.fX) / fScaleW;
        characterInfo.field_34 = characterInfo.fY / fScaleH;
        characterInfo.field_38 = (characterInfo.fWidth + characterInfo.fX) / fScaleW;
        characterInfo.field_3C = (characterInfo.fHeight + characterInfo.fY) / fScaleH;
    }

    ticpp::Element* pKerningsElement = pFontElement->FirstChildElement("kernings", false);
    if (pKerningsElement) {
        for (ticpp::Element* pKerningElement = pKerningsElement->FirstChildElement("kerning", false);
             pKerningElement != nullptr;
             pKerningElement = pKerningElement->NextSiblingElement("kerning", false)) {
            uint32_t first{};
            uint32_t second{};
            float amount{};

            pKerningElement->GetAttribute("first", &first);
            pKerningElement->GetAttribute("second", &second);
            pKerningElement->GetAttribute("amount", &amount);

            auto it = definition.MatchCharacter(second);
            if (it != definition.characterInfoMap.end() && second >= it->first) {
                it->second.kerningMap[first] = amount;
            }
        }
    }

    definition.characterInfoMap[160] = definition.characterInfoMap[32];
    definition.characterInfoMap[12288] = definition.characterInfoMap[32];

    return boost::make_shared<const SFontDefinition>(definition);
}
