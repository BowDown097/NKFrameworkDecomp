#pragma once
#include "IHttpCallback.h"

struct HttpCallbackFunctor : IHttpCallback
{
    std::function<void(const SHttpRequest&)> httpCallback; // 0x08
    std::shared_ptr<HttpCallbackFunctor> field_38; // 0x38

    void HttpComplete(const SHttpRequest& req) override;
    void HttpFailed(const SHttpRequest& req) override;
};
