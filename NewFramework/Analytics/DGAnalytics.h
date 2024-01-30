#ifndef DGANALYTICS_H
#define DGANALYTICS_H
#include <map>
#include <string>

// TODO: implement this at a later time

class CVersion;
class DGAnalyticsData;
class IAnalytics;

namespace AnalyticsEventGroups { class Group; }

class DGAnalytics
{
public:
    static DGAnalytics* Instance();
    // void ApplyEventGroupSettings(int, std::map<std::string,json_spirit::Value_impl<json_spirit::Config_map<std::string>>>&);
    void Add(IAnalytics*, std::string, const CVersion&, bool);
    void Initialize();
    void Log(std::string, ...);
    void SetUser(const std::string&);
    bool AppActive();
    bool AppPaused();
    bool AppDestroyed();
    bool DidReceiveMemoryWarning();
    void SetCheckpoint(std::string, std::map<std::string, std::string>*, std::pair<double, std::string>,
                       AnalyticsEventGroups::Group, unsigned int);
    void StartTimedEvent(std::string, std::map<std::string, std::string>*);
    void EndTimedEvent(std::string, std::map<std::string, std::string>*);
    void SetKey(const std::string&, const std::string&);
    void SetKey(const std::string&, double);
    void SetKey(const std::string&, unsigned long long);
    void SendDataEvent(const DGAnalyticsData&, bool, AnalyticsEventGroups::Group, unsigned int);
    void GetUserID();
    void SetDoNotTrack(int);
    void GetNonLiNKID();
    void SetSessionID(int);
    void EnableAnalyticsGroup(bool, AnalyticsEventGroups::Group);
private:
    static inline DGAnalytics* _pInstance;
};

#endif // DGANALYTICS_H
