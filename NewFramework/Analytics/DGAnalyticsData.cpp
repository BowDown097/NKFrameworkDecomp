#include "DGAnalyticsData.h"
#include "NewFramework/Utilities/StringHelper.h"

DGAnalyticsData& operator<<(DGAnalyticsData& data, const std::string& str)
{
    if (data.field_0)
    {
        data.field_8 = str;
        data.field_0 = false;
    }
    else
    {
        data.field_60.emplace(data.field_8, str);
        data.field_0 = true;
    }

    return data;
}

DGAnalyticsData::DGAnalyticsData(const DGAnalyticsData& other)
{
    field_0 = other.field_0;
    field_8 = other.field_8;
    field_20 = other.field_20;
    field_38 = other.field_38;
    field_40 = other.field_40;
    field_48 = other.field_48;

    for (auto it = other.field_60.begin(); it != other.field_60.end(); ++it)
        field_60.emplace(it->first, it->second);
}

DGAnalyticsData& operator<<(DGAnalyticsData& data, int num)
{
    if (data.field_0)
    {
        data.field_0 = false;
        data.field_38 = true;
    }
    else
    {
        data.field_60.emplace(data.field_8, StringHelper::Format("%d", num));
        data.field_0 = true;
    }

    return data;
}

void DGAnalyticsData::AddPair(const std::string& key, const std::string& value)
{
    field_60.emplace(key, value);
}

void DGAnalyticsData::AddPair(const std::string& key, int value)
{
    field_60.emplace(key, StringHelper::Format("%d", value));
}
