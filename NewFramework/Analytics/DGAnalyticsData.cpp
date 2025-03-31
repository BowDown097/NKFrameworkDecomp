#include "DGAnalyticsData.h"

#include "NewFramework/Utilities/StringHelper.h"

DGAnalyticsData::DGAnalyticsData(std::string field_20)
    : field_0(true),
      field_20(field_20),
      field_38(true) {}

DGAnalyticsData& DGAnalyticsData::operator<<(const std::string& str) {
    if (field_0) {
        field_8 = str;
        field_0 = false;
    } else {
        field_60.emplace(field_8, str);
        field_0 = true;
    }

    return *this;
}

DGAnalyticsData& DGAnalyticsData::AddPair(const std::string& key, long long value) {
    field_60.emplace(key, StringHelper::Format("%lld", value));
    return *this;
}

DGAnalyticsData& DGAnalyticsData::operator<<(int num) {
    if (field_0) {
        field_0 = false;
        field_38 = true;
    } else {
        field_60.emplace(field_8, StringHelper::Format("%d", num));
        field_0 = true;
    }

    return *this;
}

DGAnalyticsData& DGAnalyticsData::AddPair(const std::string& key, const std::string& value) {
    field_60.emplace(key, value);
    return *this;
}

DGAnalyticsData& DGAnalyticsData::AddPair(const std::string& key, int value) {
    field_60.emplace(key, StringHelper::Format("%d", value));
    return *this;
}
