#ifndef BASEPOINTERS_H
#define BASEPOINTERS_H

// TODO: figure out field_88, field_B8, and field_C8 (if that's even possible)
class IBasePointers
{
public:
    class CTextureManager* textureManager = nullptr; // 0x00
    class CTextureLoader* textureLoader = nullptr; // 0x08
    class CCore* core = nullptr; // 0x10
    class CApp* app = nullptr; // 0x18
    class CFontManager* fontManager = nullptr; // 0x20
    class CBaseFileIO* fileIO = nullptr; // 0x28
    class CScreenManager* screenManager = nullptr; // 0x30
    class CSoundManager* soundManager = nullptr; // 0x38
    class CMusicManager* musicManager = nullptr; // 0x40
    class CTransitions* transitions = nullptr; // 0x48
    class CAgeGate* ageGate = nullptr; // 0x50
    class CSteamInterface* steamInterface = nullptr; // 0x58
    class CDebugXml* debugXml = nullptr; // 0x60
    class CLicensing* licensing = nullptr; // 0x68
    class CLegacyNetworkQueue* legacyNetworkQueue = nullptr; // 0x70
    class CLegacyNetConnectionManager* legacyNetConnectionManager = nullptr; // 0x78
    class CConnectionManager* connectionManager = nullptr; // 0x80
    void* field_88 = nullptr; // 0x88
    class CNetworkMessageTranslator* networkMessageTranslator = nullptr; // 0x90
    class CCurlHttpRequestManager* requestManager = nullptr; // 0x98
    class CPrivateGameInterface* privateGameInterface = nullptr; // 0xA0
    class CDebugStream* debugStream = nullptr; // 0xA8
    class CLoc* loc = nullptr; // 0xB0
    void* field_B8 = nullptr; // 0xB8
    class CEventManager* eventManager = nullptr; // 0xC0
    void* field_C8 = nullptr; // 0xC8


    IBasePointers();
    void CopyFrom(const IBasePointers& other);
    void CopyTo(IBasePointers& other);
};

#endif // BASEPOINTERS_H
