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
    std::string sName; // 0x10

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

    virtual void Init(const std::string& sName, const CVersion& version) {
        this->sName = sName;
    }
    virtual unsigned int PassFilter(uint uIn) {
        return (uIn & 2) >> 1;
    }
    virtual void SetDoNotTrack(int doNotTrack) {
        this->doNotTrack = doNotTrack;
    }
    virtual bool SetUserConsent(bool bConsent) {
        ENFORCE_LINE(57); return NKAssert(false, "Setting consent unhandled by this interface");
    }
};

struct SAnalyticsInitialisation {
    std::string sName; // 0x00
    CVersion version; // 0x18
    IAnalytics* pAnalytics; // 0x38

    SAnalyticsInitialisation(const std::string& sName, const CVersion& version, IAnalytics* pAnalytics);
};

class DGAnalytics {
public:
    static DGAnalytics* Instance();
    void ApplyEventGroupSettings(int throttleThreshold, json_spirit::mObject& obj);
    void Add(IAnalytics* pAnalytics, std::string sName, const CVersion& version, bool bInit);
    IAnalytics* Get(std::string sName);
    void Initialise();
    void Log(std::string sMessage, ...);
    void SetUser(const std::string& sUser);
    void AppActive();
    void AppPaused();
    void AppDestroyed();
    void DidReceiveMemoryWarning();
    void SetCheckpoint(
        std::string a2, std::map<std::string, std::string>* a3,
        std::pair<double, std::string> a4,
        AnalyticsEventGroups::Group a5, uint a6);
    void StartTimedEvent(std::string a2, std::map<std::string, std::string>* a3);
    void EndTimedEvent(std::string a2, std::map<std::string, std::string>* a3);
    void SetKey(const std::string& a2, const std::string& a3);
    void SetKey(const std::string& a2, double a3);
    void SetKey(const std::string& a2, unsigned long long a3);
    void SendDataEvent(
        const DGAnalyticsData& data, bool a3,
        AnalyticsEventGroups::Group eGroup, uint a5);
    std::string GetUserID();
    void SetDoNotTrack(int doNotTrack);
    std::string GetNonLiNKID();
    void SetSessionID(int sessionId);
    void EnableAnalyticsGroup(bool bEnabled, AnalyticsEventGroups::Group eGroup);
private:
    static inline DGAnalytics* _pInstance;

    bool _bEventsSupported = true; // 0x00
    bool _bDataEventsSupported{}; // 0x01
    int _sessionID{}; // 0x04
    std::vector<IAnalytics*> _vecAnalytics; // 0x08
    std::vector<SAnalyticsInitialisation> _vecInitialisation; // 0x20
    AnalyticsEventGroups::CGroupSettingsContainer _groupSettingsContainer; // 0x38
};
