#ifndef BASEFILEIO_H
#define BASEFILEIO_H
#include "FilePolicy.h"
#include <map>

enum class eFileOpenMode { Read, Write, ReadWriteNew, ReadWriteExisting, Append };

class IFile;

class CBaseFileIO
{
public:
    CBaseFileIO();
    void AddSupportedStorage(eFileLocation location, const std::string& path);
    std::string GetPath(eFileLocation location, const std::string& fileName) const;
    CFilePolicy* GetPolicyByName(const std::string& name) const;
    std::string GetStorageDirectory(eFileLocation location) const;
    bool IsStorageSupported(eFileLocation location) const;
    void OpenFile(const std::string& path, const CFilePolicy& policy, eFileOpenMode openMode);
private:
    std::string archivePassword;
    CFilePolicy assetPolicy;
    CFilePolicy cachePolicy;
    CFilePolicy documentPolicy;
    CFilePolicy externalPolicy;
    std::map<std::string, CFilePolicy*> policyMap;
    std::map<eFileLocation, std::string> storageMap;
};

#endif // BASEFILEIO_H
