#include "JPEGTextures.h"
#include "NewFramework/IO/File/BaseFileIO.h"
#include "NewFramework/IO/File/FilePolicy.h"
#include "NewFramework/Platform/Shared/Logging.h"
#include <csetjmp>
#include <jpeglib.h>

static char lasterror[200] = "No error";

struct my_error_mgr
{
    jpeg_error_mgr pub;
    jmp_buf jb;
};

static void my_error_exit(j_common_ptr cinfo)
{
    my_error_mgr* err = reinterpret_cast<my_error_mgr*>(cinfo->err);
    cinfo->err->output_message(cinfo);
    longjmp(err->jb, 1);
}

static void my_output_message(j_common_ptr cinfo)
{
    cinfo->err->format_message(cinfo, lasterror);
}

uint8_t* LoadJPEGTexture(CBaseFileIO* fileIO, const std::string& path, CFilePolicy& filePolicy, int* width,
                         int* height, uint32_t* pixels)
{
    IFile* file = fileIO->OpenFile(path, filePolicy, eFileOpenMode::Read);
    if (!file)
    {
        LOG_ERROR("Can't open JPEG '%s'.\n", path.c_str());
        return NULL;
    }

    jpeg_decompress_struct cinfo;
    my_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;
    jerr.pub.output_message = my_output_message;

    if (setjmp(jerr.jb))
    {
        LOG_ERROR("JPEG code has signaled an error: '%s'.", lasterror);
        jpeg_destroy_decompress(&cinfo);
        delete file;
        return NULL;
    }

    jpeg_create_decompress(&cinfo);

    size_t fileLength = file->GetFileLength();
    uint8_t* fileData = new uint8_t[fileLength + 1];
    fileData[fileLength] = 0;
    file->ReadBytes(fileData, fileLength);

    jpeg_mem_src(&cinfo, fileData, fileLength);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    JDIMENSION rowStride = cinfo.output_width * cinfo.output_components;
    JSAMPARRAY buffer = cinfo.mem->alloc_sarray(reinterpret_cast<j_common_ptr>(&cinfo), JPOOL_IMAGE, rowStride, 1);

    *width = cinfo.output_width;
    *height = cinfo.output_height;
    *pixels = *width * *height * cinfo.output_components;

    uint8_t* result = static_cast<uint8_t*>(malloc(*pixels));
    while (cinfo.output_scanline < cinfo.output_height)
    {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        memcpy(result + rowStride * (cinfo.output_scanline - 1), *buffer, rowStride);
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    delete[] fileData;
    delete file;

    return result;
}

void LoadJPEGTexture(uint8_t* data, uint32_t size, int* width, int* height, uint32_t* pixels, uint8_t** out)
{
    jpeg_decompress_struct cinfo;
    my_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;
    jerr.pub.output_message = my_output_message;

    if (setjmp(jerr.jb))
    {
        LOG_ERROR("JPEG code has signaled an error: '%s'.", lasterror);
        out = NULL;
        jpeg_destroy_decompress(&cinfo);
        return;
    }

    jpeg_create_decompress(&cinfo);
    jpeg_mem_src(&cinfo, data, size);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    JDIMENSION rowStride = cinfo.output_width * cinfo.output_components;
    JSAMPARRAY buffer = cinfo.mem->alloc_sarray(reinterpret_cast<j_common_ptr>(&cinfo), JPOOL_IMAGE, rowStride, 1);

    *width = cinfo.output_width;
    *height = cinfo.output_height;
    *pixels = *width * *height * cinfo.output_components;

    if (!*out)
        *out = static_cast<uint8_t*>(malloc(*pixels));

    while (cinfo.output_scanline < cinfo.output_height)
    {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        memcpy(*out + rowStride * (cinfo.output_scanline - 1), *buffer, rowStride);
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
}
