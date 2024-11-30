#pragma once
#include <cstdint>
#include <string>

namespace NKSignature
{
    std::string Generate(const std::string& accessToken, const std::string& privateKey,
                         const std::string& data, const std::string& nonce);
    std::string GenerateNonce(uint64_t managerTime, uint64_t deviceBootTime);
    bool Verify(const std::string& accessToken, const std::string& privateKey, const std::string& data, const std::string& sig);
}
