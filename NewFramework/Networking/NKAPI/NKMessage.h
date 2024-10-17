#pragma once
#include <string>

struct NKAccessToken
{
    std::string token;
};

struct NKMessageAuth
{
    NKAccessToken session;
    std::string device;
    int appID;
    int skuID;
};

struct NKMessage
{
    NKMessageAuth auth;
    std::string data;
    std::string sig;
    std::string nonce;
};
