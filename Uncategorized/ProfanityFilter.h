#pragma once
#include <cstdint>
#include <map>
#include <string>
#include <vector>

class CProfanityFilterConfig {
public:
    std::vector<std::string> whitelist;
    std::vector<std::string> blacklist;
    std::string regexTemplate;
    std::vector<std::string> spacingWildcards;
    std::map<char, std::string> charWildcards;

    void LoadDefaults();
};

class CProfanityFilter {
public:
    static bool Match(const CProfanityFilterConfig& config, const std::string& str);
    static std::vector<std::pair<uint64_t, uint64_t>> Run(
        const CProfanityFilterConfig& config, const std::string& str);
    static std::string GetCensored(CProfanityFilterConfig* config, const std::string& str);
    static void Censor(std::string& out, const CProfanityFilterConfig& config, char replace = '*');
private:
    static std::vector<std::pair<uint64_t, uint64_t>> applyBlacklist(
        const std::string& str,
        const std::vector<std::pair<uint64_t, uint64_t>>& ranges,
        const CProfanityFilterConfig& config);
    static std::vector<std::pair<uint64_t, uint64_t>> applyWhitelist(
        const std::string& str, const std::vector<std::string>& whitelist);
    static std::vector<std::pair<uint64_t, uint64_t>> stripWhitelistTerm(
        const std::pair<uint64_t, uint64_t>& range, std::string str, const std::string& term);
};
