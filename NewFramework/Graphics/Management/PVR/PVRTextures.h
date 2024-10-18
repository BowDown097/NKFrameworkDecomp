#pragma once
#include <cstdint>

uint8_t* LoadPVRTexture(const char* path, int* width, int* height, uint32_t* colorSpace, uint32_t* pixels);
