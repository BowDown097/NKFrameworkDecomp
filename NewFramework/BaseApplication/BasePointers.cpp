#include "BasePointers.h"

void IBasePointers::CopyFrom(const IBasePointers& other) {
    pTextureManager = other.pTextureManager;
    pTextureLoader = other.pTextureLoader;
    pInput = other.pInput;
    pApp = other.pApp;
    pFontManager = other.pFontManager;
    pFileIO = other.pFileIO;
    pScreenManager = other.pScreenManager;
    pSoundManager = other.pSoundManager;
    pMusicManager = other.pMusicManager;
    pTransitions = other.pTransitions;
    pStoreInterface = other.pStoreInterface;
    pSteamInterface = other.pSteamInterface;
    pDebugXml = other.pDebugXml;
    pLicensing = other.pLicensing;
    pNetworkQueue = other.pNetworkQueue;
    pLegacyConnectionManager = other.pLegacyConnectionManager;
    pConnectionManager = other.pConnectionManager;
    pNetworkMessageTranslator = other.pNetworkMessageTranslator;
    pHttpRequestManager = other.pHttpRequestManager;
    field_A0 = other.field_A0;
    pLoc = other.pLoc;
    field_B8 = other.field_B8;
    pEventManager = other.pEventManager;
    field_C8 = other.field_C8;
    field_D0 = other.field_D0;
}

void IBasePointers::CopyTo(IBasePointers& other) {
    other.CopyFrom(*this);
}
