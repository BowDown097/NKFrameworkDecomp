#pragma once
#include "Impl/NKFileClientImpl.h"

class NKFileClient {
public:
    std::string GetCachedFileEtag(const std::string& name) const;
    std::string GetCachedFileDataHash(const std::string& name) const;
    std::vector<std::string> GetCachedFileNames() const;
    void ClearCachedFileEtag(const std::string& name);
    void SaveCache();
    const bool Download(
        const std::string& filename,
        eNKFileClientFilePermissions filePermissions,
        NKFileClientCallback_t callback);
    const bool Download(
        const std::string& appID,
        const std::string& nkapiID,
        const std::string& filename,
        bool isRendered,
        bool isStorageResponse,
        eNKFileClientFilePermissions filePermissions,
        NKFileClientCallback_t callback,
        bool stephen);
    const bool Download(
        const std::string& url,
        NKFileClientCallback_t callback,
        SHttpTimeoutOptions timeoutOpts,
        bool a4,
        bool stephen);
    const bool Download(
        const std::string& url,
        SHttpTimeoutOptions timeoutOpts,
        NKResponseRenderOptions renderOpts,
        NKFileClientCallback_t callback,
        bool stephen);
    const bool Download(
        const std::string& filename,
        const std::string& nkapiID,
        eNKFileClientFilePermissions filePermissions,
        NKFileClientCallback_t callback,
        bool stephen);
    const bool Initialise(CBaseFileIO* fileIO, CEventManager* eventManager);
    void Invalidate(const std::string& filename);
    const bool IsDownloadingURL(const std::string& url) const;
    const bool IsDownloading(const std::string& filename) const;
    const bool Upload(
        const std::string& filename,
        const std::string& data,
        eNKFileClientFilePermissions filePermissions,
        NKFileClientCallback_t callback,
        NKFileClientConflictCallback_t conflictCallback,
        bool noConflictCheck);
    const bool Upload(
        const std::string& filename,
        IFile* file,
        eNKFileClientFilePermissions filePermissions,
        NKFileClientCallback_t callback,
        NKFileClientConflictCallback_t conflictCallback,
        bool noConflictCheck);
    const bool UploadBuffered(
        const std::string& filename,
        const std::string& data,
        eNKFileClientFilePermissions filePermissions,
        NKFileClientCallback_t callback,
        NKFileClientConflictCallback_t conflictCallback,
        uint64_t maximumBufferDuration,
        bool noConflictCheck);
    const bool UploadBuffered(
        const std::string& filename,
        IFile* file,
        eNKFileClientFilePermissions filePermissions,
        NKFileClientCallback_t callback,
        NKFileClientConflictCallback_t conflictCallback,
        uint64_t maximumBufferDuration,
        bool noConflictCheck);
private:
    boost::shared_ptr<NKFileClientImpl> impl = boost::shared_ptr<NKFileClientImpl>(new NKFileClientImpl);
};
