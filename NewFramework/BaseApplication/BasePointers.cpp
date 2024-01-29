#include "BasePointers.h"

void IBasePointers::CopyFrom(const IBasePointers& other)
{
    textureManager = other.textureManager;
    textureLoader = other.textureLoader;
    core = other.core;
    app = other.app;
    fontManager = other.fontManager;
    fileIO = other.fileIO;
    screenManager = other.screenManager;
    soundManager = other.soundManager;
    musicManager = other.musicManager;
    transitions = other.transitions;
    ageGate = other.ageGate;
    steamInterface = other.steamInterface;
    debugXml = other.debugXml;
    licensing = other.licensing;
    legacyNetworkQueue = other.legacyNetworkQueue;
    legacyNetConnectionManager = other.legacyNetConnectionManager;
    connectionManager = other.connectionManager;
    field_88 = other.field_88;
    networkMessageTranslator = other.networkMessageTranslator;
    requestManager = other.requestManager;
    privateGameInterface = other.privateGameInterface;
    debugStream = other.debugStream;
    loc = other.loc;
    field_B8 = other.field_B8;
    eventManager = other.eventManager;
    field_C8 = other.field_C8;
}

void IBasePointers::CopyTo(IBasePointers& other)
{
    other.textureManager = textureManager;
    other.textureLoader = textureLoader;
    other.core = core;
    other.app = app;
    other.fontManager = fontManager;
    other.fileIO = fileIO;
    other.screenManager = screenManager;
    other.soundManager = soundManager;
    other.musicManager = musicManager;
    other.transitions = transitions;
    other.ageGate = ageGate;
    other.steamInterface = steamInterface;
    other.debugXml = debugXml;
    other.licensing = licensing;
    other.legacyNetworkQueue = legacyNetworkQueue;
    other.legacyNetConnectionManager = legacyNetConnectionManager;
    other.connectionManager = connectionManager;
    other.field_88 = field_88;
    other.networkMessageTranslator = networkMessageTranslator;
    other.requestManager = requestManager;
    other.privateGameInterface = privateGameInterface;
    other.debugStream = debugStream;
    other.loc = loc;
    other.field_B8 = field_B8;
    other.eventManager = eventManager;
    other.field_C8 = field_C8;
}
