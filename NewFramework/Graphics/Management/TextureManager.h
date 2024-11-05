#pragma once
#include "TextureLoader.h"
#include "Uncategorized/SpriteInformation.h"
#include <boost/thread/recursive_mutex.hpp>
#include <boost/unordered_map.hpp>
#include <ticpp.h>

// might not be the right struct but there def is one with these members
struct SAnimationFrame
{
    int flipx;
    int flipy;
    int texw;
    int texh;
};

struct SAnimationInfo;
struct SCompoundSprite;

class CTextureManager
{
public:
    struct STextureInfo;
    enum class eTextureLoadMethod;

    using tMapStringAnimationInfo = boost::unordered_map<std::string, SAnimationInfo*>;
    using tMapStringCompoundSprite = boost::unordered_map<std::string, SCompoundSprite*>;
    using tMapStringSpriteInfo = boost::unordered_map<std::string, SSpriteInfo*>;
    using tMapStringTextureInfo = boost::unordered_map<std::string, STextureInfo*>;

    CTextureLoader* textureLoader; // 0x00
    void* field_8{}; // 0x08
    tMapStringTextureInfo textureInfoMap; // 0x10
    tMapStringSpriteInfo spriteInfoMap; // 0x40
    tMapStringAnimationInfo animationInfoMap; // 0x70
    tMapStringCompoundSprite compoundSpriteMap; // 0xA0
    tMapStringTextureInfo textureInfoMap2; // 0xD0
    SAnimationFrame currentFrame{}; // 0x100
    void* field_110{}; // 0x110
    CBaseFileIO* fileIO; // 0x118
    boost::recursive_mutex mutex1; // 0x120
    boost::recursive_mutex mutex2; // 0x160
    bool field_1A0{}; // 0x1A0

    CTextureManager(CTextureLoader* textureLoader, CBaseFileIO* fileIO);
    void Update(const double&);
    void PrepareTextures(const std::string& texturesPath, ticpp::Element* spriteInfo);
    CTexture* AddTexture(const std::string& texturesPath, const std::string& name,
                         const eTextureType& type, bool preMultiplyAlpha);
    void PrepareCells(const std::string& texturesPath, ticpp::Element* frameInfo, eTextureSize size);
    SSpriteInfo* CreateSpriteInfoFromElement(ticpp::Element*, eTextureSize);
    void AddSpriteInfoStruct(const std::string&, const std::string&, SSpriteInfo*);
    SAnimationInfo* CreateAnimationFromElement(ticpp::Element*, bool, CTexture*);
    void AddAnimationStruct(const std::string&, const std::string&, SAnimationInfo*);
    void PrepareSpriteInfo(const std::string&, ticpp::Element*);
    STextureInfo* GetTextureInfoPtr(const std::string&);
    CTexture* GetTextureHandlePtr(const std::string&);
    CTexture* AddTextureFromFile(const std::string&, const std::string&, const CFilePolicy&,
                                 eTextureType, eTextureSize, bool);
    CTexture* AddTexture(const std::string&, const std::string&, const eTextureType&, bool, CFilePolicy&);
    CTexture* AddTexture(const std::string&, const std::string&, const std::string&, const eTextureType&,
                         bool, CFilePolicy&);
    eTextureSize FindTexture(const std::string&, const std::string&, eTextureType, CFilePolicy&);
    CTexture* AddTexture(const std::string&, const unsigned int&, const unsigned int&);
    CTexture* AddTexture(CTexture *const);
    void RemoveTexture(CTexture*);
    bool IsTextureLoaded(const std::string&);
    unsigned int GetRefCount(const std::string&);
    int GetLookups();
    void ResetLookups();
    void LoadTexture(const std::string&, eTextureLoadMethod);
    void UnloadTexture(const std::string&);
    void ReloadTextures();
    int IncTextureRefCount(const std::string&, const std::string&);
    int DecTextureRefCount(const std::string&, const std::string&);
    bool SuspendTexture(const std::string&);
    bool ResumeTexture(const std::string&);
    bool GetTextureAvailable(const std::string&);
    std::string GetTextureStatus();
    std::string GetLoadedTextures(bool, bool);
    unsigned int GetLoadedTextureSize();
    static float GetSpriteScale(eTextureSize);
    SAnimationFrame* CreateAnimFrameFromElement(ticpp::Element*, eTextureSize);
    void AddCompoundStruct(const std::string&, SCompoundSprite*, bool);
    SSpriteInfo* GetSpriteInfoPtr(const std::string&, const std::string&);
    bool GetSpriteInfoExists(const std::string&, const std::string&);
    std::string GetSpriteInfoExistsPartialMatch(const std::string&, const std::string&);
    SAnimationInfo* GetAnimationPtr(const std::string&, const std::string&);
    SCompoundSprite* GetCompoundPtr(const std::string&, bool);
    tMapStringSpriteInfo* GetTextureSprites(const std::string&);
    bool HasTextureSprites(const std::string&, tMapStringSpriteInfo&);
    SAnimationInfo* GetAnimationPtr(const std::string&);
    eTextureSize GetTextureSize();
    tMapStringSpriteInfo GetSpritesInTexture(const std::string&);
    eTextureSize FindTexture(const std::string&, const std::string&, eTextureType);
    eTextureSize FindTextureXML(const std::string&, const std::string&, CFilePolicy&);
    void LoadCompoundFile(const std::string&, const std::string&);
    void LoadCompound(const std::string&, const std::string&, bool);
    bool LoadTextureXMLFile(const std::string&, bool);
    bool LoadTextureXMLFile(const std::string&, CFilePolicy&, bool);
    void ParseSpriteXmlDoc(std::string&, ticpp::Document&, bool);
    void StartDrawing();
    void EndDrawing();
    void PrintUntouchedAssets(bool);
    void PrintRefCountInfo();
    void PrintSpriteInfoTotalBytes();
};
