#include "ProfanityFilter.h"
#include <boost/algorithm/string/replace.hpp>
#include <regex>

void CProfanityFilterConfig::LoadDefaults() {
    const std::string whitelistTerms[] = {
        "abasement",
        "accumulate",
        "accumulation",
        "advertisement",
        "ambassador",
        "amusement",
        "analog",
        "analyse",
        "analysis",
        "analyst",
        "analytic",
        "analyze",
        "analyzing",
        "arsenal",
        "arsenic",
        "assassin",
        "assault",
        "assay",
        "assemble",
        "assemblies",
        "assembly",
        "assert",
        "assess",
        "asset",
        "assign",
        "assist",
        "associate",
        "association",
        "assorted",
        "assortment",
        "assume",
        "assumed",
        "assumes",
        "assuming",
        "assumption",
        "assumptions",
        "assurance",
        "assure",
        "assured",
        "basement",
        "bass",
        "biomass",
        "brass",
        "canal",
        "cass",
        "casserole",
        "cassette",
        "cassettes",
        "cassidy",
        "charset",
        "chassis",
        "circumstance",
        "circumstances",
        "class",
        "coarse",
        "cockburn",
        "cockpit",
        "cocktail",
        "cocktails",
        "cucumber",
        "cumberland",
        "cumbria",
        "cummings",
        "cumulative",
        "dickens",
        "dickinson",
        "dickson",
        "document",
        "documentaries",
        "documentary",
        "documentation",
        "documented",
        "documenting",
        "documents",
        "draper",
        "embarrassing",
        "embassy",
        "endorsement",
        "essex",
        "glass",
        "grape",
        "grass",
        "hancock",
        "harassment",
        "hassle",
        "hitchcock",
        "incumbent",
        "larsen",
        "manuscript",
        "mass",
        "massachusetts",
        "massacre",
        "massage",
        "masses",
        "massey",
        "massive",
        "middlesex",
        "milford",
        "montenegro",
        "nassau",
        "parse",
        "pass",
        "peacock",
        "picasso",
        "potassium",
        "reimbursement",
        "saltwater",
        "sassy",
        "scrap",
        "scunthorpe",
        "shitake",
        "sussex",
        "tallahassee",
        "therapeutic",
        "therapist"
    };

    whitelist.clear();
    for (const std::string& term : whitelistTerms) {
        whitelist.push_back(term);
    }

    const std::string blacklistTerms[] = {
        "anal",
        "anilingus",
        "anus",
        "arse",
        "ass",
        "ballgag",
        "klux",
        "ballkicking",
        "balllicking",
        "ballsack",
        "ballsucking",
        "bangbros",
        "bareback",
        "barelylegal",
        "bastard",
        "beavercleaver",
        "beaverlips",
        "bestiality",
        "bigblack",
        "bimbo",
        "birdlock",
        "bitch",
        "bluewaffle",
        "blumpkin",
        "bollocks",
        "bondage",
        "boner",
        "boob",
        "booty",
        "breasts",
        "bukkake",
        "bunghole",
        "busty",
        "buttcheeks",
        "butthole",
        "cameltoe",
        "carpetmuncher",
        "circlejerk",
        "clevelandsteamer",
        "clit",
        "cock",
        "crap",
        "cum",
        "cunnilingus",
        "cunt",
        "dick",
        "dildo",
        "doggystyle",
        "douche",
        "ejaculation",
        "erotic",
        "faggot",
        "femdom",
        "fingering",
        "fisting",
        "footjob",
        "frotting",
        "fuck",
        "futanari",
        "genitals",
        "girlon",
        "goatse",
        "goldenshower",
        "grope",
        "handjob",
        "hentai",
        "hooker",
        "hotchick",
        "humping",
        "incest",
        "intercourse",
        "jackoff",
        "jailbait",
        "jerkoff",
        "jizz",
        "juggs",
        "kkk",
        "knockers",
        "lemonparty",
        "lolita",
        "masturbate",
        "milf",
        "negro",
        "nigga",
        "nigger",
        "nipple",
        "nude",
        "nudity",
        "orgasm",
        "orgy",
        "paedophile",
        "panties",
        "pedobear",
        "pedophile",
        "penis",
        "pieceofshit",
        "porn",
        "pubes",
        "pussy",
        "queef",
        "raghead",
        "rape",
        "rapist",
        "rectum",
        "rimjob",
        "rimming",
        "semen",
        "sex",
        "shit",
        "slut",
        "sodomize",
        "strapon",
        "strappado",
        "tits",
        "titties",
        "titty",
        "topless",
        "twat",
        "undress",
        "vagina",
        "vibrator",
        "voyeur",
        "vulva",
        "wank",
        "wetdream",
        "yiffy",
        "zoophilia"
    };

    blacklist.clear();
    for (const std::string& term : blacklistTerms) {
        blacklist.push_back(term);
    }

    regexTemplate = "[\\w]*TERM[\\w]*";

    const std::string v14[] = {
        "",
        "([.,\\/#!$%\\^&\\*;:{}=\\-_`~()]|_)*",
        " +"
    };

    spacingWildcards.clear();
    for (const std::string& s : v14) {
        spacingWildcards.push_back(s);
    }

    charWildcards.clear();
    charWildcards.emplace('o', "(\\(\\)|[0o])");
    charWildcards.emplace('i', "([i1!|])");
    charWildcards.emplace('u', "(\\|_\\||u)");
}

bool CProfanityFilter::Match(const CProfanityFilterConfig& config, const std::string& str) {
    return !Run(config, str).empty();
}

std::vector<std::pair<uint64_t, uint64_t>> CProfanityFilter::Run(
    const CProfanityFilterConfig& config, const std::string& str) {

    std::string strLower = str;
    for (int i = 0; i < strLower.size(); ++i) {
        strLower[i] = tolower(static_cast<unsigned char>(strLower[i]));
    }

    return applyBlacklist(strLower, applyWhitelist(strLower, config.whitelist), config);
}

std::string CProfanityFilter::GetCensored(CProfanityFilterConfig* config, const std::string& str) {
    const CProfanityFilterConfig* usingConfig = config;
    std::string out = str;

    if (!config) {
        static CProfanityFilterConfig* s_pFallbackConfig = [] {
            CProfanityFilterConfig* config = new CProfanityFilterConfig;
            config->LoadDefaults();
            return config;
        }();
        usingConfig = s_pFallbackConfig;
    }

    if (usingConfig) {
        Censor(out, *usingConfig);
    }

    return out;
}

void CProfanityFilter::Censor(std::string& out, const CProfanityFilterConfig& config, char replace) {
    std::vector<std::pair<uint64_t, uint64_t>> ranges = Run(config, out);

    for (auto it = ranges.begin(); it != ranges.end(); ++it) {
        for (int i = it->first; i < it->second; ++i) {
            out[i] = replace;
        }
    }
}

std::vector<std::pair<uint64_t, uint64_t>> CProfanityFilter::applyBlacklist(
    const std::string& str,
    const std::vector<std::pair<uint64_t, uint64_t>>& ranges,
    const CProfanityFilterConfig& config) {

    std::vector<std::pair<uint64_t, uint64_t>> out;

    if (config.blacklist.empty()) {
        return out;
    }

    for (const std::string& term : config.blacklist) {
        static std::regex esc("[-\\/\\^$*+?.()|[\\]{}]");
        static std::string rep = "\\\\$&";

        std::string escTerm;
        std::regex_replace(std::back_inserter(escTerm), term.begin(), term.end(), esc, rep);

        for (const std::string& wildcard : config.spacingWildcards) {
            std::string wildcardTerm;
            for (int i = 0; i < escTerm.size(); ++i) {
                wildcardTerm += escTerm[i];
                if (i != escTerm.size() - 1) {
                    wildcardTerm += wildcard;
                }
            }

            for (auto it = config.charWildcards.begin(); it != config.charWildcards.end(); ++it) {
                boost::algorithm::replace_all(wildcardTerm, (char[2]){it->first, '\0'}, it->second);
            }

            std::string regexString = config.regexTemplate;
            boost::algorithm::replace_all(regexString, "TERM", wildcardTerm);

            for (auto it = ranges.begin(); it != ranges.end(); ++it) {
                std::regex regex(regexString);

                std::sregex_iterator sri_begin(str.begin() + it->first, str.begin() + it->second, regex);
                std::sregex_iterator sri_end;

                for (auto regex_it = sri_begin; regex_it != sri_end; ++regex_it) {
                    for (const std::ssub_match& submatch : *regex_it) {
                        out.push_back({it->first + std::distance(str.begin(), submatch.first),
                                       it->first + std::distance(str.begin(), submatch.second)});
                    }
                }
            }
        }
    }

    return out;
}

std::vector<std::pair<uint64_t, uint64_t>> CProfanityFilter::applyWhitelist(
    const std::string& str, const std::vector<std::string>& whitelist) {

    std::vector<std::pair<uint64_t, uint64_t>> out;
    out.push_back({0, str.size()});

    if (whitelist.empty()) {
        return out;
    }

    for (const std::string& term : whitelist) {
        if (!term.empty()) {
            for (int i = 0; i < out.size(); ++i) {
                std::vector<std::pair<uint64_t, uint64_t>> stripped = stripWhitelistTerm(out[i], str, term);

                std::vector<std::pair<uint64_t, uint64_t>> withStripped(out.begin(), out.begin() + i);
                withStripped.insert(withStripped.end(), stripped.begin(), stripped.end());
                withStripped.insert(withStripped.end(), out.begin() + i + 1, out.end());

                out = withStripped;
            }
        }
    }

    return out;
}

std::vector<std::pair<uint64_t, uint64_t>> CProfanityFilter::stripWhitelistTerm(
    const std::pair<uint64_t, uint64_t>& range, std::string str, const std::string& term) {

    std::vector<std::pair<uint64_t, uint64_t>> out;
    std::string substr = str.substr(range.first, range.second - range.first);
    size_t posStart = 0, posEnd;

    if (range.first != range.second) {
        while ((posEnd = substr.find(term, posStart)) != std::string::npos) {
            out.push_back({range.first + posStart, range.first + posEnd});
        }
    }

    out.push_back({range.first + posStart, range.second});
    return out;
}
