#pragma once

// TODO: figure out field_0, field_90, field_C0, and field_D0 (if that's even possible)
class IBasePointers
{
public:
    void* field_0{};
    class CTextureManager* textureManager{}; // 0x08
    class CTextureLoader* textureLoader{}; // 0x10
    class CCore* core{}; // 0x18
    class CApp* app{}; // 0x20
    class CFontManager* fontManager{}; // 0x28
    class CBaseFileIO* fileIO{}; // 0x30
    class CScreenManager* screenManager{}; // 0x38
    class CSoundManager* soundManager{}; // 0x40
    class CMusicManager* musicManager{}; // 0x48
    class CTransitions* transitions{}; // 0x50
    class CAgeGate* ageGate{}; // 0x58
    class CSteamInterface* steamInterface{}; // 0x60
    class CDebugXml* debugXml{}; // 0x68
    class CLicensing* licensing{}; // 0x70
    class CLegacyNetworkQueue* legacyNetworkQueue{}; // 0x78
    class CLegacyNetConnectionManager* legacyNetConnectionManager{}; // 0x80
    class CConnectionManager* connectionManager{}; // 0x88
    void* field_90{}; // 0x90
    class CNetworkMessageTranslator* networkMessageTranslator{}; // 0x98
    class CCurlHttpRequestManager* requestManager{}; // 0xA0
    class CPrivateGameInterface* privateGameInterface{}; // 0xA8
    class CDebugStream* debugStream{}; // 0xB0
    class CLoc* loc{}; // 0xB8
    void* field_C0{}; // 0xC0
    class CEventManager* eventManager{}; // 0xC8
    void* field_D0{}; // 0xD0


    IBasePointers();
    void CopyFrom(const IBasePointers& other);
    void CopyTo(IBasePointers& other);
};
