#pragma once
#include <cstdint>
#include <string>
#include <vector>

struct NKMessageSession
{
    std::string token; // 0x00
    time_t expires; // 0x18
};

struct NKAccessToken
{
    std::string token; // 0x00
    time_t expires; // 0x18

    NKAccessToken();
    NKAccessToken(const NKMessageSession& session)
        : token(session.token), expires(session.expires) {}
};

struct NKMessageAuth
{
    std::string session; // 0x00
    std::string device; // 0x18
    std::uint32_t appID; // 0x30
    std::uint32_t skuID; // 0x34
};

struct NKMessage
{
    NKMessageAuth auth; // 0x00
    std::string data; // 0x38
    std::string sig; // 0x50
    std::string nonce; // 0x68
};

struct NKResponseUser
{
    std::string field_0; // 0x00
    std::string field_18; // 0x18
    std::string field_30; // 0x30
    void* field_48; // 0x48
    std::string field_50; // 0x50
    std::string field_68; // 0x68
    char field_80[16]; // 0x80
    void* field_90; // 0x90
    std::vector<std::string> field_98; // 0x98
    char field_B0[16]; // 0xB0
};

struct NKResponseUtilityTime
{
    time_t time;
};
