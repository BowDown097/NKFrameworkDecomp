#ifndef SANALYTICSINITIALISATION_H
#define SANALYTICSINITIALISATION_H
#include "IAnalytics.h"
#include "Uncategorized/Version.h"

struct SAnalyticsInitialisation
{
    std::string name;
    CVersion version;
    IAnalytics* host;

    SAnalyticsInitialisation(const std::string& name, const CVersion& version, IAnalytics* host)
        : name(name), version(version), host(host) {}
};

#endif // SANALYTICSINITIALISATION_H
