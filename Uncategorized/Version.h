#pragma once
#include <cstdint>
#include <string>

class CVersion
{
public:
    int8_t major{};
    int8_t minor{};
    int8_t patch{};
    int versionCode{};
    std::string versionString;

    CVersion();
    CVersion(int major, int minor, int patch);
    int ConstructIntVersion() const;
    std::string ConstructStrVersion() const;
    CVersion(int versionCode);
    void SetVersion(int versionCode);
    std::string ConstructStrVersionMajorMinor() const;
    bool IsEquivalent(const CVersion& version, bool checkMinor, bool checkPatch) const;
};
