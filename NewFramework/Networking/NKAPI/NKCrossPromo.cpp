#include "NKCrossPromo.h"

#include "NewFramework/Graphics/Management/TextureManager.h"
#include "NewFramework/Networking/NKAPI/NKManager.h"
#include "NewFramework/Networking/NKAPI/NKSession.h"
#include "NewFramework/Platform/Shared/Logging.h"
#include "NewFramework/Utilities/JSON/JSONWrapper.h"
#include "NewFramework/Utilities/JSON/JSON_PropertyReader/JSON_Read.h"
#include "NewFramework/Utilities/StringHelper.h"
#include "NKFileClient.h"

#include <boost/bind/bind.hpp>
using namespace boost::placeholders;

namespace {
void replace_res_tags(std::string& url, const char* replacement, eTextureSize textureSize) {
    size_t startPos = 0;
    while ((startPos = url.find('{', startPos)) != std::string::npos) {
        std::string textureSizeUpper = StringHelper::ToUpper(CTexture::TextureSizeToString(textureSize));
        url.replace(startPos, 7, textureSizeUpper);
    }
}
}


NKCrossPromo::SPromotion* NKCrossPromo::GetPromotion(uint32_t index) {
    return index < promotions.size() ? &promotions[index] : nullptr;
}

NKCrossPromo::SPromotion* NKCrossPromo::GetPromotionWithTag(std::string tag) {
    auto it = std::find_if(promotions.begin(), promotions.end(), [&tag](const NKCrossPromo::SPromotion& promo) {
        return std::find(promo.tags.begin(), promo.tags.end(), tag) != promo.tags.end();
    });

    return it != promotions.end() ? &*it : nullptr;
}

void NKCrossPromo::OnPromoDownload(
    bool success, const std::string& savePath, const std::string& filename, const NKError* error) {

    appPromoAttempts = 0;
    appPromoCompletions = 0;

    if (error || !success) {
        downloadStarted = !pendingPromotions.empty();
        return;
    }

    CJSONWrapper jsonWrapper(fileIO);
    ParsePromoDownload(jsonWrapper.ReadObject(savePath + "/" + filename, fileIO->cachePolicy));

    if (appPromoAttempts == 0) {
        appPromoFailed = true;
    }

    downloadStarted = appPromoAttempts > appPromoCompletions || !pendingPromotions.empty();
}

void NKCrossPromo::Download(const std::string& url) {
    downloadStarted = fileClient->Download(url, boost::bind(&NKCrossPromo::OnPromoDownload, this, _1, _2, _3, _4));

    if (!downloadStarted) {
        LOG_ERROR("Failed to start download for cross promotion file"); ENFORCE_LINE(63);
    }
}

void NKCrossPromo::OnAppPromoDownload(
    bool success, const std::string& savePath, const std::string& fileName, const NKError* error, int appID) {

    ++appPromoCompletions;

    if (error || !success) {
        appIDs.erase(appIDs.cbegin());
        TryStartAppPromoDownload();
    } else {
        CJSONWrapper jsonWrapper(fileIO);
        if (!ParseAppPromoDownload(jsonWrapper.ReadObject(savePath + "/" + fileName, fileIO->cachePolicy), appID)) {
            appIDs.erase(appIDs.cbegin());
            TryStartAppPromoDownload();
        }
    }

    downloadStarted = appPromoAttempts > appPromoCompletions || !pendingPromotions.empty();
}

uint32_t NKCrossPromo::ParseAppPromoDownload(const json_spirit::mObject& obj, int appID) {
    JSON_PropertyReader propertyReader;
    propertyReader.sourceObject = &obj;

    json_spirit::mObject links;
    propertyReader.Read<json_spirit::mObject>(links, "links");

    propertyReader.Unload();
    propertyReader.sourceObject = &links;

    std::string url;
    if (!propertyReader.ReadIfExists(url, platformID)) {
        propertyReader.ReadIfExists(url, "default");
    }

    if (url.empty()) {
        return 0;
    }

    propertyReader.Unload();
    propertyReader.sourceObject = &obj;

    json_spirit::mArray assets;
    propertyReader.Read<json_spirit::mArray>(assets, "assets");

    eTextureSize textureSize = textureManager->GetTextureSize();
    std::string appidKey = StringHelper::Format("appid-%d", manager->GetAppID());

    if (assets.empty()) {
        return 0;
    }

    uint32_t hits{};

    for (int i = 0; i < assets.size(); ++i) {
        propertyReader.Unload();
        propertyReader.sourceArray = &assets;

        json_spirit::mObject asset;
        propertyReader.Read(asset, i);

        propertyReader.Unload();
        propertyReader.sourceObject = &asset;

        std::vector<std::string> tags;
        propertyReader.Read<std::string>(tags, "tags");

        auto it = std::find(tags.begin(), tags.end(), platformID);
        if (it == tags.end()) {
            continue;
        }

        it = std::find(tags.begin(), tags.end(), appidKey);
        if (it == tags.end()) {
            continue;
        }

        std::string assetUrl;
        propertyReader.Read<std::string>(assetUrl, "url");
        replace_res_tags(assetUrl, nullptr, textureSize);

        NKCrossPromo::SPendingPromotion pendingPromo = { pendingPromoID++, appID, assetUrl, url, textureSize, tags };
        pendingPromotions.push_back(pendingPromo);

        ++hits;
        fileClient->Download(url, boost::bind(&NKCrossPromo::OnImageDownload, this, _1, _2, _3, _4, pendingPromo.id));
    }

    return hits;
}

void NKCrossPromo::TryStartAppPromoDownload() {
    if (appIDs.empty() && pendingPromotions.empty()) {
        appPromoFailed = true;
        return;
    }

    eNKLinkServerCluster serverCluster = manager->GetSessionModule()->GetServerCluster();
    std::string fileBaseUrl = NKEndpoints::GetPublicFileBaseUrl(serverCluster);
    std::string appdocUrl = StringHelper::Format(fileBaseUrl + "/appdocs/%d/appdocs/cross_promotion", appIDs[0]);

    ++appPromoAttempts;
    fileClient->Download(appdocUrl, boost::bind(&NKCrossPromo::OnAppPromoDownload, this, _1, _2, _3, _4, appIDs[0]));
}

void NKCrossPromo::OnImageDownload(
    bool success, const std::string& savePath, const std::string& fileName, const NKError* error, int id) {

    auto it = std::find_if(pendingPromotions.begin(), pendingPromotions.end(), [id](const NKCrossPromo::SPendingPromotion& pp) {
        return pp.id == id;
    });

    if (it != pendingPromotions.end()) {
        if (success && !error) {
            std::string fullPath = savePath + "/" + fileName;

            std::string textureExtension, textureName;
            size_t periodIndex = fileName.rfind('.');

            if (periodIndex == std::string::npos) {
                textureName = fileName;
            } else {
                textureExtension = fileName.substr(periodIndex + 1);
                textureName = fileName.substr(0, periodIndex);
            }

            eTextureType textureType = CTexture::ParseTextureType(textureExtension);
            textureManager->AddTextureFromFile(textureName, fullPath, fileIO->cachePolicy, textureType, it->textureSize, false);

            NKCrossPromo::SPromotion promo = { it->appID, textureName, it->url, it->tags };
            promotions.push_back(promo);

            appIDs.clear();
            pendingPromotions.erase(it);
        } else {
            eTextureSize textureSize = it->textureSize;
            it->textureSize = static_cast<eTextureSize>(static_cast<int>(textureSize) - 1);
            if (textureSize < eTextureSize::High) {
                pendingPromotions.erase(it);
            } else {
                std::string assetUrl = it->assetUrl;
                replace_res_tags(assetUrl, nullptr, it->textureSize);
                downloadStarted = fileClient->Download(
                    assetUrl, boost::bind(&NKCrossPromo::OnImageDownload, this, _1, _2, _3, _4, id));
            }
        }
    }

    if (pendingPromotions.empty()) {
        appPromoFailed = true;
        if (appPromoAttempts <= appPromoCompletions) {
            downloadStarted = false;
            if (promotions.empty()) {
                TryStartAppPromoDownload();
            }
        }
    } else {
        downloadStarted = true;
    }
}

void NKCrossPromo::ParsePromoDownload(const json_spirit::mObject& obj) {
    JSON_PropertyReader propertyReader;
    propertyReader.sourceObject = &obj;

    json_spirit::mObject promotionsObj;
    propertyReader.Read<json_spirit::mObject>(promotionsObj, "promotions");

    uint32_t managerAppID = manager->GetAppID();
    int64_t localTimeMS = manager->GetLocalTimeMS();

    propertyReader.Unload();
    propertyReader.sourceObject = &promotionsObj;

    json_spirit::mArray appData;
    propertyReader.Read<json_spirit::mArray>(appData, StringHelper::Format("%d", managerAppID));

    for (int i = 0; i < appData.size(); ++i) { // TODO: disassembly is weird. check with debugger.
        propertyReader.Unload();
        propertyReader.sourceArray = &appData;

        json_spirit::mObject inner;
        propertyReader.Read(inner, i);

        propertyReader.Unload();
        propertyReader.sourceObject = &inner;

        uint64_t startTime = -1;
        propertyReader.Read<uint64_t>(startTime, "startTime");

        if (localTimeMS >= startTime) {
            int appID = 0;
            propertyReader.Read<int>(appID, "appid");

            if (appID != managerAppID) {
                appIDs.push_back(appID);
            }
        }
    }

    TryStartAppPromoDownload();
}
