#ifndef DGANALYTICSDATA_H
#define DGANALYTICSDATA_H
#include <map>
#include <string>

struct DGAnalyticsData
{
    bool field_0; // 0x00
    std::string field_8; // 0x08
    std::string field_20; // 0x20
    bool field_38; // 0x38
    std::pair<double, std::string> field_40; // 0x40
    std::string field_48; // 0x48
    std::map<std::string, std::string> field_60; // 0x60

    friend DGAnalyticsData& operator<<(DGAnalyticsData& data, const std::string& str);
    DGAnalyticsData(const DGAnalyticsData& other);
    friend DGAnalyticsData& operator<<(DGAnalyticsData& data, int num);
    void AddPair(const std::string& key, const std::string& value);
    void AddPair(const std::string& key, int value);
    DGAnalyticsData() : field_0(false), field_38(true) {}
    DGAnalyticsData(const std::string& field_20) : field_0(true), field_20(field_20), field_38(false) {}
};

#endif // DGANALYTICSDATA_H
