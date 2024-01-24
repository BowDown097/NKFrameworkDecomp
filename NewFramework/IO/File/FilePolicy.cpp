#include "FilePolicy.h"

void CFilePolicy::AddLocation(eFileLocation location)
{
    policyItems.insert(policyItems.begin(), SPolicyItem { .location = location, .type = ePolicyItemType::File });
    UpdateWritePermissions();
}

void CFilePolicy::AddPath(ePolicyItemType type, const std::string& path)
{
    std::string pathCopy(path);
    if (type == ePolicyItemType::Directory && !pathCopy.empty() && pathCopy.back() != '/')
        pathCopy.push_back('/');

    policyItems.insert(policyItems.begin(), SPolicyItem {
        .path = path,
        .type = type
    });

    UpdateWritePermissions();
}

void CFilePolicy::ClearPolicyItems()
{
    policyItems.clear();
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
