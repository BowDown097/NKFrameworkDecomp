#pragma once
#include <json_spirit/json_spirit_value.h>

namespace AnalyticsEventGroups
{
    enum class Group { Global, AlwaysSend, Gameplay, Network, Account, Framework, Assert, LiNK };
    enum class Server { Unk0, Unk1, Unk2 };
    static std::string names[] = { "global", "alwaysSend", "gameplay", "network", "account", "framework", "assert", "link" };

    // i'm not sure about this, but it's the best guess i can make
    // based on its one and only usage in CGroupSettingsContainer's constructor.
    // might have to look at this in a debugger later.
    static Server server[] = { Server::Unk0, Server::Unk0, Server::Unk0, Server::Unk0, Server::Unk0,
                               Server::Unk0, Server::Unk1, Server::Unk2, Server::Unk0, Server::Unk0,
                               Server::Unk0, Server::Unk0 };

    struct SGroupSettings
    {
        Group group; // 0x00
        bool throttle; // 0x04
        int throttlePercentage; // 0x08
        Server server; // 0x0C

        SGroupSettings(Group group, bool throttle, Server server);
        bool Deserialize(int throttleThreshold, json_spirit::mObject& obj);
        void Serialize(json_spirit::mObject& obj);
    };

    class CGroupSettingsContainer
    {
    public:
        CGroupSettingsContainer();
        SGroupSettings& GetSettings(Group group);
        bool Deserialize(int throttleThreshold, json_spirit::mObject& obj);
        void Serialize(json_spirit::mObject& obj);
    private:
        std::vector<SGroupSettings> groupSettings;
    };
}
