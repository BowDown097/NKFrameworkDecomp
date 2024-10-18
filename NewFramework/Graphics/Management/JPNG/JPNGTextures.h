#pragma once
#include <cstdint>
#include <string>

uint8_t* LoadJPNGTexture(class CBaseFileIO* fileIO, const std::string& path, int* width, int* height, uint32_t* pixels);
