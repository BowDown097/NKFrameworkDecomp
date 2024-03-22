#ifndef LOAD_PNG_H
#define LOAD_PNG_H
#include <cstdint>

uint8_t* load_png(class IFile* file, int* width, int* height, bool grayToRgb, bool alphaFilterIfNeeded, bool noSimd);
uint8_t* load_png(class CBaseFileIO* fileIO, const char* path, class CFilePolicy& filePolicy, int* width, int* height);
uint8_t* load_png_memory(uint8_t* data, uint32_t size, int* width, int* height);

#endif // LOAD_PNG_H
