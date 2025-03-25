#include "DGAnalytics.h"

#include "DGAnalyticsData.h"
#include "NewFramework/Networking/NKAPI/NKManager.h"
#include "NewFramework/Networking/NKAPI/NKSession.h"
#include "NewFramework/Platform/Shared/Core.h"

#include <cstdarg>















































































































SAnalyticsInitialisation::SAnalyticsInitialisation(const std::string& name, const CVersion& version, IAnalytics* host)
    : name(name), version(version), host(host) {}

DGAnalytics* DGAnalytics::Instance() {
    if (!_pInstance) {
        _pInstance = new DGAnalytics;
    }
    return _pInstance;
}

void DGAnalytics::ApplyEventGroupSettings(int throttleThreshold, json_spirit::mObject& obj) {
    _groupSettingsContainer.Deserialize(throttleThreshold, obj);
}

void DGAnalytics::Add(IAnalytics* host, std::string name, const CVersion& version, bool init) {
    _analytics.push_back(host);
    if (init) {
        host->Init(name, version);
        return;
    }
    _analyticsInitialisation.emplace_back(name, version, host);
}

void DGAnalytics::Initialise() {
    for (const SAnalyticsInitialisation& ai : _analyticsInitialisation) {
        ai.host->Init(ai.name, ai.version);
    }
}

void DGAnalytics::Log(std::string message, ...) {
    va_list args;
    va_start(args, message);

    char text[2048];
    vsnprintf(text, sizeof(text), message.c_str(), args);

    for (IAnalytics* analyticsHost : _analytics) {
        if (analyticsHost->field_8) {
            analyticsHost->Log(text);
        }
    }

    va_end(args);
}

void DGAnalytics::SetUser(const std::string& user) {
    for (IAnalytics* analyticsHost : _analytics) {
        if (analyticsHost->field_8) {
            analyticsHost->SetUser(user);
        }
    }
}

void DGAnalytics::AppActive() {
    for (IAnalytics* analyticsHost : _analytics) {
        if (analyticsHost->field_8) {
            analyticsHost->AppActive();
        }
    }
}

void DGAnalytics::AppPaused() {
    for (IAnalytics* analyticsHost : _analytics) {
        if (analyticsHost->field_8) {
            analyticsHost->AppPaused();
        }
    }
}

void DGAnalytics::AppDestroyed() {
    for (IAnalytics* analyticsHost : _analytics) {
        if (analyticsHost->field_8) {
            analyticsHost->AppDestroyed();
        }
    }
}

void DGAnalytics::DidReceiveMemoryWarning() {
    for (IAnalytics* analyticsHost : _analytics) {
        if (analyticsHost->field_8) {
            analyticsHost->DidReceiveMemoryWarning();
        }
    }
}

void DGAnalytics::SetCheckpoint(
    std::string a, std::map<std::string, std::string>* b,
    std::pair<double, std::string> c,
    AnalyticsEventGroups::Group d, unsigned int e) {

    if (!_bEventsSupported) {
        return;
    }

    for (IAnalytics* analyticsHost : _analytics) {
        if (analyticsHost->field_8 && analyticsHost->PassFilter(e)) {
            analyticsHost->SetCheckpoint(a, b, c, d);
        }
    }
}

void DGAnalytics::StartTimedEvent(std::string a, std::map<std::string, std::string>* b) {
    if (!_bEventsSupported) {
        return;
    }

    for (IAnalytics* analyticsHost : _analytics) {
        if (analyticsHost->field_8) {
            analyticsHost->StartTimedEvent(a, b);
        }
    }
}

void DGAnalytics::EndTimedEvent(std::string a, std::map<std::string, std::string>* b) {
    if (!_bEventsSupported) {
        return;
    }

    for (IAnalytics* analyticsHost : _analytics) {
        if (analyticsHost->field_8) {
            analyticsHost->EndTimedEvent(a, b);
        }
    }
}

void DGAnalytics::SetKey(const std::string& a, const std::string& b) {
    for (IAnalytics* analyticsHost : _analytics) {
        if (analyticsHost->field_8) {
            analyticsHost->SetKey(a, b);
        }
    }
}

void DGAnalytics::SetKey(const std::string& a, double b) {
    for (IAnalytics* analyticsHost : _analytics) {
        if (analyticsHost->field_8) {
            analyticsHost->SetKey(a, b);
        }
    }
}

void DGAnalytics::SetKey(const std::string& a, unsigned long long b) {
    for (IAnalytics* analyticsHost : _analytics) {
        if (analyticsHost->field_8) {
            analyticsHost->SetKey(a, b);
        }
    }
}

void DGAnalytics::SendDataEvent(
    const DGAnalyticsData& data, bool b,
    AnalyticsEventGroups::Group group, unsigned int d) {

    AnalyticsEventGroups::Server server = AnalyticsEventGroups::Server::Unk0;
    if (!_bDataEventsSupported && group != AnalyticsEventGroups::Group::AlwaysSend) {
        if (!_bEventsSupported) {
            return;
        }
        AnalyticsEventGroups::SGroupSettings& settings = _groupSettingsContainer.GetSettings(group);
        if (!settings.throttle) {
            return;
        }
        server = settings.server;
    }

    if (b) {
        SetCheckpoint(
            data.field_20, const_cast<std::map<std::string, std::string>*>(&data.field_60),
            data.field_40, group, d);
    }

    for (IAnalytics* analyticsHost : _analytics) {
        if (analyticsHost->field_8 && analyticsHost->PassFilter(d)) {
            analyticsHost->SendDataEventWithID(GetUserID(), data, server);
        }
    }
}

std::string DGAnalytics::GetUserID() {
    std::string nkapiID = NKManager::GetManager()->GetSessionModule()->GetUserDetails().nkapiID;
    return !nkapiID.empty() ? nkapiID : GetNonLiNKID();
}

void DGAnalytics::SetDoNotTrack(int doNotTrack) {
    for (IAnalytics* analyticsHost : _analytics) {
        analyticsHost->SetDoNotTrack(doNotTrack);
    }
}

std::string DGAnalytics::GetNonLiNKID() {
    std::string vendorId = CCore::GetVendorId();
    return !vendorId.empty() ? "NO_LINK" + vendorId : "";
}

void DGAnalytics::SetSessionID(int sessionId) {
    if (_sessionID != 0 && _sessionID != sessionId) {
        NKAssert(false, "Changing the session ID after it has already been set, is this intentional?"); ENFORCE_LINE(316);
    }
    _sessionID = sessionId;
}

void DGAnalytics::EnableAnalyticsGroup(bool enabled, AnalyticsEventGroups::Group group) {
    AnalyticsEventGroups::SGroupSettings& settings = _groupSettingsContainer.GetSettings(group);
    settings.throttle = enabled;
}
