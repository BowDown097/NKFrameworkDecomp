#include "NewFramework/Platform/iOS/File.h" // TODO: change this #include based on platform when that's done
#include "NewFramework/Platform/Shared/Core.h"
#include "NewFramework/Platform/Shared/Logging.h"
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>

#ifndef NDEBUG
#include "NewFramework/Utilities/StringHelper.h"
#endif

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

CFilePolicy* CBaseFileIO::GetPolicyByName(const std::string& name) const
{
    auto it = policyMap.find(name);
    return it != policyMap.end() ? it->second : nullptr;
}

std::string CBaseFileIO::GetPath(eFileLocation location, const std::string& fileName) const
{
    return GetStorageDirectory(location) + fileName;
}

std::string CBaseFileIO::GetStorageDirectory(eFileLocation location) const
{
    auto it = storageMap.find(location);
    return it != storageMap.end() ? it->second : std::string();
}

void CBaseFileIO::AddSupportedStorage(eFileLocation location, const std::string& path)
{
    auto it = storageMap.find(location);
    if (it == storageMap.end())
        storageMap.emplace(location, path + '/');
}

bool CBaseFileIO::IsStorageSupported(eFileLocation location) const
{
    return storageMap.find(location) != storageMap.end();
}

IFile* CBaseFileIO::OpenFile(const std::string& path, const CFilePolicy& policy, eFileOpenMode openMode)
{
    bool writeMode = openMode == eFileOpenMode::Write || openMode == eFileOpenMode::ReadWriteNew ||
                     openMode == eFileOpenMode::ReadWriteExisting || openMode == eFileOpenMode::Append;
    if (writeMode && !policy.hasWritePermission)
    {
        LOG_ERROR(
            "Attempted to open file (%s) in write mode using %s policy which contains non-writable locations or archives",
            path.c_str(), policy.policyName.c_str()
            );
        return nullptr;
    }

    std::vector<CFilePolicy::SPolicyItem> policyItems = policy.GetPolicyItems();
    if (policyItems.empty())
    {
        LOG_ERROR("Error loading file (%s) using %s policy.", path.c_str(), policy.policyName.c_str());
        return nullptr;
    }

    std::vector<std::string> errors;
    IFile* file = nullptr;
    for (const CFilePolicy::SPolicyItem& policyItem : policyItems)
    {
        std::string error;
        switch (policyItem.type)
        {
        case CFilePolicy::ePolicyItemType::Archive:
            file = OpenFileFromArchive(path, policyItem.path, error);
            break;
        case CFilePolicy::ePolicyItemType::File:
            file = OpenFileFromPath(GetPath(policyItem.location, path), openMode, error);
            break;
        case CFilePolicy::ePolicyItemType::Directory:
            file = OpenFileFromPath(policyItem.path + path, openMode, error);
            break;
        }

        if (!file)
            errors.push_back(error);
    }

#ifndef NDEBUG // speculative
    if (!errors.empty())
        LOG_ERROR("OpenFile error: %s", StringHelper::Join(errors, ", ").c_str());
#endif

    if (!file)
        LOG_ERROR("Error loading file (%s) using %s policy.", path.c_str(), policy.policyName.c_str());

    return file;
}

CBaseFileIO::CFile* CBaseFileIO::OpenFileFromPath(const std::string& path, eFileOpenMode openMode, std::string& error)
{
    if (openMode == eFileOpenMode::Append || openMode == eFileOpenMode::ReadWriteNew)
    {
        for (int i = path.size() - 1; i >= 0; i--)
        {
            if (memchr("\\/", path[i], 2))
            {
                std::string pathSegment(path, 0, i);
                if (!CreateDirectoryForPath(pathSegment))
                    return nullptr;
                break;
            }
        }
    }

    CBaseFileIO::CFile* result = new CBaseFileIO::CFile;
    if (result->Open(path, openMode, error))
        return result;
    delete result;
    return nullptr;
}

bool CBaseFileIO::FileExists(const std::string& path, const CFilePolicy& policy)
{
    std::vector<CFilePolicy::SPolicyItem> policyItems = policy.GetPolicyItems();
    for (const CFilePolicy::SPolicyItem& policyItem : policyItems)
    {
        bool existsAtPath = false;
        switch (policyItem.type)
        {
        case CFilePolicy::ePolicyItemType::Archive:
            existsAtPath = DoesFileExistInArchive(path, policyItem.path);
            break;
        case CFilePolicy::ePolicyItemType::File:
            existsAtPath = DoesFileExistAtPath(GetPath(policyItem.location, path));
            break;
        case CFilePolicy::ePolicyItemType::Directory:
            existsAtPath = DoesFileExistAtPath(policyItem.path + path);
            break;
        }

        if (existsAtPath)
            return true;
    }

    return false;
}

bool CBaseFileIO::DoesFileExistAtPath(const std::string& path)
{
    std::string ignore;
    IFile* file = OpenFileFromPath(path, eFileOpenMode::Read, ignore);
    bool result = file != NULL;

    delete file;
    return result;
}

// WTF NinjaKiwi!!!!!
// This returns true even if nothing is done, as long as the policy has write permissions.
// I like triple checked just to make sure, not a disassembly problem I'm pretty positive.
// And it still just returns true if there's no policy items.
bool CBaseFileIO::FileDelete(const std::string& path, const CFilePolicy& policy) const
{
    if (!policy.hasWritePermission)
    {
        LOG_ERROR(
            "Attempted to delete file (%s) using %s policy which contains non-writable locations or archives",
            path.c_str(), policy.policyName.c_str()
            );
        return false;
    }

    std::vector<CFilePolicy::SPolicyItem> policyItems = policy.GetPolicyItems();
    for (const CFilePolicy::SPolicyItem& policyItem : policyItems)
    {
        std::string fullPath = policyItem.type == CFilePolicy::ePolicyItemType::File
                                   ? GetPath(policyItem.location, path) : policyItem.path + path;
        remove(fullPath.c_str());
    }

    return true;
}

bool CBaseFileIO::DeleteFileFromPath(const std::string& path) const
{
    return remove(path.c_str()) != 0;
}

bool CBaseFileIO::FileCopy(const std::string& source, const CFilePolicy& sourcePolicy,
                           const std::string& dest, const CFilePolicy& destPolicy,
                           IFile::eWriteSyncBehaviour writeSyncBehaviour)
{
    ValidateFilePath(dest);
    if (!destPolicy.hasWritePermission)
    {
        LOG_ERROR(
            "Attempted to copy file (%s) to (%s) using %s policy which contains non-writeable locations or archives",
            source.c_str(), dest.c_str(), destPolicy.policyName.c_str()
        );
        return false;
    }

    IFile* sourceObject = OpenFile(source, sourcePolicy, eFileOpenMode::Read);
    if (!sourceObject)
    {
        LOG_ERROR("Cannot open source file (%s) for copying", source.c_str());
        return false;
    }

    IFile* destObject = OpenFile(dest, destPolicy, eFileOpenMode::ReadWriteNew);
    if (!destObject)
    {
        LOG_ERROR("Cannot open destination file (%s) for copying", dest.c_str());
        delete sourceObject;
        return false;
    }

    uint64_t sourceLength = sourceObject->GetFileLength();
    uint64_t readBytes = 0, writeBytes = 0, writeTotal = 0;
    do
    {
        if (writeTotal >= sourceLength)
            break;

        uint8_t readWriteBuf[16384];
        readBytes = sourceObject->ReadBytes(readWriteBuf, sizeof(readWriteBuf));
        if (readBytes == 0)
            break;

        writeBytes = destObject->WriteBytes(readWriteBuf, readBytes, writeSyncBehaviour);
        writeTotal += writeBytes;
    } while (writeBytes == readBytes);

    if (writeTotal != sourceLength)
        FileDelete(dest, destPolicy);

    sourceObject->Close(IFile::eWriteSyncBehaviour::NoSync);
    destObject->Close(IFile::eWriteSyncBehaviour::NoSync);

    delete sourceObject;
    delete destObject;

    return writeTotal == sourceLength;
}

bool CBaseFileIO::FileMove(const std::string& source, const CFilePolicy& sourcePolicy,
                           const std::string& dest, const CFilePolicy& destPolicy,
                           IFile::eWriteSyncBehaviour writeSyncBehaviour)
{
    bool result = FileCopy(source, sourcePolicy, dest, destPolicy, writeSyncBehaviour);
    if (result)
        FileDelete(source, sourcePolicy);
    return result;
}

bool CBaseFileIO::FileRename(const std::string& source, const std::string& dest, const CFilePolicy& policy)
{
    ValidateFilePath(dest);
    if (!policy.hasWritePermission)
    {
        LOG_ERROR(
            "Attempted to rename file (%s) using %s policy which contains non-writable locations or archives",
            source.c_str(), policy.policyName.c_str()
            );
        return false;
    }

    std::vector<CFilePolicy::SPolicyItem> policyItems = policy.GetPolicyItems();
    for (const CFilePolicy::SPolicyItem& policyItem : policyItems)
    {
        switch (policyItem.type)
        {
        case CFilePolicy::ePolicyItemType::Directory:
            RenameFileAtPath(policyItem.path + source, policyItem.path + dest);
            break;
        case CFilePolicy::ePolicyItemType::File:
            RenameFileAtPath(GetPath(policyItem.location, source), GetPath(policyItems[0].location, dest));
            break;
        default:
            break;
        }
    }

    return true;
}

int CBaseFileIO::RenameFileAtPath(const std::string& source, const std::string& dest)
{
    return rename(source.c_str(), dest.c_str());
}

std::vector<std::string> CBaseFileIO::FilesInDirectory(const std::string& dir, const CFilePolicy& policy, const std::string& extension)
{
    std::vector<std::string> out;
    FilesAndFoldersInDirectory(dir, policy, &out, extension, nullptr);
    return out;
}

void CBaseFileIO::FilesAndFoldersInDirectory(const std::string& dir, const CFilePolicy& policy, std::vector<std::string>* files,
                                             const std::string& extension, std::vector<std::string>* folders)
{
    std::vector<CFilePolicy::SPolicyItem> policyItems = policy.GetPolicyItems();
    int successes = 0;
    for (const CFilePolicy::SPolicyItem& policyItem : policyItems)
    {
        switch (policyItem.type)
        {
        case CFilePolicy::ePolicyItemType::Archive:
            successes += FileNamesInArchive(dir, policyItem.path, extension, files, folders);
            break;
        case CFilePolicy::ePolicyItemType::File:
            successes += FileNamesInPath(GetPath(policyItem.location, dir), extension, files, folders);
            break;
        case CFilePolicy::ePolicyItemType::Directory:
            successes += FileNamesInPath(policyItem.path + dir, extension, files, folders);
            break;
        }
    }

    // TODO: finish this duplicate entry checker.
    // the decompilation from here on is extremely hard to decipher.
    // i will come back to this later.
    std::vector<std::vector<std::string>*> filesAndFolders;
    if (files)
        filesAndFolders.push_back(files);
    if (folders)
        filesAndFolders.push_back(folders);
}

std::vector<std::string> CBaseFileIO::FoldersInDirectory(const std::string& dir, const CFilePolicy& policy)
{
    std::vector<std::string> out;
    FilesAndFoldersInDirectory(dir, policy, nullptr, "", &out);
    return out;
}

bool CBaseFileIO::FileNamesInPath(const std::string& dir, const std::string& extension,
                                  std::vector<std::string>* files, std::vector<std::string>* folders)
{
    std::string dirCopy(dir);
    if (!dirCopy.empty() && dirCopy.back() != '/')
        dirCopy.push_back('/');

    DIR* dirObj = opendir(dirCopy.c_str());
    if (!dirObj)
        return false;

    bool result = false;
    while (true)
    {
        struct dirent* dirent = readdir(dirObj);
        if (!dirent)
        {
            closedir(dirObj);
            break;
        }

        std::string innerDir = dirCopy + dirent->d_name;
        struct stat innerStat;
        if (stat(innerDir.c_str(), &innerStat) != 0)
        {
#ifndef NDEBUG // speculative
            LOG_ERROR("stat() failed on file (%s): %s", innerDir.c_str(), CCore::ThreadSafeStrError(errno).c_str());
#endif
            continue;
        }

        std::string dName = dirent->d_name;
        if (S_ISREG(innerStat.st_mode))
        {
            if (files && dName.size() > extension.size())
            {
                std::string dNameExt(dName, dName.size() - extension.size());
                if (dNameExt == extension)
                {
                    result = true;
                    files->push_back(dName);
                }
            }
        }
        else if (S_ISDIR(innerStat.st_mode) && folders && dName != "." && dName != "..")
        {
            folders->push_back(dName);
        }
    }

    return result;
}
