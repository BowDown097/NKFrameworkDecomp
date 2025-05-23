#include "HttpRequest.h"
#include "NewFramework/Utilities/Encoding.h"

std::string SHttpRequest::GetDownloadedDataStr() const
{
    return std::string(responseData.begin(), responseData.end());
}

std::string SHttpRequest::GetResponseHeaderBase64() const
{
    std::string headers;
    for (auto it = responseHeaders.begin(); it != responseHeaders.end(); ++it)
        headers += it->first + "," + it->second + ",";

    std::string encoded;
    if (string_to_base64(headers, encoded))
        return encoded;
    else
        return "FAILED TO TRANSLATE HEADERS";
}

std::string SHttpRequest::GetErrorString() const
{
    return curl_easy_strerror(curl->error);
}
