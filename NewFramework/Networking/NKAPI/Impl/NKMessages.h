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
    bool field_78{}; // 0x78
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

struct NKResponseUserCurrent
{
    NKResponseUser user; // 0x00
};

struct NKResponseLogin
{
    NKResponseUser user; // 0x00
    NKMessageSession session; // 0xC0
};

struct NKResponseCreate
{
    NKResponseUser user; // 0x00
};

struct NKResponseUtilityTime
{
    time_t time; // 0x00
};

// TODO: implement TryParse for NKResponseUser (depends on CProfanityFilter and unimplemented GetXXX methods)
namespace NKJSON
{
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

    void Serialise(const NKMessageAuth& val, json_spirit::mObject& obj);
    void Serialise(const NKMessage& val, json_spirit::mObject& obj);
    void Serialise(const NKMessageSession& val, json_spirit::mObject& obj);
    void Serialise(const NKResponseLogin& val, json_spirit::mObject& obj);
    void Serialise(const NKResponseUser& val, json_spirit::mObject& obj);
    void Serialise(const NKMessageStorageSaveOptions& val, json_spirit::mObject& obj);
    void Serialise(const NKMessageStorageSave& val, json_spirit::mObject& obj);
    void Serialise(const NKMessageStorageSaveBufferOptions& val, json_spirit::mObject& obj);
    void Serialise(const NKMessageStorageSaveBuffer& val, json_spirit::mObject& obj);
    void Serialise(const NKMessageStorageLoad& val, json_spirit::mObject& obj);

    const bool TryParse(NKResponseUser& out, const json_spirit::mObject& obj);
    const bool TryParse(NKResponseUserCurrent& out, const json_spirit::mObject& obj);
    const bool TryParse(NKMessageErrorDetails& out, const json_spirit::mObject& obj);
    const bool TryParse(NKMessageError& out, const json_spirit::mObject& obj);
    const bool TryParse(NKMessageResponse& out, const json_spirit::mObject& obj);
    const bool TryParse(NKMessageSession& out, const json_spirit::mObject& obj);
    const bool TryParse(NKResponseLogin& out, const json_spirit::mObject& obj);
    const bool TryParse(NKResponseCreate& out, const json_spirit::mObject& obj);
    const bool TryParse(NKResponseLink& out, const json_spirit::mObject& obj);
    const bool TryParse(NKMessageResponseFileStorage& out, const json_spirit::mObject& obj);
    const bool TryParse(NKEndpointFileOptions& out, const json_spirit::mObject& obj);
    const bool TryParse(NKMessageResponseFile& out, const json_spirit::mObject& obj);
}
