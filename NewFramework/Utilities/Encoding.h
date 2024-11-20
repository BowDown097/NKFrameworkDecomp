#pragma once
#include <cstdint>
#include <string>
#include <vector>

bool compress(const std::vector<char>& data, std::string& out);
bool decompress(const char* data, uint64_t data_sz, std::string& out);
std::string base64_encode(const std::string& bindata);
std::string base64_decode(const std::string& ascdata);
bool string_to_base64(const std::string& bindata, std::string& out);
bool base64_to_string(const std::string& ascdata, std::string& out);
std::string string_to_md5(const std::string& str);
std::string calc_nkdatahash_v1(const std::string& str);
std::string hex_string_to_string(const std::string& str);
std::string hmac_sha1(const std::string& key, const std::string& message, uint64_t blockSize = 0x40);
std::string strip_weak_etag_marker(const std::string& str);
