#pragma once
#include "NewFramework/Utilities/JSON/JSONWrapper.h"
#include <cstdint>

struct NKMessageSession
{
    std::string sessionID; // 0x00
    time_t expires; // 0x18
};

struct NKAccessToken
{
    std::string token; // 0x00
    time_t expires; // 0x18

    NKAccessToken() = default;
    NKAccessToken(const NKMessageSession& session)
        : token(session.sessionID), expires(session.expires) {}

    // speculative: avoids repeating code
    inline bool IsValid() const { return !token.empty() && expires; }
};

struct NKMessageAuth
{
    std::string session; // 0x00
    std::string device; // 0x18
    uint32_t appID; // 0x30
    uint32_t skuID; // 0x34
};

struct NKMessageErrorDetails
{
    std::string reason; // 0x00
    std::string fix; // 0x18
};

struct NKMessageError
{
    std::string type; // 0x00
    NKMessageErrorDetails details; // 0x18
};

struct NKMessageResponse
{
    NKMessageError error; // 0x00
    std::string data; // 0x48
    std::string sig; // 0x60
};

struct NKEndpointFileOptions
{
    bool isPublic; // 0x00
    bool compress; // 0x01
    bool decode; // 0x02
    bool reducedRedundancy; // 0x03
};

struct NKMessageResponseFileStorage
{
    std::string filename; // 0x00
    std::string data; // 0x18
    bool success{}; // 0x30
    std::string url; // 0x38
    std::string etag; // 0x50
    NKEndpointFileOptions storageOptions; // 0x68
};

struct NKMessageResponseFile
{
    NKMessageResponseFileStorage storage; // 0x00
    void* field_70; // 0x70
    bool upToDate{}; // 0x78
};

struct NKMessage
{
    NKMessageAuth auth; // 0x00
    std::string data; // 0x38
    std::string sig; // 0x50
    std::string nonce; // 0x68
};

struct NKMessageStorageLoad
{
    std::string filename; // 0x00
    std::string owner; // 0x18
    bool includeBody; // 0x30
};

struct NKMessageStorageSaveOptions
{
    bool isPublic{}; // 0x00
    bool compress{}; // 0x01
    bool decode{}; // 0x02
    bool reducedRedundancy{}; // 0x03
    uint64_t expiresIn{}; // 0x08
    std::string render = "plain"; // 0x10
    std::string contentType = "application/json"; // 0x28
    bool stephen{}; // 0x40
};

struct NKMessageStorageSave
{
    std::string filename; // 0x00
    std::string data; // 0x18
    NKMessageStorageSaveOptions options; // 0x30
};

struct NKMessageStorageSaveBufferOptions
{
    bool isPublic{}; // 0x00
    bool compress{}; // 0x01
    bool decode{}; // 0x02
    bool reducedRedundancy{}; // 0x03
    uint64_t expiresIn{}; // 0x08
    std::string render = "plain"; // 0x10
    std::string contentType = "application/json"; // 0x28
    bool stephen{}; // 0x40
};

struct NKMessageStorageSaveBuffer
{
    std::string filename; // 0x00
    std::string data; // 0x18
    NKMessageStorageSaveBufferOptions options; // 0x30
    std::string storageEngine; // 0x78
    uint64_t maximumBufferDuration; // 0x90
};

struct NKMessageUtilityCountry
{

};

struct NKMessageUtilityEsCreate
{
    std::string index; // 0x00
    std::string payload; // 0x18
};

struct NKMessageUtilityEsUpdate
{
    std::string index; // 0x00
    std::string id; // 0x18
    bool modify; // 0x30
};

struct NKMessageUtilityGeoIP
{
    std::vector<std::string> ips; // 0x00
    bool ownIP; // 0x18
    bool distances; // 0x19
};

struct NKMessageUtilitySearch
{
    std::string hint; // 0x00
    std::string index; // 0x18
    std::string query; // 0x30
    uint64_t limit; // 0x48
    uint64_t offset; // 0x50
    json_spirit::mObject options; // 0x58
};

struct NKMessageUtilitySession
{

};

struct NKMessageUtilityTime
{

};

struct NKResponseLink
{

};

struct NKResponseUser
{
    std::string nkapiID; // 0x00
    std::string shortcode; // 0x18
    std::string displayName; // 0x30
    uint32_t clan; // 0x48
    std::string country; // 0x50
    std::string continent; // 0x68
    uint32_t avatar; // 0x80
    int field_84; // 0x84
    bool online; // 0x88
    int field_8C; // 0x8C
    uint32_t onlineApp; // 0x90
    std::vector<std::string> providersAvailable; // 0x98
    uint32_t access; // 0xB0
    int field_B4; // 0xB4
    int age; // 0xB8

    uint64_t GetCreationTimeStamp() const;
};

struct NKResponseCreate
{
    NKResponseUser user;
};

struct NKResponseLogin
{
    NKResponseUser user; // 0x00
    NKMessageSession session; // 0xC0
};

struct NKResponseUserCurrent
{
    NKResponseUser user;
};

struct NKResponseUtilityCountry
{
    std::string country; // 0x00
    bool inEU; // 0x18
};

struct NKResponseUtilityEsCreate
{
    Json::Value document;
};

struct NKResponseUtilityEsUpdate
{
    bool success;
};

struct NKResponseUtilityGeoIPGeoI
{
    std::string country_code; // 0x00
    std::string country_code3; // 0x18
    std::string country_name; // 0x30
    std::string continent_code; // 0x48
    bool found; // 0x60
    std::string ip; // 0x68
    json_spirit::mObject distances; // 0x80
};

struct NKResponseUtilityGeoIP
{
    std::vector<NKResponseUtilityGeoIPGeoI> geoIP; // 0x00
    std::string licence; // 0x18
};

struct NKResponseUtilitySearch
{
    uint64_t total; // 0x00
    uint64_t next; // 0x08
    uint64_t found; // 0x10
    json_spirit::mArray results; // 0x18
};

struct NKResponseUtilitySession
{
    bool valid;
};

struct NKResponseUtilityTimeDate
{
    time_t time;
};

struct NKResponseUtilityTime
{
    NKResponseUtilityTimeDate date;
};

namespace NKJSON
{
    void Serialise(const NKMessageAuth& val, json_spirit::mObject& obj); // 100D5C2CA
    void Serialise(const NKMessage& val, json_spirit::mObject& obj); // 100D5C526
    void Serialise(const NKMessageSession& val, json_spirit::mObject& obj); // 100D5C78C
    void Serialise(const NKResponseLogin& val, json_spirit::mObject& obj); // 100D5CB4E
    void Serialise(const NKResponseUser& val, json_spirit::mObject& obj); // 100D5CD40
    void Serialise(const NKMessageUtilityTime& val, json_spirit::mObject& obj); // 100D63649
    void Serialise(const NKMessageUtilityGeoIP& val, json_spirit::mObject& obj); // 100D63A83
    void Serialise(const NKMessageUtilitySession& val, json_spirit::mObject& obj); // 100D64653
    void Serialise(const NKResponseUtilitySession& val, json_spirit::mObject& obj); // 100D64659
    void Serialise(const NKMessageUtilitySearch& val, json_spirit::mObject& obj); // 100D647F2
    void Serialise(const NKMessageStorageSaveOptions& val, json_spirit::mObject& obj); // 100D64F3C
    void Serialise(const NKMessageStorageSave& val, json_spirit::mObject& obj); // 100D6530C
    void Serialise(const NKMessageStorageSaveBufferOptions& val, json_spirit::mObject& obj); // 100D66EB8
    void Serialise(const NKMessageStorageSaveBuffer& val, json_spirit::mObject& obj); // 100D67288
    void Serialise(const NKMessageStorageLoad& val, json_spirit::mObject& obj); // 100D68D45

    const bool TryParse(NKResponseUser& out, const json_spirit::mObject& obj); // 100D5F114
    const bool TryParse(NKMessageUtilityTime& out, const json_spirit::mObject& obj); // 100D6381D
    const bool TryParse(NKResponseUtilityTimeDate& out, const json_spirit::mObject& obj); // 100D63825
    const bool TryParse(NKResponseUtilityTime& out, const json_spirit::mObject& obj); // 100D638F0
    const bool TryParse(NKMessageUtilityGeoIP& out, const json_spirit::mObject& obj); // 100D63CBC
    const bool TryParse(NKResponseUtilityGeoIPGeoI& out, const json_spirit::mObject& obj); // 100D63DCC
    const bool TryParse(NKResponseUtilityGeoIP& out, const json_spirit::mObject& obj); // 100D6421B
    const bool TryParse(NKMessageUtilitySession& out, const json_spirit::mObject& obj); // 100D6471F
    const bool TryParse(NKResponseUtilitySession& out, const json_spirit::mObject& obj); // 100D64727
    const bool TryParse(NKResponseUtilitySearch& out, const json_spirit::mObject& obj); // 100D64AFF
    const bool TryParse(NKResponseUserCurrent& out, const json_spirit::mObject& obj); // 100D6BB19
    const bool TryParse(NKMessageErrorDetails& out, const json_spirit::mObject& obj); // 100D83352
    const bool TryParse(NKMessageError& out, const json_spirit::mObject& obj); // 100D835DF
    const bool TryParse(NKMessageResponse& out, const json_spirit::mObject& obj); // 100D83873
    const bool TryParse(NKMessageSession& out, const json_spirit::mObject& obj); // 100D83C35
    const bool TryParse(NKResponseLogin& out, const json_spirit::mObject& obj); // 100D83E64
    const bool TryParse(NKResponseCreate& out, const json_spirit::mObject& obj); // 100D840A6
    const bool TryParse(NKResponseLink& out, const json_spirit::mObject& obj); // 100D841F2
    const bool TryParse(NKMessageResponseFileStorage& out, const json_spirit::mObject& obj); // 100D841FA
    const bool TryParse(NKEndpointFileOptions& out, const json_spirit::mObject& obj); // 100D848F1
    const bool TryParse(NKMessageResponseFile& out, const json_spirit::mObject& obj); // 100D84D08

    std::string GetString(const json_spirit::mObject& obj, std::string key);
    std::string GetStringWithDefault(const json_spirit::mObject& obj, std::string key, std::string defaultValue = "");
    bool GetBool(const json_spirit::mObject& obj, std::string key);
    uint64_t GetUInt(const json_spirit::mObject& obj, std::string key);
    int64_t GetInt(const json_spirit::mObject& obj, std::string key);
    json_spirit::mArray GetArray(const json_spirit::mObject& obj, std::string key);
    json_spirit::mObject GetMap(const json_spirit::mObject& obj, std::string key);

    template<typename T>
    const bool TryParse(T& out, const std::string& data)
    {
        if (data.empty())
            return false;

        json_spirit::mValue value;
        CJSONWrapper wrapper(nullptr);
        if (!wrapper.ParseJSONData(reinterpret_cast<uint8_t*>(const_cast<char*>(data.data())), data.size(), value, true))
            return false;

        return TryParse(out, value.get_obj());
    }

    template<typename T>
    std::string Serialise(const T& val)
    {
        json_spirit::mObject obj;
        Serialise(val, obj);
        return json_spirit::write(obj);
    }
}

namespace JSONCPP
{
    bool GetValue(std::string& out, const Json::Value& value);
    bool GetValue(bool& out, const Json::Value& value);
    bool GetValue(uint64_t& out, const Json::Value& value);
    bool GetValue(int64_t& out, const Json::Value& value);
}

namespace NKJSONCPP
{
    void Serialise(const NKMessageUtilityEsCreate& val, Json::Value& obj); // 100D84E57
    void Serialise(const NKMessageUtilityEsUpdate& val, Json::Value& obj); // 100D851B1
    void Serialise(const NKMessageUtilityCountry& val, Json::Value& obj); // 100D862F0

    const bool TryParse(NKResponseUtilityEsCreate& out, const Json::Value& obj); // 100D84EF4
    const bool TryParse(NKResponseUtilityEsUpdate& out, const Json::Value& obj); // 100D8528D
    const bool TryParse(NKResponseUtilityCountry& out, const Json::Value& obj); // 100D88129

    std::string GetString(const Json::Value& value, std::string key);
    std::string GetStringWithDefault(const Json::Value& value, std::string key, std::string defaultValue = "");
    bool GetBool(const Json::Value& value, std::string key);
    uint64_t GetUInt(const Json::Value& value, std::string key);
    int64_t GetInt(const Json::Value& value, std::string key);
    Json::Value GetArray(const Json::Value& value, std::string key);
    Json::Value GetMap(const Json::Value& value, std::string key);

    template<typename T>
    const bool TryParse(T& out, const std::string& data)
    {
        if (data.empty())
            return false;

        Json::Value value;
        CJSONWrapper wrapper(nullptr);
        if (!wrapper.ParseJSONData(reinterpret_cast<uint8_t*>(const_cast<char*>(data.data())), data.size(), value, true))
            return false;

        return TryParse(out, value);
    }

    template<typename T>
    std::string Serialise(const T& val)
    {
        Json::Value obj;
        Serialise(val, obj);

        Json::FastWriter fastWriter;
        return fastWriter.write(obj);
    }
}
