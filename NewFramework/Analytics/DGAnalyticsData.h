#pragma once

#include <map>
#include <string>

struct DGAnalyticsData {
    bool field_0 = false; // 0x00
    std::string field_8; // 0x08
    std::string field_20; // 0x20
    bool field_38 = true; // 0x38
    std::pair<double, std::string> field_40; // 0x40
    std::map<std::string, std::string> field_60; // 0x60

    DGAnalyticsData& operator<<(const std::string& str);
    DGAnalyticsData& AddPair(const std::string& key, long long value);
    DGAnalyticsData& operator<<(int num);
    DGAnalyticsData& AddPair(const std::string& key, const std::string& value);
    DGAnalyticsData& AddPair(const std::string& key, int value);
    explicit DGAnalyticsData(std::string field_20);
};
