#include "NKEndpoints.h"
#include "NewFramework/Networking/Protocols/HTTP/IHttpRequestManager.h"
#include "Uncategorized/StuffToRefactor.h"

namespace NKEndpoints
{
    int sendMessage(const std::string& endpoint, std::map<std::string, std::string>& headers,
                    const std::string& data, const std::string& callbackKey, HTTP_METHOD method,
                    IHttpCallback* cb, SHttpTimeoutOptions timeoutOptions)
    {
        if (!STUFF_TO_REFACTOR::s_HttpRequestMgr)
            return 0;

        SHttpRequest req;
        req.callbackKey = callbackKey;
        req.contentType = TYPE_JSON;
        req.method = method;
        req.requestData = data;
        req.requestHeaders = headers;
        req.url = endpoint;
        req.failOnError = false;
        req.field_58 = 60;

        if (!timeoutOptions.connectTimeout)
            timeoutOptions.connectTimeout = 15;
        if (!timeoutOptions.lowSpeedLimit)
            timeoutOptions.lowSpeedLimit = 1024;
        if (!timeoutOptions.lowSpeedTime)
            timeoutOptions.lowSpeedTime = 10;
        req.timeoutOptions = timeoutOptions;

        return STUFF_TO_REFACTOR::s_HttpRequestMgr->Send(req, cb);
    }
}
