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
    void AddSupportedStorage(eFileLocation location, const std::string& path);
    bool FileCopy(const std::string& source, const CFilePolicy& sourcePolicy,
                  const std::string& dest, const CFilePolicy& destPolicy,
                  IFile::eWriteSyncBehaviour writeSyncBehaviour);
    bool FileDelete(const std::string& path, const CFilePolicy& policy) const;
    bool FileExists(const std::string& path, const CFilePolicy& policy);
    bool FileMove(const std::string& source, const CFilePolicy& sourcePolicy,
                  const std::string& dest, const CFilePolicy& destPolicy,
                  IFile::eWriteSyncBehaviour writeSyncBehaviour);
    bool FileRename(const std::string& source, const std::string& dest, const CFilePolicy& policy);
    std::vector<std::string> FilesInDirectory(const std::string& dir, const CFilePolicy& policy, const std::string& extension);
    std::vector<std::string> FoldersInDirectory(const std::string& dir, const CFilePolicy& policy);
    std::string GetPath(eFileLocation location, const std::string& fileName) const;
    CFilePolicy* GetPolicyByName(const std::string& name) const;
    std::string GetStorageDirectory(eFileLocation location) const;
    bool IsStorageSupported(eFileLocation location) const;
    C_ZipArchive* LoadArchive(const std::string& path, int, eFileOpenMode openMode); // UNIMPLEMENTED
    IFile* OpenFile(const std::string& path, const CFilePolicy& policy, eFileOpenMode openMode);
protected:
    virtual void ValidateFilePath(const std::string& path);
private:
    std::string archivePassword;
    CFilePolicy assetPolicy;
    CFilePolicy cachePolicy;
    CFilePolicy documentPolicy;
    CFilePolicy externalPolicy;
    std::map<std::string, CFilePolicy*> policyMap;
    std::map<eFileLocation, std::string> storageMap;

    bool DeleteFileFromPath(const std::string& path) const;
    bool DoesFileExistAtPath(const std::string& path);
    bool DoesFileExistInArchive(const std::string& archive, const std::string& file) const; // UNIMPLEMENTED
    IFile* OpenFileFromArchive(const std::string& archive, const std::string& file, std::string& error); // UNIMPLEMENTED, returns some other file type
    CBaseFileIO::CFile* OpenFileFromPath(const std::string& path, eFileOpenMode openMode, std::string& error);
    int RenameFileAtPath(const std::string& source, const std::string& dest);

    void FilesAndFoldersInDirectory(const std::string& dir, const CFilePolicy& policy, std::vector<std::string>* files,
                                    const std::string& extension, std::vector<std::string>* folders);
    bool FileNamesInArchive(const std::string&, const std::string&, const std::string&,
                            std::vector<std::string>*, std::vector<std::string>*); // UNIMPLEMENTED
    bool FileNamesInPath(const std::string& dir, const std::string& extension, std::vector<std::string>* files,
                         std::vector<std::string>* folders);
};

#endif // BASEFILEIO_H
