#pragma once
#include <string>
#include <vector>

enum class eFileLocation { Documents, Cache, Unk0, Assets, Unk1, External };

class CFilePolicy
{
public:
    enum class ePolicyItemType { Archive, File, Directory };

    struct SPolicyItem
    {
        ePolicyItemType type; // 0x00
        eFileLocation location; // 0x04
        std::string path; // 0x08

        friend bool operator==(const SPolicyItem& lhs, const SPolicyItem& rhs)
        { return lhs.location == rhs.location && lhs.path == rhs.path && lhs.type == rhs.type; }
    };

    std::string policyName = "Unnamed"; // 0x00
    std::vector<SPolicyItem> policyItems; // 0x18
    bool hasWritePermission = true; // 0x30

    void AddLocation(eFileLocation location);
    void UpdateWritePermissions();
    void AddPath(ePolicyItemType itemType, const std::string& path);
    void ClearPolicyItems();
    std::vector<SPolicyItem> GetPolicyItems() const { return policyItems; }
    void SetPolicyName(const std::string& policyName) { this->policyName = policyName; }
};
