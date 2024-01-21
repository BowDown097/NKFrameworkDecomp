#ifndef VERSION_H
#define VERSION_H
#include <string>

class CVersion
{
public:
    int versionCode = 0;
    std::string versionString;

    CVersion(int major, int minor, int patch);
    CVersion(int versionCode);

    int ConstructIntVersion() const;
    std::string ConstructStrVersion() const;
    std::string ConstructStrVersionMajorMinor() const;
    bool IsEquivalent(const CVersion& version, bool checkMinor, bool checkPatch) const;
    void SetVersion(int versionCode);
private:
    int major = 0;
    int minor = 0;
    int patch = 0;
};

#endif // VERSION_H
