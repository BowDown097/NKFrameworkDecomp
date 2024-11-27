#pragma once
#include <cstdint>
#include <string>

int CalcCRC(int val);
uint32_t CalcCRCUint32(uint32_t val);
int GetCRCFromName(const char* data);
int GetCRCFromData(const char* data, size_t size);
uint32_t GetCRCFromDataUint32(const char* data, size_t size);
uint32_t FlagToIndex(uint32_t val);
uint32_t flag_to_index(uint32_t val);
uint64_t djb2_Hash(std::string& data);
void nk_memory_dump(void* offset, size_t bytes, const char* name);
