#include "BaseFileIO.h"
#include "NewFramework/Platform/Shared/Logging.h"

CBaseFileIO::CBaseFileIO() : archivePassword("Q%_{6#Px]]")
{
    assetPolicy.SetPolicyName("Asset");
    assetPolicy.AddLocation(eFileLocation::Assets);

    documentPolicy.SetPolicyName("Document");
    documentPolicy.AddLocation(eFileLocation::Documents);

    cachePolicy.SetPolicyName("Cache");
    documentPolicy.AddLocation(eFileLocation::Cache);

    externalPolicy.SetPolicyName("External");
    externalPolicy.AddLocation(eFileLocation::External);

    policyMap.emplace("Asset", &assetPolicy);
    policyMap.emplace("Document", &documentPolicy);
    policyMap.emplace("Cache", &cachePolicy);
    policyMap.emplace("External", &externalPolicy);
}

void CBaseFileIO::AddSupportedStorage(eFileLocation location, const std::string& path)
{
    auto it = storageMap.find(location);
    if (it == storageMap.end())
        storageMap.emplace(location, path + '/');
}

std::string CBaseFileIO::GetPath(eFileLocation location, const std::string& fileName) const
{
    return GetStorageDirectory(location) + fileName;
}

CFilePolicy* CBaseFileIO::GetPolicyByName(const std::string& name) const
{
    auto it = policyMap.find(name);
    return it != policyMap.end() ? it->second : nullptr;
}

std::string CBaseFileIO::GetStorageDirectory(eFileLocation location) const
{
    auto it = storageMap.find(location);
    return it != storageMap.end() ? it->second : std::string();
}

bool CBaseFileIO::IsStorageSupported(eFileLocation location) const
{
    return storageMap.find(location) != storageMap.end();
}

void CBaseFileIO::OpenFile(const std::string& path, const CFilePolicy& policy, eFileOpenMode openMode)
{
    if ((openMode == eFileOpenMode::Append || openMode == eFileOpenMode::ReadWriteNew) && !policy.hasWritePermission)
    {
        LOG_ERROR(
            "Attempted to open file (%s) in write mode using %s policy which contains non-writable locations or archives",
            path.c_str(), policy.policyName.c_str()
        );
        return;
    }
}
