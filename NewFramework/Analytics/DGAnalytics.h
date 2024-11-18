#pragma once
#include "SAnalyticsInitialisation.h"
#include "Uncategorized/Version.h"

static const std::string nullString = "";
static const std::string CANNOT_FIND_LINK = "ERR_USER_CANNOT_FIND_LINK";
static const std::string sNKRoot = "LiNK";
static const std::string sNKAccountLinked = "AccountLinked";
static const std::string sNKSessionOpen = "SessionOpen";
static const std::string sNKSessionOpened = "SessionOpened";
static const std::string sNKSessionSwitch = "SessionSwitch";
static const std::string sNKNewUser = "NewUser";
static const std::string sNKExistingUser = "ExistingUser";
static const std::string sNKError_HttpParse = "HTTP:Parse";
static const std::string sNKError_HttpSecurity = "HTTP:Security";
static const std::string sNKError_HttpLink = "HTTP:Link";
static const std::string sNKError_HttpRequest = "HTTP:Request";
static const std::string sNKError_HttpError = "HTTP:Error";
static const std::string sNKError_HttpFileWrite = "HTTP:FileWrite";

class DGAnalytics
{
public:
    DGAnalytics() : groupSettingsContainer(new AnalyticsEventGroups::CGroupSettingsContainer) {}
    static DGAnalytics* Instance();
    void ApplyEventGroupSettings(int throttleThreshold, json_spirit::mObject& obj);
    void Add(IAnalytics* host, std::string name, const CVersion& version, bool init);
    void Initialise();
    void Log(std::string message, ...);
    void SetUser(const std::string& user);
    void AppActive();
    void AppPaused();
    void AppDestroyed();
    void DidReceiveMemoryWarning();
    void SetCheckpoint(std::string a, std::map<std::string, std::string>* b, std::pair<double, std::string> c,
                       AnalyticsEventGroups::Group d, unsigned int e);
    void StartTimedEvent(std::string a, std::map<std::string, std::string>* b);
    void EndTimedEvent(std::string a, std::map<std::string, std::string>* b);
    void SetKey(const std::string& a, const std::string& b);
    void SetKey(const std::string& a, double b);
    void SetKey(const std::string& a, unsigned long long b);
    void SendDataEvent(const DGAnalyticsData& data, bool b, AnalyticsEventGroups::Group group, unsigned int d);
    std::string GetUserID();
    void SetDoNotTrack(int doNotTrack);
    std::string GetNonLiNKID();
    void SetSessionID(int sessionId);
    void EnableAnalyticsGroup(bool enabled, AnalyticsEventGroups::Group group);
private:
    static inline DGAnalytics* _pInstance;

    bool eventsSupported = true; // 0x00
    bool dataEventsSupported{}; // 0x01
    int sessionId{}; // 0x04
    std::vector<IAnalytics*> analytics; // 0x08
    std::vector<SAnalyticsInitialisation> analyticsInitialisation; // 0x20
    AnalyticsEventGroups::CGroupSettingsContainer* groupSettingsContainer; // 0x38
};
