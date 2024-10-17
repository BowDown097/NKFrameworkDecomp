#pragma once
#include "IHttpCallback.h"

// TODO: check for and add fields, if there are any
struct IHttpRequestManager
{
    virtual ~IHttpRequestManager() = default;
    virtual void SetUserAgent(const std::string& userAgent) = 0;
    virtual void SetProxyOverride(const std::string& ip, const int& port) = 0;
    virtual void Process() = 0;
    virtual int Send(const SHttpRequest& req, IHttpCallback* cb) = 0;
};
