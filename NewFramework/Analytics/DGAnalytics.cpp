#include "DGAnalytics.h"

#include "DGAnalyticsData.h"
#include "NewFramework/Networking/NKAPI/NKManager.h"
#include "NewFramework/Networking/NKAPI/NKSession.h"
#include "NewFramework/Platform/Shared/Core.h"

#include <cstdarg>









































































































DGAnalytics* DGAnalytics::Instance() {
    if (!_pInstance) {
        _pInstance = new DGAnalytics;
    }
    return _pInstance;
}

void DGAnalytics::ApplyEventGroupSettings(int throttleThreshold, json_spirit::mObject& obj) {
    _groupSettingsContainer.Deserialize(throttleThreshold, obj);
}

void DGAnalytics::Add(IAnalytics* pAnalytics, std::string sName, const CVersion& version, bool bInit) {
    _vecAnalytics.push_back(pAnalytics);
    if (bInit) {
        pAnalytics->Init(sName, version);
    } else {
        _vecInitialisation.emplace_back(sName, version, pAnalytics);
    }
}

IAnalytics* DGAnalytics::Get(std::string sName) {
    for (IAnalytics* pAnalytics : _vecAnalytics) {
        if (pAnalytics->sName == sName) {
            return pAnalytics;
        }
    }

    return nullptr;
}

void DGAnalytics::Initialise() {
    for (const SAnalyticsInitialisation& ai : _vecInitialisation) {
        ai.pAnalytics->Init(ai.sName, ai.version);
    }
}

void DGAnalytics::Log(std::string sMessage, ...) {
    va_list args;
    va_start(args, sMessage);

    char text[2048];
    vsnprintf(text, sizeof(text), sMessage.c_str(), args);

    for (IAnalytics* pAnalytics : _vecAnalytics) {
        if (pAnalytics->field_8) {
            pAnalytics->Log(text);
        }
    }

    va_end(args);
}

void DGAnalytics::SetUser(const std::string& sUser) {
    for (IAnalytics* pAnalytics : _vecAnalytics) {
        if (pAnalytics->field_8) {
            pAnalytics->SetUser(sUser);
        }
    }
}

void DGAnalytics::AppActive() {
    for (IAnalytics* pAnalytics : _vecAnalytics) {
        if (pAnalytics->field_8) {
            pAnalytics->AppActive();
        }
    }
}

void DGAnalytics::AppPaused() {
    for (IAnalytics* pAnalytics : _vecAnalytics) {
        if (pAnalytics->field_8) {
            pAnalytics->AppPaused();
        }
    }
}

void DGAnalytics::AppDestroyed() {
    for (IAnalytics* pAnalytics : _vecAnalytics) {
        if (pAnalytics->field_8) {
            pAnalytics->AppDestroyed();
        }
    }
}

void DGAnalytics::DidReceiveMemoryWarning() {
    for (IAnalytics* pAnalytics : _vecAnalytics) {
        if (pAnalytics->field_8) {
            pAnalytics->DidReceiveMemoryWarning();
        }
    }
}

void DGAnalytics::SetCheckpoint(
    std::string a2, std::map<std::string, std::string>* a3,
    std::pair<double, std::string> a4,
    AnalyticsEventGroups::Group a5, uint a6) {

    if (!_bEventsSupported) {
        return;
    }

    for (IAnalytics* pAnalytics : _vecAnalytics) {
        if (pAnalytics->field_8 && pAnalytics->PassFilter(a6)) {
            pAnalytics->SetCheckpoint(a2, a3, a4, a5);
        }
    }
}

void DGAnalytics::StartTimedEvent(std::string a2, std::map<std::string, std::string>* a3) {
    if (!_bEventsSupported) {
        return;
    }

    for (IAnalytics* pAnalytics : _vecAnalytics) {
        if (pAnalytics->field_8) {
            pAnalytics->StartTimedEvent(a2, a3);
        }
    }
}

void DGAnalytics::EndTimedEvent(std::string a2, std::map<std::string, std::string>* a3) {
    if (!_bEventsSupported) {
        return;
    }

    for (IAnalytics* pAnalytics : _vecAnalytics) {
        if (pAnalytics->field_8) {
            pAnalytics->EndTimedEvent(a2, a3);
        }
    }
}

void DGAnalytics::SetKey(const std::string& a2, const std::string& a3) {
    for (IAnalytics* pAnalytics : _vecAnalytics) {
        if (pAnalytics->field_8) {
            pAnalytics->SetKey(a2, a3);
        }
    }
}

void DGAnalytics::SetKey(const std::string& a2, double a3) {
    for (IAnalytics* pAnalytics : _vecAnalytics) {
        if (pAnalytics->field_8) {
            pAnalytics->SetKey(a2, a3);
        }
    }
}

void DGAnalytics::SetKey(const std::string& a2, unsigned long long a3) {
    for (IAnalytics* pAnalytics : _vecAnalytics) {
        if (pAnalytics->field_8) {
            pAnalytics->SetKey(a2, a3);
        }
    }
}

void DGAnalytics::SendDataEvent(
    const DGAnalyticsData& data, bool a3,
    AnalyticsEventGroups::Group eGroup, uint a5) {

    AnalyticsEventGroups::Server eServer = AnalyticsEventGroups::Server::Unk0;
    if (!_bDataEventsSupported && eGroup != AnalyticsEventGroups::Group::AlwaysSend) {
        if (!_bEventsSupported) {
            return;
        }
        const AnalyticsEventGroups::SGroupSettings& settings = _groupSettingsContainer.GetSettings(eGroup);
        if (!settings.bThrottle) {
            return;
        }
        eServer = settings.eServer;
    }

    if (a3) {
        SetCheckpoint(
            data.field_20, const_cast<std::map<std::string, std::string>*>(&data.field_60),
            data.field_40, eGroup, a5);
    }

    for (IAnalytics* pAnalytics : _vecAnalytics) {
        if (pAnalytics->field_8 && pAnalytics->PassFilter(a5)) {
            pAnalytics->SendDataEventWithID(GetUserID(), data, eServer);
        }
    }
}

std::string DGAnalytics::GetUserID() {
    std::string sNKapiID = NKManager::GetManager()->GetSessionModule()->GetUserDetails().nkapiID;
    return !sNKapiID.empty() ? sNKapiID : GetNonLiNKID();
}

void DGAnalytics::SetDoNotTrack(int doNotTrack) {
    for (IAnalytics* pAnalytics : _vecAnalytics) {
        pAnalytics->SetDoNotTrack(doNotTrack);
    }
}

std::string DGAnalytics::GetNonLiNKID() {
    std::string sVendorID = CCore::GetVendorId();
    return !sVendorID.empty() ? "NO_LINK" + sVendorID : "";
}

void DGAnalytics::SetSessionID(int sessionID) {
    NKAssert(!_sessionID || _sessionID == sessionID, "Changing the session ID after it has already been set, is this intentional?"); ENFORCE_LINE(316);
    _sessionID = sessionID;
}

void DGAnalytics::EnableAnalyticsGroup(bool bEnabled, AnalyticsEventGroups::Group eGroup) {
    AnalyticsEventGroups::SGroupSettings& settings = _groupSettingsContainer.GetSettings(eGroup);
    settings.bThrottle = bEnabled;
}

SAnalyticsInitialisation::SAnalyticsInitialisation(const std::string& sName, const CVersion& version, IAnalytics* pAnalytics)
    : sName(sName), version(version), pAnalytics(pAnalytics) {}
