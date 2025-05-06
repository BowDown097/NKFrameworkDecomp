#pragma once

#include "FontImporter.h"
#include "NewFramework/Graphics/Texture.h"

class CBaseFileIO;
class CTextureManager;

struct SPage {
    int id; // 0x00
    std::string sFile; // 0x08
    CTexture* pTexture; // 0x20
    float fScaleW{}; // 0x28
    float fScaleH{}; // 0x2C
};

class CXmlFontImporter : public IFontImporter {
public:
    explicit CXmlFontImporter(CTextureManager* pTextureManager);
    const bool CanImport(const std::string& sPath) const override;
    const boost::shared_ptr<const SFontDefinition> Import(IFile* const pFile, const bool bLuminanceAlpha) override;
private:
    CTextureManager* m_pTextureManager; // 0x08
};
