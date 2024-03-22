#include "PVRTextures.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>

#define PVR_HEADER_SIZE 52

void EndianSwap(char* ptr, uint32_t chunkSize, uint32_t chunks)
{
    if (chunkSize == 0 || chunks == 0)
        return;

    uint32_t chunkSizeRound = chunkSize & 0xFFFFFF00;
    if (chunkSizeRound == 0)
        return;

    for (int i = 0; i < chunks; ++i)
    {
        char* start = &ptr[i * chunkSizeRound];
        char* end = &ptr[(i + 1) * chunkSizeRound - 1];
        while (start < end)
        {
            char temp = *start;
            *start++ = *end;
            *end-- = temp;
        }
    }
}

uint8_t* LoadPVRTexture(const char* path, int* width, int* height, uint32_t* colorSpace, uint32_t* pixels)
{
    FILE* file = fopen(path, "rb");
    uint8_t* result{};

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);

    char* data = static_cast<char*>(malloc(length));
    fread(data, 1, length, file);
    fclose(file);

    EndianSwap(data, 32, 13);
    int* data_intptr = reinterpret_cast<int*>(data);

    *colorSpace = *(data_intptr + 4);
    *width = *(data_intptr + 7);
    *height = *(data_intptr + 6);
    *pixels = 8 * *width * *height;

    result = static_cast<uint8_t*>(malloc(*pixels));
    memcpy(result, data + PVR_HEADER_SIZE, *pixels);

    free(data);
    return result;
}
