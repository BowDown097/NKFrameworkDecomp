#pragma once
#include <string>

// TODO: implement compress, decompress, calc_nkdatahash_v1, hex_string_to_string, hmac_sha1, and strip_weak_etag_marker
std::string base64_encode(const std::string& bindata);
std::string base64_decode(const std::string& ascdata);
bool string_to_base64(const std::string& bindata, std::string& out);
bool base64_to_string(const std::string& ascdata, std::string& out);
std::string string_to_md5(const std::string& data);
