#include "File.h"
#include "NewFramework/Platform/Shared/Assert.h"
#include "NewFramework/Platform/Shared/Core.h"
#include "NewFramework/Platform/Shared/Logging.h"
#include "Uncategorized/StringHelper.h"
#include <cstring>
#include <fcntl.h>
#include <functional>

CBaseFileIO::CFile::~CFile()
{
    CBaseFileIO::CFile::Close(eWriteSyncBehaviour::Sync);
}

void CBaseFileIO::CFile::Close(eWriteSyncBehaviour writeSyncBehaviour)
{
    if (!platformData->file)
        return;

    if (openMode == eFileOpenMode::ReadWriteNew && writeSyncBehaviour == eWriteSyncBehaviour::Sync &&
        ferror(platformData->file) == 0)
    {
        SyncToDisk();
    }

    if (fclose(platformData->file) != 0)
    {
        std::string error = "Error running fclose: " + CCore::ThreadSafeStrError(errno);
        NKAssert(false, error.c_str());
    }
}

size_t CBaseFileIO::CFile::GetFileLength()
{
    UpdateFileLength();
    return length;
}

bool CBaseFileIO::CFile::Open(const std::string& path, eFileOpenMode openMode, std::string& error)
{
    this->openMode = openMode;
    if (openMode == eFileOpenMode::Append)
    {
        error = "Append isn't (yet) supported on this platform";
        if (!NKAssert(false, error.c_str()))
            return false;
    }

    std::string modeString;
    switch (openMode)
    {
    case eFileOpenMode::Read:
        modeString = "rb";
        break;
    case eFileOpenMode::Write:
        modeString = "wb";
        break;
    case eFileOpenMode::ReadWriteNew:
        modeString = "w+b";
        break;
    case eFileOpenMode::ReadWriteExisting:
        modeString = "r+b";
        break;
    case eFileOpenMode::Append:
        modeString = "a+";
        break;
    }

    platformData->file = fopen(path.c_str(), modeString.c_str());
    if (!platformData->file)
    {
        error = StringHelper::Format("CFile::Open unable to open file ('%s') : %s", path.c_str(), CCore::ThreadSafeStrError(errno).c_str());
        return false;
    }

    UpdateFileLength();
    this->path = path;

    return platformData->file != NULL;
}

size_t CBaseFileIO::CFile::ReadBytes(uint8_t* const data, const size_t size)
{
    SFileIOResult ioResult = ReadBytesWithResult(data, size);
    if (ioResult.status != eFileIOStatus::Success)
        NKAssert(false, ioResult.info.c_str());
    return ioResult.bytesTransferred;
}

IFile::SFileIOResult CBaseFileIO::CFile::ReadBytes_Internal(uint8_t* const data, const size_t size)
{
    SFileIOResult result;
    result.bytesAttempted = size;

    if (fseek(platformData->file, offset, SEEK_SET) != 0)
    {
        result.info = "Error: " + CCore::ThreadSafeStrError(errno);
        result.status = eFileIOStatus::IOFailed;
        return result;
    }

    result.bytesTransferred = fread(data, 1, size, platformData->file);
    if (result.bytesTransferred == result.bytesAttempted)
    {
        result.info = "All bytes were read successfully";
    }
    else
    {
        int eof = feof(platformData->file);
        if (eof == 0)
        {
            int errorInd = ferror(platformData->file);
            if (errorInd == 0)
            {
                result.info = "Unable to read all bytes, but eof and error flags not set";
                result.status = eFileIOStatus::Failed;
                return result;
            }
            
            result.info = "Error: " + CCore::ThreadSafeStrError(errno);
            result.status = result.bytesTransferred == 0 ? eFileIOStatus::IOFailed : eFileIOStatus::IOIncomplete;
            return result;
        }

        result.info = "Warning: Attempted to read past end of file";
    }

    result.status = eFileIOStatus::Success;

    size_t pos = ftell(platformData->file);
    if (pos == -1)
    {
        LOG_ERROR("Unable to get file stream position (%s). Setting current offset manually.", CCore::ThreadSafeStrError(errno).c_str());
        offset = result.bytesTransferred;
    }
    else
    {
        offset = pos;
    }

    return result;
}

IFile::SFileIOResult CBaseFileIO::CFile::ReadBytesWithResult(uint8_t* const data, const size_t size)
{
    SFileIOResult result;

    std::string sanityError;
    if (!SanityCheck(sanityError, eFileOpenMode::Read))
    {
        result.info = StringHelper::Format("ReadBytesWithResult(): %s", sanityError.c_str());
        result.status = eFileIOStatus::SanityCheckFailed;
        return result;
    }

    if (!data)
    {
        result.info = "ReadBytesWithResult() Null Buffer. ('" + path + "')";
        result.status = eFileIOStatus::NullReadBuffer;
        return result;
    }

    if (size == 0)
    {
        result.info = "ReadBytesWithResult() Zero-length Buffer. ('" + path + "')";
        result.status = eFileIOStatus::ZeroLengthReadBuffer;
        return result;
    }

    result = ReadBytes_Internal(data, size);
    return result;
}

bool CBaseFileIO::CFile::ReadString(std::string& data)
{
    SFileIOResult ioResult = ReadStringWithResult(data);
    if (ioResult.status != eFileIOStatus::Success)
        NKAssert(false, ioResult.info.c_str());
    return ioResult.status == eFileIOStatus::Success;
}

IFile::SFileIOResult CBaseFileIO::CFile::ReadStringWithResult(std::string& data)
{
    SFileIOResult result;

    std::string sanityError;
    if (!SanityCheck(sanityError, eFileOpenMode::Read))
    {
        result.info = StringHelper::Format("ReadStringWithResult(): %s", sanityError.c_str());
        result.status = eFileIOStatus::SanityCheckFailed;
        return result;
    }

    UpdateFileLength();
    data.resize(length);
    if (!length)
    {
        result.info = "File is empty (zero length)";
        result.status = eFileIOStatus::Success;
        return result;
    }

    SFileIOResult readBytesResult = ReadBytes_Internal(reinterpret_cast<uint8_t* const>(data.data()), data.size());
    result.bytesAttempted = readBytesResult.bytesAttempted;
    result.status = readBytesResult.status;
    result.info = readBytesResult.info;

    if (result.status != eFileIOStatus::Success)
        data = "";

    return result;
}

bool CBaseFileIO::CFile::SanityCheck(std::string& errorString, eFileOpenMode expectedOpenMode)
{
    if (!platformData->file)
    {
        errorString = StringHelper::Format("Sanity check failed for file '%s' - handle is not valid", path.c_str());
        return false;
    }

    if (ferror(platformData->file))
    {
        errorString = StringHelper::Format("Sanity check failed for file '%s' - error indicator is set", path.c_str());
        return false;
    }

    if (expectedOpenMode != openMode)
    {
        errorString = StringHelper::Format("Sanity check failed for file '%s' - file open mode is invalid", path.c_str());
        return false;
    }

    return true;
}

void CBaseFileIO::CFile::SyncToDisk()
{
    std::string sanityError;
    if (!SanityCheck(sanityError, eFileOpenMode::ReadWriteNew))
    {
        LOG_ERROR("When syncing to disk: %s", sanityError.c_str());
        return;
    }

    if (fflush(platformData->file) != 0)
    {
        LOG_ERROR("Can't sync file '%s' to disk - fflush failed (%s)", path.c_str(), CCore::ThreadSafeStrError(errno).c_str());
        return;
    }

    int desc = fileno(platformData->file);
    if (desc == -1)
    {
        LOG_ERROR("Can't sync file '%s' to disk - fileno failed (%s)", path.c_str(), CCore::ThreadSafeStrError(errno).c_str());
        return;
    }

    if (fcntl(desc, 51, 1) == -1)
        LOG_ERROR("Can't sync file '%s' to disk - fcntl failed (%s)", path.c_str(), CCore::ThreadSafeStrError(errno).c_str());
}

int CBaseFileIO::CFile::UpdateFileLength()
{
    fseek(platformData->file, 0, SEEK_END);
    length = ftell(platformData->file);
    return fseek(platformData->file, offset, SEEK_SET);
}

size_t CBaseFileIO::CFile::WriteBytes(uint8_t* const input, const size_t size, eWriteSyncBehaviour writeSyncBehaviour)
{
    SFileIOResult ioResult = WriteBytesWithResult(input, size, writeSyncBehaviour);
    if (ioResult.status != eFileIOStatus::Success)
        NKAssert(false, ioResult.info.c_str());
    return ioResult.bytesTransferred;
}

IFile::SFileIOResult CBaseFileIO::CFile::WriteBytesWithResult(uint8_t* const input, const size_t size,
                                                              eWriteSyncBehaviour writeSyncBehaviour)
{
    SFileIOResult result;

    std::string sanityError;
    if (!SanityCheck(sanityError, eFileOpenMode::ReadWriteNew))
    {
        result.info = StringHelper::Format("WriteBytesWithResult(): %s", sanityError.c_str());
        result.status = eFileIOStatus::SanityCheckFailed;
        return result;
    }

    if (!input)
    {
        result.status = eFileIOStatus::NullWriteBuffer;
        result.info = "Error: Source buffer is null";
        return result;
    }

    if (size == 0)
    {
        result.status = eFileIOStatus::ZeroLengthWriteBuffer;
        result.info = "Error: Attempted to write no bytes";
        return result;
    }

    fseek(platformData->file, offset, SEEK_SET);
    result.bytesAttempted = size;
    result.bytesTransferred = fwrite(input, 1, size, platformData->file);

    if (result.bytesTransferred == 0)
    {
        result.info = "WriteBytesWithResult() Did not write any data - write was a no-op. ('" + path + "')";
        result.status = eFileIOStatus::IOFailed;
        return result;
    }

    if (result.bytesAttempted != result.bytesTransferred || ferror(platformData->file))
    {
        result.info = StringHelper::Format(
            "WriteBytesWithResult() Did not write all data - the file will contain some data, but the pointer has been reset."
            " If writing a stream, verify filelength. ('%s'), bytesAttempted: %d, bytesTransferred: %d",
            path.c_str(),
            result.bytesAttempted,
            result.bytesTransferred
        );
        result.status = eFileIOStatus::IOIncomplete;
        return result;
    }

    offset = result.bytesTransferred;

    UpdateFileLength();
    if (writeSyncBehaviour == eWriteSyncBehaviour::Sync)
        SyncToDisk();

    result.status = eFileIOStatus::Success;
    return result;
}

bool CBaseFileIO::CFile::WriteString(const std::string& input, eWriteSyncBehaviour writeSyncBehaviour)
{
    SFileIOResult ioResult = WriteStringWithResult(input, writeSyncBehaviour);
    if (ioResult.status != eFileIOStatus::Success)
        NKAssert(false, ioResult.info.c_str());
    return ioResult.status == eFileIOStatus::Success;
}

IFile::SFileIOResult CBaseFileIO::CFile::WriteStringWithResult(const std::string& input, eWriteSyncBehaviour writeSyncBehaviour)
{
    return WriteBytesWithResult(
        reinterpret_cast<uint8_t* const>(const_cast<char*>(input.data())),
        input.size(),
        writeSyncBehaviour
    );
}
