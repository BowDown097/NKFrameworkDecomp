#ifndef FILEPOLICY_H
#define FILEPOLICY_H
#include <string>
#include <vector>

enum class eFileLocation { Documents, Cache, Unknown1, Assets, Unknown2, External };

class CFilePolicy
{
public:
    enum class ePolicyItemType { Unknown, File, Directory };

    struct SPolicyItem
    {
        eFileLocation location;
        std::string path;
        ePolicyItemType type;
    };

    bool hasWritePermission = true;
    std::string policyName = "Unnamed";

    void AddLocation(eFileLocation location);
    void AddPath(ePolicyItemType itemType, const std::string& path);
    void ClearPolicyItems();
    std::vector<SPolicyItem> GetPolicyItems() const { return policyItems; }
    void SetPolicyName(const std::string& policyName) { this->policyName = policyName; }
    void UpdateWritePermissions();
private:
    std::vector<SPolicyItem> policyItems;
};

#endif // FILEPOLICY_H
