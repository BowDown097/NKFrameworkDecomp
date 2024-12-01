#pragma once
#include <cstdint>
#include <json_spirit/json_spirit_value.h>

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
    std::uint32_t appID; // 0x30
    std::uint32_t skuID; // 0x34
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

struct NKMessage
{
    NKMessageAuth auth; // 0x00
    std::string data; // 0x38
    std::string sig; // 0x50
    std::string nonce; // 0x68
};

struct NKResponseLink
{

};

struct NKResponseUser
{
    std::string nkapiID; // 0x00
    std::string shortcode; // 0x18
    std::string displayName; // 0x30
    std::uint32_t clan; // 0x48
    std::string country; // 0x50
    std::string continent; // 0x68
    std::uint32_t avatar; // 0x80
    int field_84; // 0x84
    bool online; // 0x88
    int field_8C; // 0x8C
    std::uint32_t onlineApp; // 0x90
    std::vector<std::string> providersAvailable; // 0x98
    std::uint32_t access; // 0xB0
    int field_B4; // 0xB4
    int age; // 0xB8
};

struct NKResponseUserCurrent
{
    NKResponseUser user;
};

struct NKResponseLogin
{
    NKResponseUser user;
    NKMessageSession session;
};

struct NKResponseCreate
{
    NKResponseUser user;
};

struct NKResponseUtilityTime
{
    time_t time;
};

// TODO: implement TryParse for NKResponseUser (depends on CProfanityFilter and unimplemented GetXXX methods)
namespace NKJSON
{
    template<typename T>
    const bool TryParse(T& out, const std::string& data);
    template<typename T>
    std::string Serialise(const T& val);

    template<> const bool TryParse(NKResponseCreate& out, const std::string& data);
    template<> const bool TryParse(NKResponseLink& out, const std::string& data);
    template<> const bool TryParse(NKResponseLogin& out, const std::string& data);
    template<> std::string Serialise(const NKMessage& val);
    template<> const bool TryParse(NKMessageResponse& out, const std::string& data);
    template<> const bool TryParse(NKResponseUserCurrent& out, const std::string& data);
    template<> std::string Serialise(const NKResponseLogin& val);

    void Serialise(const NKMessageAuth& val, json_spirit::mObject& obj);
    void Serialise(const NKMessage& val, json_spirit::mObject& obj);
    void Serialise(const NKMessageSession& val, json_spirit::mObject& obj);
    void Serialise(const NKResponseLogin& val, json_spirit::mObject& obj);
    void Serialise(const NKResponseUser& val, json_spirit::mObject& obj);

    const bool TryParse(NKResponseUser& out, const json_spirit::mObject& obj);
    const bool TryParse(NKResponseUserCurrent& out, const json_spirit::mObject& obj);
    const bool TryParse(NKMessageErrorDetails& out, const json_spirit::mObject& obj);
    const bool TryParse(NKMessageError& out, const json_spirit::mObject& obj);
    const bool TryParse(NKMessageResponse& out, const json_spirit::mObject& obj);
    const bool TryParse(NKMessageSession& out, const json_spirit::mObject& obj);
    const bool TryParse(NKResponseLogin& out, const json_spirit::mObject& obj);
    const bool TryParse(NKResponseCreate& out, const json_spirit::mObject& obj);
    const bool TryParse(NKResponseLink& out, const json_spirit::mObject& obj);
}
