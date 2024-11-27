#pragma once
#include "IHttpCallback.h"

struct HttpCallbackFunctor : IHttpCallback
{
    void* field_8; // 0x08
    std::function<void(const SHttpRequest&)> httpCallback; // 0x10
    std::shared_ptr<HttpCallbackFunctor> field_40; // 0x40

    void HttpComplete(const SHttpRequest& req) override;
    void HttpFailed(const SHttpRequest& req) override;
};
