#pragma once
#include <boost/shared_ptr.hpp>
#include <curl/curl.h>
#include <map>

enum CONTENT_TYPE { TYPE_URLENCODED, TYPE_ATOMXML, TYPE_JSON, TYPE_PLAIN };
enum HTTP_METHOD { METHOD_GET, METHOD_POST, METHOD_HEAD, METHOD_UNKNOWN };

struct CurlData
{
    CURL* curl;
    curl_slist* slist;
    CURLcode error;
};

struct SHttpTimeoutOptions
{
    long connectTimeout; // 0x00
    long timeout; // 0x08
    long lowSpeedLimit; // 0x10
    long lowSpeedTime; // 0x18
};

// TODO: come up with names for field_58, field_88, field_A8, field_B0, and field_B8
struct SHttpRequest
{
    std::string userAgent; // 0x00
    std::string callbackKey; // 0x18
    std::string url; // 0x30
    HTTP_METHOD method{}; // 0x48
    CONTENT_TYPE contentType{}; // 0x4C
    bool writeToFile{}; // 0x50
    long field_58 = -1; // 0x58
    SHttpTimeoutOptions timeoutOptions{}; // 0x60
    bool failOnError{}; // 0x80
    void* field_88{}; // 0x88
    std::map<std::string, std::string> requestHeaders; // 0x90
    int field_A8{}; // 0xA8
    void* field_B0{}; // 0xB0
    int field_B8{}; // 0xB8
    long statusCode{}; // 0xC0
    int error{}; // 0xC8
    std::vector<char> responseData; // 0xD0
    std::map<std::string, std::string> responseHeaders; // 0xE8
    std::string requestData; // 0x100
    boost::shared_ptr<CurlData> curl; // 0x118

    std::string GetDownloadedDataStr();
    std::string GetResponseHeaderBase64();
    std::string GetErrorString();
};
