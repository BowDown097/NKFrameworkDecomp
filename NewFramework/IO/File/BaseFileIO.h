#ifndef BASEFILEIO_H
#define BASEFILEIO_H
#include "FilePolicy.h"
#include "IFile.h"
#include <map>

class C_ZipArchive;

class CBaseFileIO
{
public:
    class CFile;

    CBaseFileIO();
    CFilePolicy* GetPolicyByName(const std::string& name) const;
    std::string GetPath(eFileLocation location, const std::string& fileName) const;
    std::string GetStorageDirectory(eFileLocation location) const;
    void AddSupportedStorage(eFileLocation location, const std::string& path);
    bool IsStorageSupported(eFileLocation location) const;
    C_ZipArchive* LoadArchive(const std::string& path, int, eFileOpenMode openMode); // UNIMPLEMENTED
    IFile* OpenFile(const std::string& path, const CFilePolicy& policy, eFileOpenMode openMode);
    IFile* OpenFileFromArchive(const std::string& archive, const std::string& file, std::string& error); // UNIMPLEMENTED, returns some other file type
    CBaseFileIO::CFile* OpenFileFromPath(const std::string& path, eFileOpenMode openMode, std::string& error);
    bool FileExists(const std::string& path, const CFilePolicy& policy);
    bool DoesFileExistInArchive(const std::string& archive, const std::string& file) const; // UNIMPLEMENTED
    bool DoesFileExistAtPath(const std::string& path);
    bool FileDelete(const std::string& path, const CFilePolicy& policy) const;
    bool DeleteFileFromPath(const std::string& path) const;
    bool FileCopy(const std::string& source, const CFilePolicy& sourcePolicy,
                  const std::string& dest, const CFilePolicy& destPolicy,
                  IFile::eWriteSyncBehaviour writeSyncBehaviour);
    bool FileMove(const std::string& source, const CFilePolicy& sourcePolicy,
                  const std::string& dest, const CFilePolicy& destPolicy,
                  IFile::eWriteSyncBehaviour writeSyncBehaviour);
    bool FileRename(const std::string& source, const std::string& dest, const CFilePolicy& policy);
    int RenameFileAtPath(const std::string& source, const std::string& dest);
    std::vector<std::string> FilesInDirectory(const std::string& dir, const CFilePolicy& policy, const std::string& extension);
    void FilesAndFoldersInDirectory(const std::string& dir, const CFilePolicy& policy, std::vector<std::string>* files,
                                    const std::string& extension, std::vector<std::string>* folders);
    std::vector<std::string> FoldersInDirectory(const std::string& dir, const CFilePolicy& policy);
    bool FileNamesInArchive(const std::string&, const std::string&, const std::string&,
                            std::vector<std::string>*, std::vector<std::string>*); // UNIMPLEMENTED
    bool FileNamesInPath(const std::string& dir, const std::string& extension, std::vector<std::string>* files,
                         std::vector<std::string>* folders);
protected:
    virtual void ValidateFilePath(const std::string& path);
    virtual uint64_t GetFreeDiskSpace(const std::string& path) const;
    virtual const bool CreateDirectoryForPath(const std::string& path);
private:
    std::map<eFileLocation, std::string> storageMap; // 0x08
    CFilePolicy assetPolicy; // 0x20
    CFilePolicy documentPolicy; // 0x58
    CFilePolicy cachePolicy; // 0x90
    CFilePolicy externalPolicy; // 0xC8
    std::map<std::string, CFilePolicy*> policyMap; // 0x100
    std::string archivePassword; // 0x118
};

#endif // BASEFILEIO_H
