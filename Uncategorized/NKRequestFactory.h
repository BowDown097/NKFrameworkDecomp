#pragma once
#include "NewFramework/Networking/Protocols/HTTP/HttpRequest.h"

struct IHttpCallback;

namespace NKRequestFactory {
    int GenerateRequestFullURL(const std::string& endpoint, std::string data, HTTP_METHOD method,
                               IHttpCallback* cb, long timeout, long lowSpeedTime);
    int GenerateRequest(const std::string& endpoint, std::string data, HTTP_METHOD method,
                        IHttpCallback* cb, long timeout, long lowSpeedTime);
    int GenerateRequestPartialURL(const std::string&, std::string, HTTP_METHOD, IHttpCallback*, long, long);
}
