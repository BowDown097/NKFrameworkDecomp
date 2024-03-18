#include "FilePolicy.h"

void CFilePolicy::AddLocation(eFileLocation location)
{
    policyItems.insert(policyItems.begin(), SPolicyItem { .type = ePolicyItemType::File, .location = location });
    UpdateWritePermissions();
}

void CFilePolicy::UpdateWritePermissions()
{
    if (policyItems.empty())
    {
        hasWritePermission = true;
        return;
    }

    for (const SPolicyItem& policyItem : policyItems)
    {
        if (policyItem.type == ePolicyItemType::Archive)
            break;

        if (policyItem.type == ePolicyItemType::File && policyItem.location == eFileLocation::Assets)
            hasWritePermission = false;
    }
}

void CFilePolicy::AddPath(ePolicyItemType type, const std::string& path)
{
    std::string pathCopy(path);
    if (type == ePolicyItemType::Directory && !pathCopy.empty() && pathCopy.back() != '/')
        pathCopy.push_back('/');

    policyItems.insert(policyItems.begin(), SPolicyItem { .type = type, .path = path });
    UpdateWritePermissions();
}

void CFilePolicy::ClearPolicyItems()
{
    policyItems.clear();
    UpdateWritePermissions();
}
