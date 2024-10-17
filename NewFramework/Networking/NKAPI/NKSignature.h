#pragma once
#include <cstdint>
#include <string>

// TODO: implement Verify
namespace NKSignature
{
    std::string Generate(const std::string& accessToken, const std::string& privateKey,
                         const std::string& data, const std::string& nonce);
    std::string GenerateNonce(uint64_t managerTime, uint64_t deviceBootTime);
    std::string Verify(const std::string& a1, const std::string& a2, const std::string& a3, const std::string& a4);
}
