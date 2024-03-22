#ifndef JPNGTEXTURES_H
#define JPNGTEXTURES_H
#include <cstdint>
#include <string>

uint8_t* LoadJPNGTexture(class CBaseFileIO* fileIO, const std::string& path, int* width, int* height, uint32_t* pixels);

#endif // JPNGTEXTURES_H
