#pragma once
#include <json_spirit/json_spirit_value.h>

class CBaseFileIO;
class CTextureManager;
enum class eTextureSize;
struct NKError;
class NKFileClient;
class NKManager;

class NKCrossPromo {
public:
    struct SPromotion {
        int appID; // 0x00
        std::string name; // 0x08
        std::string url; // 0x20
        std::vector<std::string> tags; // 0x38
    };

    struct SPendingPromotion {
        int id; // 0x00
        int appID; // 0x04
        std::string assetUrl; // 0x08
        std::string url; // 0x20
        eTextureSize textureSize; // 0x38
        std::vector<std::string> tags; // 0x40
    };

    NKCrossPromo(NKManager* manager, NKFileClient* fileClient, CBaseFileIO* fileIO,
                 CTextureManager* textureManager, const std::string& platformID)
        : manager(manager), fileClient(fileClient), fileIO(fileIO), textureManager(textureManager), platformID(platformID) {}
    void Download(const std::string& url);
    void OnPromoDownload(
        bool success, const std::string& savePath, const std::string& filename, const NKError* error);
    SPromotion* GetPromotion(uint32_t index);
    SPromotion* GetPromotionWithTag(std::string tag);
    void OnAppPromoDownload(
        bool success, const std::string& savePath, const std::string& fileName, const NKError* error, int appID);
    uint32_t ParseAppPromoDownload(const json_spirit::mObject& obj, int appID);
    void TryStartAppPromoDownload();
    void OnImageDownload(
        bool success, const std::string& savePath, const std::string& fileName, const NKError* error, int id);
    void ParsePromoDownload(const json_spirit::mObject& obj);
private:
    bool downloadStarted{}; // 0x00
    bool appPromoFailed{}; // 0x01
    NKManager* manager; // 0x08
    NKFileClient* fileClient; // 0x10
    CBaseFileIO* fileIO; // 0x18
    CTextureManager* textureManager; // 0x20
    int appPromoAttempts{}; // 0x28
    int appPromoCompletions{}; // 0x2C
    int pendingPromoID; // 0x30
    std::string platformID; // 0x38
    std::vector<int> appIDs; // 0x50
    std::vector<SPendingPromotion> pendingPromotions; // 0x68
    std::vector<SPromotion> promotions; // 0x80
};
