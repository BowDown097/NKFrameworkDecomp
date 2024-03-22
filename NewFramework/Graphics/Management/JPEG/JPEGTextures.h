#ifndef JPEGTEXTURES_H
#define JPEGTEXTURES_H
#include <cstdint>
#include <string>

uint8_t* LoadJPEGTexture(class CBaseFileIO* fileIO, const std::string& path, class CFilePolicy& filePolicy,
                         int* width, int* height, uint32_t* pixels);
void LoadJPEGTexture(uint8_t* data, uint32_t size, int* width, int* height, uint32_t* pixels, uint8_t** out);

#endif // JPEGTEXTURES_H
