#pragma once
#include "NewFramework/BaseApplication/LegacyFeatures/LegacyFeatures.h"
#include <sstream>

class C_NKLoginQuery
{
public:
    C_NKLoginQuery(const std::string& query)
    {
        oss << query;
        std::string sep = LegacyFeatures::bUseOldWebView ? "?" : "#";
        pairs += query.find(sep) != std::string::npos;
        pairs += std::count(query.begin(), query.end(), '&');
    }

    template<typename T>
    void AddField(const std::string& key, const T& value)
    {
        std::string initialSep = LegacyFeatures::bUseOldWebView ? "?" : "#";
        int pairsTmp = pairs;
        ++pairs;
        std::string sep = pairsTmp > 0 ? "&" : initialSep;
        oss << sep << key << "=" << value;
    }
private:
    std::ostringstream oss; // 0x00
    int16_t pairs{}; // 0x108
};
