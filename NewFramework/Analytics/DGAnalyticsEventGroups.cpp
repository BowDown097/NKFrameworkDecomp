#include "DGAnalyticsEventGroups.h"
#include "NewFramework/Platform/Shared/Logging.h"
#include "NewFramework/Utilities/Json/Json_PropertyReader/JSON_Read.h"

AnalyticsEventGroups::CGroupSettingsContainer::CGroupSettingsContainer()
{
    groupSettings.reserve(8);
    for (int i = 0; i < 8; i++)
        groupSettings.emplace_back(static_cast<Group>(i), true, static_cast<Server>(server[i]));
}

bool AnalyticsEventGroups::CGroupSettingsContainer::Deserialize(int throttleThreshold, json_spirit::mObject& obj)
{
    JSON_PropertyReader propertyReader;
    propertyReader.sourceObject = &obj;

    for (int i = 0; i < 8; i++)
    {
        bool read = propertyReader.ReadIfExists(obj, names[i]);
        if (read && !groupSettings[i].Deserialize(throttleThreshold, obj))
            LOG_ERROR("Error applying settings to %s", names[i].c_str());
    }

    return true;
}

AnalyticsEventGroups::SGroupSettings& AnalyticsEventGroups::CGroupSettingsContainer::GetSettings(Group group)
{
    return groupSettings.at(static_cast<int>(group));
}

void AnalyticsEventGroups::CGroupSettingsContainer::Serialize(json_spirit::mObject& obj)
{
    for (int i = 0; i < 8; i++)
    {
        json_spirit::mObject serializedGroupSetting;
        groupSettings[i].Serialize(serializedGroupSetting);
        obj[names[i]] = serializedGroupSetting;
    }
}

AnalyticsEventGroups::SGroupSettings::SGroupSettings(Group group, bool throttle, Server server)
    : group(group), throttle(throttle), throttlePercentage(throttle ? 100 : 0), server(server) {}

bool AnalyticsEventGroups::SGroupSettings::Deserialize(int throttleThreshold, json_spirit::mObject& obj)
{
    JSON_PropertyReader propertyReader;
    propertyReader.sourceObject = &obj;

    bool read = propertyReader.ReadIfExists(throttlePercentage, "throttle_percentage");
    if (read)
        throttle = throttlePercentage > throttleThreshold;

    return read;
}

void AnalyticsEventGroups::SGroupSettings::Serialize(json_spirit::mObject& obj)
{
    obj["throttle_percentage"] = throttlePercentage;
}
