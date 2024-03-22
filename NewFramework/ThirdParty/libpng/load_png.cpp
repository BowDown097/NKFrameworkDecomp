#include "load_png.h"
#include "NewFramework/IO/File/BaseFileIO.h"
#include "NewFramework/IO/File/PNGInMemory.h"
#include "NewFramework/Platform/Shared/Logging.h"
#include <boost/smart_ptr/scoped_ptr.hpp>
#include <png.h>

#define PNG_SIG_BYTES 8

void custom_png_read_data(png_structp pngStruct, uint8_t* data, size_t sz)
{
    IFile* file = static_cast<IFile*>(png_get_io_ptr(pngStruct));
    file->ReadBytes(data, sz);
}

void png_error_func(png_structp pngStruct, const char* error)
{
    LOG_ERROR("%s", error);
}

// I am fairly certain the noSimd bool is correct, but I have not implemented it. Why?
// 1. It is always true, so only the code path for it being true is really needed.
// 2. Decompiled SIMD is a mess. No thanks.
uint8_t* load_png(IFile* file, int* width, int* height, bool grayToRgb, bool alphaFillerIfNeeded, bool noSimd)
{
    uint8_t sigBytes[PNG_SIG_BYTES];
    file->ReadBytes(sigBytes, PNG_SIG_BYTES);

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, png_error_func, png_error_func);
    png_infop info = png_create_info_struct(png);

    png_set_read_fn(png, file, custom_png_read_data);
    png_set_sig_bytes(png, PNG_SIG_BYTES);
    png_read_info(png, info);

    *width = png_get_image_width(png, info);
    *height = png_get_image_height(png, info);
    png_byte bitDepth = png_get_bit_depth(png, info);
    png_byte colorType = png_get_color_type(png, info);

    if (bitDepth == 16)
        png_set_strip_16(png);
    if (colorType == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);
    if (colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8)
        png_set_expand_gray_1_2_4_to_8(png);
    if (grayToRgb && (colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_GRAY_ALPHA))
        png_set_gray_to_rgb(png);

    if (png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);
    else if (alphaFillerIfNeeded)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

    png_read_update_info(png, info);

    size_t rowBytes = png_get_rowbytes(png, info);
    png_bytep imageData = static_cast<png_bytep>(malloc(rowBytes * *height));
    png_bytepp rowPointers = static_cast<png_bytepp>(malloc(sizeof(png_bytep) * *height));

    for (int i = 0; i < *height; ++i)
        rowPointers[i] = &imageData[i * rowBytes];

    png_read_image(png, rowPointers);
    free(rowPointers);
    png_destroy_read_struct(&png, &info, NULL);
    return imageData;
}

uint8_t* load_png(CBaseFileIO* fileIO, const char* path, CFilePolicy& filePolicy, int* width, int* height)
{
    IFile* file = fileIO->OpenFile(path, filePolicy, eFileOpenMode::Read);
    if (!file)
    {
        LOG_ERROR("Couldn't load PNG file (%s)", path);
        return NULL;
    }

    uint8_t* png = load_png(file, width, height, true, true, true);
    delete file;
    return png;
}

uint8_t* load_png_memory(uint8_t* data, uint32_t size, int* width, int* height)
{
    boost::scoped_ptr<PNG_In_Memory> scopedFile(new PNG_In_Memory);
    scopedFile->WriteBytes(data, size, IFile::eWriteSyncBehaviour::Sync);
    return load_png(scopedFile.get(), width, height, false, false, true);
}
