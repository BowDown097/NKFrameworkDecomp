#include "Version.h"
#include "NewFramework/Utilities/StringHelper.h"

CVersion::CVersion(int major, int minor, int patch)
    : major(major), minor(minor), patch(patch),
      versionCode(ConstructIntVersion()),
      versionString(ConstructStrVersion()) {}

int CVersion::ConstructIntVersion() const
{
    return patch + 100 * minor + 10000 * major;
}

std::string CVersion::ConstructStrVersion() const
{
    std::string out = ConstructStrVersionMajorMinor();
    if (patch != 0)
        out += StringHelper::Format(".%d", patch);
    return out;
}

CVersion::CVersion(int versionCode)
{
    SetVersion(versionCode);
}

void CVersion::SetVersion(int versionCode)
{
    this->versionCode = versionCode;
    major = versionCode / 10000;
    minor = (versionCode % 10000) / 100;
    patch = versionCode % 100;
    versionString = ConstructStrVersion();
}

std::string CVersion::ConstructStrVersionMajorMinor() const
{
    return StringHelper::Format("%d.%d", major, minor);
}

bool CVersion::IsEquivalent(const CVersion& version, bool checkPatch, bool checkMinor) const
{
    return major == version.major &&
           (!checkMinor || minor == version.minor) &&
           (!checkPatch || patch == version.patch);
}
