#include "DGAnalyticsEventGroups.h"
#include "NewFramework/Platform/Shared/Logging.h"
#include "NewFramework/Utilities/JSON/JSON_PropertyReader/JSON_Read.h"





































namespace AnalyticsEventGroups {
    std::string names[] = {
        "global",
        "alwaysSend",
        "gameplay",
        "network",
        "account",
        "framework",
        "assert",
        "link"
    };

    Server server[] = {
        Server::Unk0,
        Server::Unk0,
        Server::Unk0,
        Server::Unk0,
        Server::Unk0,
        Server::Unk0,
        Server::Unk1,
        Server::Unk2
    };

    SGroupSettings::SGroupSettings(Group group, bool throttle, Server server)
        : group(group), throttle(throttle), throttlePercentage(throttle ? 100 : 0), server(server) {}

    void SGroupSettings::Serialize(json_spirit::mObject& obj) {
        obj["throttle_percentage"] = throttlePercentage;
    }

    bool SGroupSettings::Deserialize(int throttleThreshold, json_spirit::mObject& obj) {
        JSON_PropertyReader propertyReader;
        propertyReader.sourceObject = &obj;

        bool read = propertyReader.ReadIfExists(throttlePercentage, "throttle_percentage");
        if (read) {
            throttle = throttlePercentage > throttleThreshold;
        }

        return read;
    }

    CGroupSettingsContainer::CGroupSettingsContainer() {
        _groupSettings.reserve(8);
        for (int i = 0; i < 8; i++) {
            _groupSettings.emplace_back(static_cast<Group>(i), true, server[i]);
        }
    }

    SGroupSettings& CGroupSettingsContainer::GetSettings(Group group) {
        return _groupSettings.at(static_cast<int>(group));
    }

    void CGroupSettingsContainer::Serialize(json_spirit::mObject& obj) {
        for (int i = 0; i < 8; i++) {
            json_spirit::mObject serializedGroupSetting;
            _groupSettings[i].Serialize(serializedGroupSetting);
            obj[names[i]] = serializedGroupSetting;
        }
    }

    bool CGroupSettingsContainer::Deserialize(int throttleThreshold, json_spirit::mObject& obj) {
        JSON_PropertyReader propertyReader;
        propertyReader.sourceObject = &obj;

        for (int i = 0; i < 8; i++) {
            if (propertyReader.ReadIfExists(obj, names[i]) && !_groupSettings[i].Deserialize(throttleThreshold, obj)) {
                LOG_ERROR("Error applying settings to %s", names[i].c_str()); ENFORCE_LINE(108);
            }
        }

        return true;
    }
}
