#include "NKFileClient.h"
#include "NKSession.h"

std::string NKFileClient::GetCachedFileEtag(const std::string& name) const {
    return impl->GetCachedFileEtag(name);
}

std::string NKFileClient::GetCachedFileDataHash(const std::string& name) const {
    return impl->GetCachedFileDataHash(name);
}

std::vector<std::string> NKFileClient::GetCachedFileNames() const {
    return impl->GetCachedFileNames();
}

void NKFileClient::ClearCachedFileEtag(const std::string& name) {
    impl->ClearCachedFileEtag(name);
}

void NKFileClient::SaveCache() {
    impl->SaveCachedFileData();
}

const bool NKFileClient::Download(
    const std::string& filename,
    eNKFileClientFilePermissions filePermissions,
    NKFileClientCallback_t callback) {

    std::stringstream ss;
    ss << NKSession::GetActiveSession()->GetAppID();

    std::string nkapiID = NKSession::GetActiveSession()->GetUserDetails().nkapiID;
    std::string appIDStr = ss.str();

    return impl->Download(
        appIDStr,
        nkapiID,
        filename,
        filePermissions != eNKFileClientFilePermissions::Public,
        true,
        filePermissions,
        callback,
        false);
}

const bool NKFileClient::Download(
    const std::string& appID,
    const std::string& nkapiID,
    const std::string& filename,
    bool isRendered,
    bool isStorageResponse,
    eNKFileClientFilePermissions filePermissions,
    NKFileClientCallback_t callback,
    bool stephen) {

    return impl->Download(appID, nkapiID, filename, isRendered, isStorageResponse, filePermissions, callback, stephen);
}

const bool NKFileClient::Download(
    const std::string& url,
    NKFileClientCallback_t callback,
    SHttpTimeoutOptions timeoutOpts,
    bool a4,
    bool stephen) {

    NKResponseRenderOptions renderOpts;
    renderOpts.field_0 = a4 ? 256 : 0;

    return Download(url, timeoutOpts, renderOpts, callback, stephen);
}

const bool NKFileClient::Download(
    const std::string& url,
    SHttpTimeoutOptions timeoutOpts,
    NKResponseRenderOptions renderOpts,
    NKFileClientCallback_t callback,
    bool stephen) {

    return impl->Download(url, callback, timeoutOpts, renderOpts, stephen);
}

const bool NKFileClient::Download(
    const std::string& filename,
    const std::string& nkapiID,
    eNKFileClientFilePermissions filePermissions,
    NKFileClientCallback_t callback,
    bool stephen) {

    std::stringstream ss;
    ss << NKSession::GetActiveSession()->GetAppID();
    std::string appIDStr = ss.str();

    return impl->Download(
        appIDStr,
        nkapiID,
        filename,
        filePermissions != eNKFileClientFilePermissions::Public,
        true,
        filePermissions,
        callback,
        stephen);
}

const bool NKFileClient::Initialise(CBaseFileIO* fileIO, CEventManager* eventManager) {
    return impl->Initialise(fileIO, eventManager);
}

void NKFileClient::Invalidate(const std::string& filename) {
    impl->Invalidate(filename);
}

const bool NKFileClient::IsDownloadingURL(const std::string& url) const {
    return impl->IsDownloadingURL(url);
}

const bool NKFileClient::IsDownloading(const std::string& filename) const {
    return impl->IsDownloading(filename);
}

const bool NKFileClient::Upload(
    const std::string& filename,
    const std::string& data,
    eNKFileClientFilePermissions filePermissions,
    NKFileClientCallback_t callback,
    NKFileClientConflictCallback_t conflictCallback,
    bool noConflictCheck) {

    return impl->Upload(filename, data, filePermissions, callback, conflictCallback, noConflictCheck);
}

const bool NKFileClient::Upload(
    const std::string& filename,
    IFile* file,
    eNKFileClientFilePermissions filePermissions,
    NKFileClientCallback_t callback,
    NKFileClientConflictCallback_t conflictCallback,
    bool noConflictCheck) {

    return impl->Upload(filename, file, filePermissions, callback, conflictCallback, noConflictCheck);
}

const bool NKFileClient::UploadBuffered(
    const std::string& filename,
    const std::string& data,
    eNKFileClientFilePermissions filePermissions,
    NKFileClientCallback_t callback,
    NKFileClientConflictCallback_t conflictCallback,
    uint64_t maximumBufferDuration,
    bool noConflictCheck) {

    return impl->UploadBuffered(
        filename, data, filePermissions, callback, conflictCallback, maximumBufferDuration, noConflictCheck);
}

const bool NKFileClient::UploadBuffered(
    const std::string& filename,
    IFile* file,
    eNKFileClientFilePermissions filePermissions,
    NKFileClientCallback_t callback,
    NKFileClientConflictCallback_t conflictCallback,
    uint64_t maximumBufferDuration,
    bool noConflictCheck) {

    return impl->UploadBuffered(
        filename, file, filePermissions, callback, conflictCallback, maximumBufferDuration, noConflictCheck);
}
