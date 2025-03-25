#pragma once

#include "DGAnalyticsData.h"
#include "DGAnalyticsEventGroups.h"
#include "NewFramework/Platform/Shared/Assert.h"
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

struct IAnalytics {
    bool field_8{}; // 0x08
    int doNotTrack{}; // 0x0C
    std::string name; // 0x10

    virtual ~IAnalytics() = default;

    virtual void Log(const std::string&);
    virtual void SetUser(const std::string&);
    virtual void AppActive();
    virtual void AppPaused();
    virtual void AppDestroyed();
    virtual void DidReceiveMemoryWarning();
    virtual void SetCheckpoint(
        std::string&, std::map<std::string, std::string>*,
        std::pair<double, std::string>, AnalyticsEventGroups::Group);
    virtual void StartTimedEvent(std::string&, std::map<std::string, std::string>*);
    virtual void EndTimedEvent(std::string&, std::map<std::string, std::string>*);
    virtual void SendDataEventWithID(std::string, const DGAnalyticsData&, AnalyticsEventGroups::Server);
    virtual void SetKey(const std::string&, const std::string&);
    virtual void SetKey(const std::string&, double);
    virtual void SetKey(const std::string&, unsigned long long);

    virtual void Init(const std::string& name, const CVersion& version) {
        this->name = name;
    }
    virtual unsigned int PassFilter(uint in) {
        return (in & 2) >> 1;
    }
    virtual void SetDoNotTrack(int doNotTrack) {
        this->doNotTrack = doNotTrack;
    }
    virtual bool SetUserConsent(bool) {
        ENFORCE_LINE(57); return NKAssert(false, "Setting consent unhandled by this interface");
    }
};

struct SAnalyticsInitialisation {
    std::string name; // 0x00
    CVersion version; // 0x18
    IAnalytics* host; // 0x38

    SAnalyticsInitialisation(const std::string& name, const CVersion& version, IAnalytics* host)
        : name(name), version(version), host(host) {}
};

class DGAnalytics {
public:
    static DGAnalytics* Instance();
    void ApplyEventGroupSettings(int throttleThreshold, json_spirit::mObject& obj);
    void Add(IAnalytics* host, std::string name, const CVersion& version, bool init);
    IAnalytics* Get(std::string name);
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

    bool _bEventsSupported = true; // 0x00
    bool _bDataEventsSupported{}; // 0x01
    int _sessionID{}; // 0x04
    std::vector<IAnalytics*> _analytics; // 0x08
    std::vector<SAnalyticsInitialisation> _analyticsInitialisation; // 0x20
    AnalyticsEventGroups::CGroupSettingsContainer _groupSettingsContainer; // 0x38
};
