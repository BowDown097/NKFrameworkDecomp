#pragma once
#include <map>
#include <string>

class URLParser
{
public:
    URLParser(const std::string& url);
    bool GetValue(const std::string& key, std::string& out);
private:
    std::map<std::string, std::string> valueMap;
};
