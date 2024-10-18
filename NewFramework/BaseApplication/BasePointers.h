#pragma once

// TODO: figure out field_88, field_B8, and field_C8 (if that's even possible)
class IBasePointers
{
public:
    class CTextureManager* textureManager{}; // 0x00
    class CTextureLoader* textureLoader{}; // 0x08
    class CCore* core{}; // 0x10
    class CApp* app{}; // 0x18
    class CFontManager* fontManager{}; // 0x20
    class CBaseFileIO* fileIO{}; // 0x28
    class CScreenManager* screenManager{}; // 0x30
    class CSoundManager* soundManager{}; // 0x38
    class CMusicManager* musicManager{}; // 0x40
    class CTransitions* transitions{}; // 0x48
    class CAgeGate* ageGate{}; // 0x50
    class CSteamInterface* steamInterface{}; // 0x58
    class CDebugXml* debugXml{}; // 0x60
    class CLicensing* licensing{}; // 0x68
    class CLegacyNetworkQueue* legacyNetworkQueue{}; // 0x70
    class CLegacyNetConnectionManager* legacyNetConnectionManager{}; // 0x78
    class CConnectionManager* connectionManager{}; // 0x80
    void* field_88{}; // 0x88
    class CNetworkMessageTranslator* networkMessageTranslator{}; // 0x90
    class CCurlHttpRequestManager* requestManager{}; // 0x98
    class CPrivateGameInterface* privateGameInterface{}; // 0xA0
    class CDebugStream* debugStream{}; // 0xA8
    class CLoc* loc{}; // 0xB0
    void* field_B8{}; // 0xB8
    class CEventManager* eventManager{}; // 0xC0
    void* field_C8{}; // 0xC8


    IBasePointers();
    void CopyFrom(const IBasePointers& other);
    void CopyTo(IBasePointers& other);
};
