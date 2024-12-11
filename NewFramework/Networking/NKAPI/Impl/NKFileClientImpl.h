#pragma once
#include "NewFramework/Networking/NKAPI/Impl/FileClientActions/NKAction_FileConflict.h"
#include "Uncategorized/BehaviourTree.h"
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/unordered/unordered_map.hpp>
#include <map>

class CBaseFileIO;
class CEventManager;
enum class eNKFileClientFilePermissions;
enum class FileClientCallbackAction;
class IFile;
struct NKError;
struct SHttpTimeoutOptions;
struct SNKFileClientBlackboard;

// not 100% sure about this struct. the disassembly of the methods that use it in this class is really weird,
// and unfortunately afaik those are the only usages of this struct. but this is the best fit i think.
struct NKResponseRenderOptions {
    int field_0; // 0x00
};

using NKFileClientCallback_t = boost::function4<void, const bool, const std::string&, const std::string&, const NKError*>;

class NKFileClientImpl {
public:
    struct FileMetaData {
        std::string etag; // 0x00
        std::string dataHash; // 0x18
    };

    struct QueuedUploadData {
        std::vector<char> data; // 0x00
        eNKFileClientFilePermissions filePermissions; // 0x18
        NKFileClientCallback_t callback; // 0x20
        NKFileClientConflictCallback_t conflictCallback; // 0x40
        uint64_t maximumBufferDuration; // 0x60
        bool buffer; // 0x68
        bool noConflictCheck; // 0x69
    };

    struct TreeData {
        boost::shared_ptr<BehaviourTree::Action> action; // 0x00
        boost::shared_ptr<SNKFileClientBlackboard> blackboard; // 0x10
        FileClientCallbackAction callbackAction; // 0x20
    };

    CBaseFileIO* fileIO{}; // 0x00
    CEventManager* eventManager; // 0x08
    boost::unordered_map<std::string, FileMetaData> metadataMap; // 0x10

    std::string GetCachedFileEtag(const std::string& name) const;
    std::string GetCachedFileDataHash(const std::string& name) const;
    std::vector<std::string> GetCachedFileNames() const;
    void ClearCachedFileEtag(const std::string& name);
    const bool IsDownloadingURL(const std::string& url) const;
    std::string ConstructSavePath(const std::string& url) const;
    const bool Download(
        const std::string& url,
        NKFileClientCallback_t callback,
        SHttpTimeoutOptions timeoutOpts,
        NKResponseRenderOptions renderOpts,
        bool stephen);
    void ProcessBHTree(boost::shared_ptr<BehaviourTree::Action>, BehaviourTree::IBlackboard*);
    const bool Download(
        const std::string& appID,
        const std::string& nkapiID,
        const std::string& filename,
        bool isRendered,
        bool isStorageResponse,
        eNKFileClientFilePermissions filePermissions,
        NKFileClientCallback_t callback,
        bool stephen);
    std::string ConstructSavePath(const std::string& appID, std::string nkapiID) const;
    const bool Initialise(CBaseFileIO* fileIO, CEventManager* eventManager);
    void LoadMetaData();
    void Invalidate(const std::string& filename);
    void SaveCachedFileData();
    bool IsDownloading(const std::string& filename) const;
    const bool Upload(
        const std::string& filename,
        const std::vector<char>& data,
        eNKFileClientFilePermissions filePermissions,
        NKFileClientCallback_t callback,
        NKFileClientConflictCallback_t conflictCallback,
        bool noConflictCheck,
        bool buffer,
        uint64_t maximumBufferDuration);
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
    void QueueUpload(
        const std::string& filename,
        const std::vector<char>& data,
        eNKFileClientFilePermissions filePermissions,
        NKFileClientCallback_t callback,
        NKFileClientConflictCallback_t conflictCallback,
        bool noConflictCheck,
        bool buffer,
        uint64_t maximumBufferDuration);
private:
    bool initialised{}; // 0x40
    std::map<std::string, QueuedUploadData> uploadDataMap; // 0x48
    std::map<std::string, TreeData> treeDataMap; // 0x60
};
