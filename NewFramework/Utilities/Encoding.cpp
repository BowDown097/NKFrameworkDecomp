#include "Encoding.h"
#include <cassert>
#include <easyzlib.h>
#include <hl_md5wrapper.h>
#include <hl_sha1wrapper.h>
#include <limits>

static const char b64_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const char reverse_table[128] = {
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
    64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
    64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64
};

bool compress(const std::vector<char>& data, std::string& out)
{
    ezbuffer buf;
    buf.Alloc(0x2000);

    const unsigned char* unsignedData = reinterpret_cast<const unsigned char*>(data.data());
    if (ezcompress(buf.pBuf, &buf.nLen, unsignedData, data.size()) == EZ_BUF_ERROR)
    {
        buf.Alloc(buf.nLen);
        if (ezcompress(buf.pBuf, &buf.nLen, unsignedData, data.size()) == EZ_BUF_ERROR)
            return false;
    }

    out.assign(reinterpret_cast<char*>(buf.pBuf), buf.nLen);
    return true;
}

bool decompress(const char* data, uint64_t data_sz, std::string& out)
{
    if (data_sz == 0)
    {
        out = "";
        return true;
    }

    ezbuffer buf;
    buf.Alloc(0x2000);

    const unsigned char* unsignedData = reinterpret_cast<const unsigned char*>(data);
    if (ezuncompress(buf.pBuf, &buf.nLen, unsignedData, data_sz) == EZ_BUF_ERROR)
    {
        buf.Alloc(buf.nLen);
        if (ezuncompress(buf.pBuf, &buf.nLen, unsignedData, data_sz) == EZ_BUF_ERROR)
            return false;
    }

    out.assign(reinterpret_cast<char*>(buf.pBuf), buf.nLen);
    return true;
}

std::string base64_encode(const std::string& bindata)
{
    if (bindata.size() > (std::numeric_limits<std::string::size_type>::max() / 4u) * 3u) {
        throw std::length_error("Converting too large a string to base64.");
    }

    const std::size_t binlen = bindata.size();
    // Use = signs so the end is properly padded.
    std::string retval((((binlen + 2) / 3) * 4), '=');
    std::size_t outpos = 0;
    int bits_collected = 0;
    unsigned int accumulator = 0;
    const std::string::const_iterator binend = bindata.end();

    for (std::string::const_iterator i = bindata.begin(); i != binend; ++i) {
        accumulator = (accumulator << 8) | (*i & 0xffu);
        bits_collected += 8;
        while (bits_collected >= 6) {
            bits_collected -= 6;
            retval[outpos++] = b64_table[(accumulator >> bits_collected) & 0x3fu];
        }
    }
    if (bits_collected > 0) { // Any trailing bits that are missing.
        assert(bits_collected < 6);
        accumulator <<= 6 - bits_collected;
        retval[outpos++] = b64_table[accumulator & 0x3fu];
    }
    assert(outpos >= (retval.size() - 2));
    assert(outpos <= retval.size());
    return retval;
}

std::string base64_decode(const std::string& ascdata)
{
    std::string retval;
    const std::string::const_iterator last = ascdata.end();
    int bits_collected = 0;
    unsigned int accumulator = 0;

    for (std::string::const_iterator i = ascdata.begin(); i != last; ++i) {
        const int c = *i;
        if (std::isspace(c) || c == '=') {
            // Skip whitespace and padding. Be liberal in what you accept.
            continue;
        }
        if ((c > 127) || (c < 0) || (reverse_table[c] > 63)) {
            throw std::invalid_argument("This contains characters not legal in a base64 encoded string.");
        }
        accumulator = (accumulator << 6) | reverse_table[c];
        bits_collected += 6;
        if (bits_collected >= 8) {
            bits_collected -= 8;
            retval += static_cast<char>((accumulator >> bits_collected) & 0xffu);
        }
    }
    return retval;
}

bool string_to_base64(const std::string& bindata, std::string& out)
{
    try
    {
        out = base64_encode(bindata);
        return true;
    } catch (const std::length_error& e) { return false; }
}

bool base64_to_string(const std::string& ascdata, std::string& out)
{
    try
    {
        out = base64_decode(ascdata);
        return true;
    } catch (const std::length_error& e) { return false; }
}

std::string string_to_md5(const std::string& str)
{
    md5wrapper wrapper;
    return wrapper.getHashFromString(str);
}

std::string calc_nkdatahash_v1(const std::string& str)
{
    std::string nkdata;
    nkdata += "LOC_GENERIC_YES";
    nkdata += string_to_md5(str);
    nkdata += "LOC_GENERIC_NO";
    return string_to_md5(nkdata);
}

std::string hex_string_to_string(const std::string& str)
{
    std::string out;

    for (int i = 0; i < str.size(); ++i)
    {
        std::string buf(str, i, 2);
        out += strtol(buf.c_str(), nullptr, 16);
    }

    return out;
}

std::string hmac_sha1(const std::string& key, const std::string& message, uint64_t blockSize)
{
    std::string iKeyPad, oKeyPad;
    std::string finalKey = key;
    sha1wrapper wrapper;

    if (finalKey.size() > blockSize)
    {
        oKeyPad = wrapper.getHashFromString(key);
        finalKey = iKeyPad = hex_string_to_string(oKeyPad);
    }

    if (finalKey.size() < blockSize)
    {
        finalKey.resize(blockSize);
    }

    iKeyPad.resize(blockSize);
    oKeyPad.resize(blockSize);

    for (uint64_t v14 = 0; v14 < blockSize; v14++)
    {
        iKeyPad[v14] = finalKey[v14] ^ 0x36;
        oKeyPad[v14] = finalKey[v14] ^ 0x5C;
    }

    return wrapper.getHashFromString(oKeyPad + hex_string_to_string(wrapper.getHashFromString(iKeyPad + message)));
}

std::string strip_weak_etag_marker(const std::string& str)
{
    static const std::string weak_etag_marker = "W/";
    if (str.rfind(weak_etag_marker, 0) == 0)
        return str.substr(weak_etag_marker.size());
    return str;
}
