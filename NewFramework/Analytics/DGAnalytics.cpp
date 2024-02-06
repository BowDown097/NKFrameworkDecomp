#include "DGAnalytics.h"
#include "DGAnalyticsData.h"
#include "NewFramework/Platform/Shared/Assert.h"
#include "NewFramework/Platform/Shared/Core.h"
#include "SAnalyticsInitialisation.h"
#include <cstdarg>

void DGAnalytics::Add(IAnalytics* host, std::string name, const CVersion& version, bool init)
{
    analytics.push_back(host);
    if (init)
    {
        host->Init(name, version);
        return;
    }
    analyticsInitialisation.emplace_back(name, version, host);
}

void DGAnalytics::AppActive()
{
    for (IAnalytics* analyticsHost : analytics)
        if (analyticsHost->field_8)
            analyticsHost->AppActive();
}

void DGAnalytics::AppDestroyed()
{
    for (IAnalytics* analyticsHost : analytics)
        if (analyticsHost->field_8)
            analyticsHost->AppDestroyed();
}

void DGAnalytics::AppPaused()
{
    for (IAnalytics* analyticsHost : analytics)
        if (analyticsHost->field_8)
            analyticsHost->AppPaused();
}

void DGAnalytics::ApplyEventGroupSettings(int throttleThreshold, json_spirit::mObject& obj)
{
    groupSettingsContainer->Deserialize(throttleThreshold, obj);
}

void DGAnalytics::DidReceiveMemoryWarning()
{
    for (IAnalytics* analyticsHost : analytics)
        if (analyticsHost->field_8)
            analyticsHost->DidReceiveMemoryWarning();
}

void DGAnalytics::EnableAnalyticsGroup(bool enabled, AnalyticsEventGroups::Group group)
{
    AnalyticsEventGroups::SGroupSettings& settings = groupSettingsContainer->GetSettings(group);
    settings.throttle = enabled;
}

void DGAnalytics::EndTimedEvent(std::string a, std::map<std::string, std::string>* b)
{
    if (!eventsSupported)
        return;

    for (IAnalytics* analyticsHost : analytics)
        if (analyticsHost->field_8)
            analyticsHost->EndTimedEvent(a, b);
}

std::string DGAnalytics::GetNonLiNKID()
{
    std::string vendorId = CCore::GetVendorId();
    return !vendorId.empty() ? "NO_LINK" + vendorId : "";
}

// TODO: uncomment NKManager line when finished
std::string DGAnalytics::GetUserID()
{
    //std::string userDetails = NKManager::GetManager()->GetSessionModule()->GetUserDetails();
    std::string userDetails = "";
    return !userDetails.empty() ? userDetails : GetNonLiNKID();
}

void DGAnalytics::Initialise()
{
    for (const SAnalyticsInitialisation& ai : analyticsInitialisation)
        ai.host->Init(ai.name, ai.version);
}

DGAnalytics* DGAnalytics::Instance()
{
    if (!_pInstance)
        _pInstance = new DGAnalytics;
    return _pInstance;
}

void DGAnalytics::Log(std::string message, ...)
{
    va_list args;
    va_start(args, message);

    char text[2048];
    vsnprintf(text, sizeof(text), message.c_str(), args);

    for (IAnalytics* analyticsHost : analytics)
        if (analyticsHost->field_8)
            analyticsHost->Log(text);

    va_end(args);
}

void DGAnalytics::SendDataEvent(const DGAnalyticsData& data, bool b, AnalyticsEventGroups::Group group, unsigned int d)
{
    AnalyticsEventGroups::Server server = AnalyticsEventGroups::Server::Server1;
    if (!dataEventsSupported && group != AnalyticsEventGroups::Group::AlwaysSend)
    {
        if (!eventsSupported)
            return;
        AnalyticsEventGroups::SGroupSettings& settings = groupSettingsContainer->GetSettings(group);
        if (!settings.throttle)
            return;
        server = settings.server;
    }

    if (b)
    {
        SetCheckpoint(data.field_20, const_cast<std::map<std::string, std::string>*>(&data.field_60),
                      data.field_40, group, d);
    }

    for (IAnalytics* analyticsHost : analytics)
        if (analyticsHost->field_8 && analyticsHost->PassFilter(d))
            analyticsHost->SendDataEventWithID(GetUserID(), data, server);
}

void DGAnalytics::SetCheckpoint(std::string a, std::map<std::string, std::string>* b, std::pair<double, std::string> c,
                                AnalyticsEventGroups::Group d, unsigned int e)
{
    if (!eventsSupported)
        return;

    for (IAnalytics* analyticsHost : analytics)
        if (analyticsHost->field_8 && analyticsHost->PassFilter(e))
            analyticsHost->SetCheckpoint(a, b, c, d);
}

void DGAnalytics::SetDoNotTrack(int doNotTrack)
{
    for (IAnalytics* analyticsHost : analytics)
        analyticsHost->SetDoNotTrack(doNotTrack);
}

void DGAnalytics::SetKey(const std::string& a, const std::string& b)
{
    for (IAnalytics* analyticsHost : analytics)
        if (analyticsHost->field_8)
            analyticsHost->SetKey(a, b);
}

void DGAnalytics::SetKey(const std::string& a, double b)
{
    for (IAnalytics* analyticsHost : analytics)
        if (analyticsHost->field_8)
            analyticsHost->SetKey(a, b);
}

void DGAnalytics::SetKey(const std::string& a, unsigned long long b)
{
    for (IAnalytics* analyticsHost : analytics)
        if (analyticsHost->field_8)
            analyticsHost->SetKey(a, b);
}

void DGAnalytics::SetSessionID(int sessionId)
{
    if (this->sessionId != 0 && this->sessionId != sessionId)
        NKAssert(false, "Changing the session ID after it has already been set, is this intentional?");
    this->sessionId = sessionId;
}

void DGAnalytics::SetUser(const std::string& user)
{
    for (IAnalytics* analyticsHost : analytics)
        if (analyticsHost->field_8)
            analyticsHost->SetUser(user);
}

void DGAnalytics::StartTimedEvent(std::string a, std::map<std::string, std::string>* b)
{
    if (!eventsSupported)
        return;

    for (IAnalytics* analyticsHost : analytics)
        if (analyticsHost->field_8)
            analyticsHost->StartTimedEvent(a, b);
}
