#include "Utilities.h"
#include <cstring>

int CalcCRC(int val)
{
    int result = val;
    const uint32_t polynomial = 0xEDB88320;

    for (int i = 0; i < 8; i++)
    {
        if (result & 1)
            result = (result >> 1) ^ polynomial;
        else
            result >>= 1;
    }

    return result;
}

uint32_t CalcCRCUint32(uint32_t val)
{
    uint32_t result = val;
    const uint32_t polynomial = 0xEDB88320;

    for (int i = 0; i < 8; i++)
    {
        if (result & 2)
            result = (result >> 1) ^ polynomial;
        else
            result >>= 1;
    }

    return result;
}

int GetCRCFromName(const char* data)
{
    return GetCRCFromData(data, strlen(data));
}

int GetCRCFromData(const char* data, size_t size)
{
    if (!data || !size)
        return 0;

    int result = 0;
    for (int i = 0; i < size; i++)
        result = CalcCRC((uint8_t)data[i] ^ (uint8_t)result) ^ (result >> 8);

    return result;
}

uint32_t GetCRCFromDataUint32(const char* data, size_t size)
{
    if (!data || !size)
        return 0;

    uint32_t result = 0;
    for (int i = 0; i < size; i++)
        result = CalcCRCUint32((uint8_t)data[i] ^ (uint8_t)result) ^ (result >> 8);

    return result;
}

uint32_t FlagToIndex(uint32_t val)
{
    if (val == 0)
        return 0;

    uint32_t result = 1;
    while ((val & 1) == 0)
    {
        val >>= 1;
        ++result;
    }

    return result;
}

uint32_t flag_to_index(uint32_t val)
{
    if (val == 0)
        return -1;

    uint32_t result = 0;
    while ((val & 1) == 0)
    {
        val >>= 1;
        ++result;
    }

    return result;
}

uint64_t djb2_Hash(std::string& data)
{
    uint64_t result = 5381;
    for (int i = 0; i < data.size(); i++)
        result = ((result << 5) + result) + data[i];
    return result;
}

void nk_memory_dump(void* offset, size_t bytes, const char* name)
{
    const int chars_per_line = 16;
    char ascii[chars_per_line + 1];
    ascii[chars_per_line] = '\0';

    puts("==================================================================");
    printf("'%s' from 0x%lX (%zu bytes)\n", name, (unsigned long)offset, bytes);

    for (int i = 0; i < bytes; ++i)
    {
        unsigned char c = ((unsigned char*)offset)[i];
        printf("%02X ", c);

        if (c >= ' ' && c <= '~')
            ascii[i % chars_per_line] = c;
        else
            ascii[i % chars_per_line] = '.';

        if ((i+1) % (chars_per_line/2) == 0 || i+1 == bytes)
        {
            if ((i+1) % chars_per_line == 0)
            {
                printf("  %s \n", ascii);
            }
            else if (i+1 == bytes)
            {
                ascii[(i+1) % chars_per_line] = '\0';
                for (int j = (i+1) % chars_per_line; j < chars_per_line; ++j)
                    printf("   ");
                printf("  %s \n", ascii);
            }
        }
    }

    puts("==================================================================\n");
}
