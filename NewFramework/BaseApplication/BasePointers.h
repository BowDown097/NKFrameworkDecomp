#pragma once

#include <memory>

struct IBasePointers {
    class CTextureManager* pTextureManager{}; // 0x00
    class CTextureLoader* pTextureLoader{}; // 0x08
    class CInput* pInput{}; // 0x10
    class CApp* pApp{}; // 0x18
    class CFontManager* pFontManager{}; // 0x20
    class CBaseFileIO* pFileIO{}; // 0x28
    class CScreenManager* pScreenManager{}; // 0x30
    class CSoundManager* pSoundManager{}; // 0x38
    class CMusicManager* pMusicManager{}; // 0x40
    class CTransitions* pTransitions{}; // 0x48
    class CStoreInterface* pStoreInterface{}; // 0x50
    class CSteamInterface* pSteamInterface{}; // 0x58
    class CDebugXml* pDebugXml{}; // 0x60
    class CLicensing* pLicensing{}; // 0x68
    class CLegacyNetworkQueue* pNetworkQueue{}; // 0x70
    class CLegacyNetConnectionManager* pLegacyConnectionManager{}; // 0x78
    std::shared_ptr<class CConnectionManager> pConnectionManager{}; // 0x80
    class CNetworkMessageTranslator* pNetworkMessageTranslator{}; // 0x90
    class IHttpRequestManager* pHttpRequestManager{}; // 0x98
    void* field_A0{}; // 0xA0
    class CDebugStream* pDebugStream{}; // 0xA8
    class CLoc* pLoc{}; // 0xB0
    void* field_B8{}; // 0xB8
    class CEventManager* pEventManager{}; // 0xC0
    void* field_C8{}; // 0xC8
    void* field_D0{}; // 0xD0

    void CopyFrom(const IBasePointers& other);
    void CopyTo(IBasePointers& other);
};
