#ifndef VERSION_H
#define VERSION_H
#include <cstdint>
#include <string>

class CVersion
{
public:
    int8_t major = 0;
    int8_t minor = 0;
    int8_t patch = 0;
    int versionCode = 0;
    std::string versionString;

    CVersion(int major, int minor, int patch);
    CVersion(int versionCode);
    int ConstructIntVersion() const;
    std::string ConstructStrVersion() const;
    void SetVersion(int versionCode);
    std::string ConstructStrVersionMajorMinor() const;
    bool IsEquivalent(const CVersion& version, bool checkMinor, bool checkPatch) const;
};

#endif // VERSION_H
