#include "HashHelper.h"
#include "NewFramework/IO/File/IFile.h"
#include <hl_md5.h>
#include <iomanip>

namespace HashHelper {
    std::string BufferToHexString(uint8_t const* buffer, unsigned int size) {
        std::ostringstream oss;
        oss << std::hex << std::setfill('0');

        for (int i = 0; i < size; ++i) {
            oss << std::setw(2) << buffer[i];
        }

        return oss.str();
    }

    int64_t HexStringToValue(const std::string& str) {
        int64_t value{};
        std::stringstream ss(str);
        ss >> std::hex >> value;
        return value;
    }

    std::string MD5String(const std::string& str) {
        MD5 md5;
        unsigned char buf[16];

        HL_MD5_CTX ctx{};
        md5.MD5Init(&ctx);
        md5.MD5Update(&ctx, reinterpret_cast<uint8_t*>(const_cast<char*>(str.data())), str.size());
        md5.MD5Final(buf, &ctx);

        return BufferToHexString(buf, sizeof(buf));
    }

    std::string MD5Data(uint8_t* data, size_t size) {
        MD5 md5;
        unsigned char buf[16];

        HL_MD5_CTX ctx{};
        md5.MD5Init(&ctx);
        md5.MD5Update(&ctx, data, size);
        md5.MD5Final(buf, &ctx);

        return BufferToHexString(buf, sizeof(buf));
    }

    std::string MD5File(IFile* file) {
        MD5 md5;
        unsigned char buf[16];

        HL_MD5_CTX ctx{};
        md5.MD5Init(&ctx);

        std::vector<unsigned char> data(0x100000);

        while (file->GetOffset() != file->GetFileLength()) {
            size_t read = file->ReadBytes(data.data(), data.size());
            md5.MD5Update(&ctx, data.data(), read);
        }

        md5.MD5Final(buf, &ctx);
        return BufferToHexString(buf, sizeof(buf));
    }
}
