#include "URLParser.h"
#include <algorithm>

URLParser::URLParser(const std::string& url)
{
    auto queryIter = std::find(url.begin(), url.end(), '?');
    while (queryIter != url.end())
    {
        auto nextQueryIter = std::find(queryIter + 1, url.end(), '&');
        std::string pair(queryIter, nextQueryIter);

        size_t equalsIndex = pair.find('=');
        if (equalsIndex != std::string::npos)
        {
            std::string key = pair.substr(0, equalsIndex);
            std::string value = pair.substr(equalsIndex + 1);
            if (!key.empty())
                valueMap.emplace(key, value);
        }

        queryIter = nextQueryIter;
    }
}

bool URLParser::GetValue(const std::string& key, std::string& out)
{
    auto it = valueMap.find(key);
    if (it != valueMap.end())
        out = it->second;
    return it != valueMap.end();
}
