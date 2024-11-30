#pragma once

class NKModuleBank;
class NKModuleIAPVerification;
class NKModuleLeaderboard;
class NKModuleMatchMaker;
class NKModulePubSub;
class NKModuleUser;
class NKModuleUtility;

// TODO: i'm guessing field_10, field_38, field_48, and field_50 are exclusive to other platforms.
// getting these will be fun i'm sure.
class NKModules {
public:
    NKModuleBank* Bank();
    NKModuleMatchMaker* Matchmaker();
    NKModulePubSub* PubSub();
    NKModuleLeaderboard* Leaderboard();
    NKModuleUtility* Utilities();
    NKModuleIAPVerification* IAPVerification();
    NKModuleUser* User();
private:
    NKModulePubSub* pubSub{}; // 0x00
    NKModuleUtility* utility{}; // 0x08
    void* field_10{}; // 0x10
    NKModuleUser* user{}; // 0x18
    NKModuleMatchMaker* matchmaker{}; // 0x20
    NKModuleLeaderboard* leaderboard{}; // 0x28
    NKModuleBank* bank{}; // 0x30
    void* field_38{}; // 0x38
    NKModuleIAPVerification* iapVerification{}; // 0x40
    void* field_48{}; // 0x48
    void* field_50{}; // 0x50
};
