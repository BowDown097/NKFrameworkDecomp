#include "NKSignature.h"
#include "NewFramework/Utilities/Encoding.h"
#include "NewFramework/Utilities/StringHelper.h"

namespace NKSignature
{
    std::string Generate(const std::string& accessToken, const std::string& privateKey,
                         const std::string& data, const std::string& nonce)
    {
        return string_to_md5(accessToken + privateKey + data + nonce);
    }

    std::string GenerateNonce(uint64_t managerTime, uint64_t deviceBootTime)
    {
        static uint64_t request_count;
        return StringHelper::Format("%llu:%llu:%llu", request_count++, managerTime, deviceBootTime);
    }

    bool Verify(const std::string& accessToken, const std::string& privateKey, const std::string& data, const std::string& sig)
    {
        return hmac_sha1(privateKey, data) == sig;
    }
}
