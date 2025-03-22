#include "HttpCallbackFunctor.h"

void HttpCallbackFunctor::HttpComplete(const SHttpRequest& req)
{
    if (httpCallback)
        httpCallback(req);
    field_40.reset();
}

void HttpCallbackFunctor::HttpFailed(const SHttpRequest& req)
{
    if (httpCallback)
        httpCallback(req);
    field_40.reset();
}
