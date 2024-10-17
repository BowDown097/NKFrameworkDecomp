#pragma once
#include "HttpRequest.h"

// TODO: check for and add fields, if there are any
struct IHttpCallback
{
    virtual ~IHttpCallback() = default;
    virtual void HttpComplete(const SHttpRequest& req) = 0;
    virtual void HttpFailed(const SHttpRequest& req) = 0;
};
