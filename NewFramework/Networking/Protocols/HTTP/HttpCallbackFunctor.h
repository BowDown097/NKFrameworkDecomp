#pragma once
#include "IHttpCallback.h"

// i'm really conflicted on whether this field_8 actually exists.
// the destructor of this class suggests so - or at least, that field_40 is actually at 0x40.
struct HttpCallbackFunctor : IHttpCallback
{
    void* field_8; // 0x08
    std::function<void(const SHttpRequest&)> httpCallback; // 0x10
    std::shared_ptr<HttpCallbackFunctor> field_40; // 0x40

    void HttpComplete(const SHttpRequest& req) override;
    void HttpFailed(const SHttpRequest& req) override;
};
