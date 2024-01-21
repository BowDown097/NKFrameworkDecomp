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
    bool AppActive();
    bool AppDestroyed();
    bool AppPaused();
    bool DidReceiveMemoryWarning();
    void EnableAnalyticsGroup(bool, AnalyticsEventGroups::Group);
    void EndTimedEvent(std::string, std::map<std::string, std::string>*);
    void GetNonLiNKID();
    void GetUserID();
    void Initialize();
    void Log(std::string, ...);
    void SendDataEvent(const DGAnalyticsData&, bool, AnalyticsEventGroups::Group, unsigned int);
    void SetCheckpoint(std::string, std::map<std::string, std::string>*, std::pair<double, std::string>,
                       AnalyticsEventGroups::Group, unsigned int);
    void SetDoNotTrack(int);
    void SetKey(const std::string&, const std::string&);
    void SetKey(const std::string&, double);
    void SetKey(const std::string&, unsigned long long);
    void SetSessionID(int);
    void SetUser(const std::string&);
    void StartTimedEvent(std::string, std::map<std::string, std::string>*);
private:
    static inline DGAnalytics* _pInstance;
};

#endif // DGANALYTICS_H
