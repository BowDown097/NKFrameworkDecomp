#include "Version.h"
#include "StringHelper.h"

CVersion::CVersion(int major, int minor, int patch)
    : major(major), minor(minor), patch(patch),
      versionCode(ConstructIntVersion()),
      versionString(ConstructStrVersion()) {}

CVersion::CVersion(int versionCode)
{
    SetVersion(versionCode);
}

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

std::string CVersion::ConstructStrVersionMajorMinor() const
{
    return StringHelper::Format("%d.%d", major, minor);
}

bool CVersion::IsEquivalent(const CVersion& version, bool checkMinor, bool checkPatch) const
{
    return major == version.major &&
           (!checkMinor || minor == version.minor) &&
           (!checkPatch || patch == version.patch);
}

void CVersion::SetVersion(int versionCode)
{
    this->versionCode = versionCode;
    major = (0x68db8badLL * versionCode < 0) + ((0x68db8badLL * versionCode) >> 44);

    int a = -10000 * major + versionCode;
    minor = a / 100;
    patch = a % 100;

    versionString = ConstructStrVersion();
}
