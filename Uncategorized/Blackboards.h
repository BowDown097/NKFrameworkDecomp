#pragma once
#include "BehaviourTree.h"
#include "NewFramework/Networking/NKAPI/Impl/NKMessages.h"
#include "NewFramework/Networking/NKAPI/NKEnums.h"
#include "NewFramework/Networking/NKAPI/NKError.h"
#include "NewFramework/Networking/Protocols/HTTP/HttpRequest.h"
#include <boost/smart_ptr/weak_ptr.hpp>

class C_NKLoginWebView;
enum class NKConflictResolutionType;
class NKFileClientImpl;
class NKSessionImpl;

enum class FileClientCallbackAction { Download, Upload, CheckConflicts }; // speculative name

struct SNKErrorBlackboard : BehaviourTree::IBlackboard
{
    NKError error;
};

struct SNKFileClientBlackboard : SNKErrorBlackboard
{
    NKFileClientImpl* fileClientImpl; // 0x60
    FileClientCallbackAction callbackAction{}; // 0x68
    std::string url; // 0x70
    std::string appIDStr; // 0x88
    std::string nkapiID; // 0xA0
    std::string fileName; // 0xB8
    std::string savePath; // 0xD0
    std::string postfix; // 0xE8
    clock_t start; // 0x100
    std::vector<char> data; // 0x108
    NKMessageResponseFile file; // 0x120
    NKConflictResolutionType conflictResolutionType; // 0x1A0
    eNKFileClientFilePermissions filePermissions; // 0x1A4
    SHttpTimeoutOptions timeoutOptions{}; // 0x1A8
    bool field_1C8{}; // 0x1C8
    bool stephen{}; // 0x1C9
    NKAccessToken accessToken; // 0x1D0
    int appID; // 0x1F0
    int skuID; // 0x1F4
    std::string privateKey; // 0x1F8
    eNKLinkServerCluster serverCluster; // 0x210
    boost::weak_ptr<BehaviourTree::Action> action; // 0x218
    bool field_228; // 0x228
    bool field_229; // 0x229
};

class NKSessionBlackboard : public SNKErrorBlackboard
{
public:
    NKSessionImpl* sessionImpl; // 0x60
    int appID; // 0x68
    int skuID; // 0x6C
    std::string privateKey; // 0x70
    eNKLinkServerCluster serverCluster; // 0x88
    std::vector<std::string> providers; // 0x90
    boost::shared_ptr<C_NKLoginWebView> webView; // 0xA8
    eNKServiceAction serviceAction; // 0xB8
    eNKLoginService loginService; // 0xBC
    bool field_C0 = true; // 0xC0
    NKAccessToken accessToken; // 0xC8
    NKResponseUser responseUser; // 0xE8
    bool newUser{}; // 0x1A8
    bool uiWasShown{}; // 0x1A9
    eNKLoginService loginServiceFromToken; // 0x1AC

    void LogMsg(std::string message) { messages.push_back(message); }
    void OutputFullLog() {}

    NKSessionBlackboard(NKSessionImpl* sessionImpl, const NKAccessToken& accessToken,
                        const NKResponseUser& responseUser, const int& appID,
                        const int& skuID, const std::string& privateKey,
                        eNKLinkServerCluster serverCluster, std::vector<std::string> providers)
        : sessionImpl(sessionImpl), appID(appID), skuID(skuID), privateKey(privateKey), serverCluster(serverCluster),
          providers(providers), accessToken(accessToken), responseUser(responseUser) {}
private:
    std::vector<std::string> messages; // 0x1B0
};

struct SNKManagerBlackboard : SNKErrorBlackboard
{
    std::string field_60; // 0x60
    bool field_78; // 0x78
    std::string field_80; // 0x80
    std::string field_98; // 0x98
};
