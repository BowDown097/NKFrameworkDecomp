#pragma once
#include "Uncategorized/types.h"
#include <string>

class CVersion {
public:
    uchar major{}; // 0x00
    uchar minor{}; // 0x01
    uchar patch{}; // 0x02
    uint versionCode; // 0x04
    std::string versionString; // 0x08

    CVersion(int major, int minor, int patch);
    int ConstructIntVersion() const;
    std::string ConstructStrVersion() const;
    CVersion(int versionCode);
    void SetVersion(int versionCode);
    std::string ConstructStrVersionMajorMinor() const;
    bool IsEquivalent(const CVersion& version, bool checkPatch, bool checkMinor) const;
};
