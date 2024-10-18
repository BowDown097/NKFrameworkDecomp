#pragma once
#include "DGAnalyticsData.h"
#include "DGAnalyticsEventGroups.h"

class CVersion;

struct IAnalytics
{
    bool field_8{}; // 0x08
    int doNotTrack{}; // 0x0C

    virtual ~IAnalytics();
    virtual void Init(const std::string&, const CVersion&);
    virtual void Log(const std::string&);
    virtual void SetUser(const std::string&);
    virtual void AppActive();
    virtual void AppPaused();
    virtual void AppDestroyed();
    virtual void DidReceiveMemoryWarning();
    virtual void SetCheckpoint(std::string&, std::map<std::string, std::string>*,
                               std::pair<double, std::string>, AnalyticsEventGroups::Group);
    virtual void StartTimedEvent(std::string&, std::map<std::string, std::string>*);
    virtual void EndTimedEvent(std::string&, std::map<std::string, std::string>*);
    virtual void SendDataEventWithID(std::string, const DGAnalyticsData&, AnalyticsEventGroups::Server);
    virtual void SetKey(const std::string&, const std::string&);
    virtual void SetKey(const std::string&, double);
    virtual void SetKey(const std::string&, unsigned long long);
    virtual unsigned int PassFilter(unsigned int in) { return (in & 2) >> 1; }
    virtual void SetDoNotTrack(int doNotTrack) { this->doNotTrack = doNotTrack; }
};
