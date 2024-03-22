#include "JPNGTextures.h"
#include "NewFramework/Graphics/Management/JPEG/JPEGTextures.h"
#include "NewFramework/IO/File/BaseFileIO.h"
#include "NewFramework/Platform/Shared/Logging.h"
#include "NewFramework/ThirdParty/libpng/load_png.h"

uint8_t* LoadJPNGTexture(CBaseFileIO* fileIO, const std::string& path, int* width, int* height, uint32_t* pixels)
{
    IFile* file = fileIO->OpenFile(path, fileIO->assetPolicy, eFileOpenMode::Read);
    if (!file)
    {
        LOG_ERROR("Can't open JPNG '%s'.\n", path.c_str());
        return NULL;
    }

    size_t fileLength = file->GetFileLength();
    uint8_t* fileData = new uint8_t[fileLength];
    file->ReadBytes(fileData, fileLength);

    uint32_t pngOffset = *(uint32_t*)&fileData[fileLength - 16];
    uint32_t pngSize = *(uint32_t*)&fileData[fileLength - 12];
    uint8_t* pngData = load_png_memory(&fileData[pngOffset], pngSize, width, height);
    *pixels = 4 * *width * *height;

    int jpegWidth, jpegHeight;
    uint32_t jpegPixels;
    uint8_t* jpegData = static_cast<uint8_t*>(malloc(*pixels));
    LoadJPEGTexture(fileData, pngOffset, &jpegWidth, &jpegHeight, &jpegPixels, &jpegData);

    delete[] fileData;
    file->Close(IFile::eWriteSyncBehaviour::Sync);
    delete file;
    free(pngData);

    return jpegData;
}
