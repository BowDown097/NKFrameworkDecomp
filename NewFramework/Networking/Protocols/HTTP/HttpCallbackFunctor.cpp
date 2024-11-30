#include "HttpCallbackFunctor.h"

void HttpCallbackFunctor::HttpComplete(const SHttpRequest& req)
{
    httpCallback(req);
    field_40.reset();
}

void HttpCallbackFunctor::HttpFailed(const SHttpRequest& req)
{
    httpCallback(req);
    field_40.reset();
}
