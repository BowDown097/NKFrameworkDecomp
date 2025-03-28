#pragma once

#include <json_spirit/json_spirit_value.h>

namespace AnalyticsEventGroups {
    enum class Group {
        Global,
        AlwaysSend,
        Gameplay,
        Network,
        Account,
        Framework,
        Assert,
        LiNK
    };

    enum class Server {
        Unk0,
        Unk1,
        Unk2
    };

    struct SGroupSettings {
        Group eGroup; // 0x00
        bool bThrottle; // 0x04
        int throttlePercentage; // 0x08
        Server eServer; // 0x0C

        SGroupSettings(Group eGroup, bool bThrottle, Server eServer);
        void Serialize(json_spirit::mObject& obj);
        bool Deserialize(int throttleThreshold, json_spirit::mObject& obj);
    };

    class CGroupSettingsContainer {
    public:
        CGroupSettingsContainer();
        SGroupSettings& GetSettings(Group eGroup);
        void Serialize(json_spirit::mObject& obj);
        bool Deserialize(int throttleThreshold, json_spirit::mObject& obj);
    private:
        std::vector<SGroupSettings> _vecGroupSettings;
    };
}
