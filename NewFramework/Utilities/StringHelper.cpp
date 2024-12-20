#include "StringHelper.h"
#include <cstdarg>

namespace StringHelper
{
    void Explode(const std::string& in, const std::string& sep, std::vector<std::string>* out)
    {
        std::string inCopy = in;
        size_t pos;
        std::string token;

        while ((pos = inCopy.find(sep)) != std::string::npos)
        {
            token = inCopy.substr(0, pos);
            out->push_back(token);
            inCopy.erase(0, pos + sep.size());
        }

        out->push_back(inCopy);
    }

    std::string Join(const std::vector<std::string>& values, const std::string& separator)
    {
        std::string out;
        if (values.empty())
            return out;

        size_t stringSize = 0;
        for (const std::string& value : values)
            stringSize += value.size();

        out.reserve(stringSize);
        out.append(values[0]);

        for (const std::string& value : values)
        {
            out.append(separator);
            out.append(value);
        }

        return out;
    }

    std::string Format(std::string formatString, ...)
    {
        static std::string sEncodingError = "Encoding error when calling StringHelper::Format()";
        static std::string sEmptyFormatStringError = "Empty format string passed to StringHelper::Format()";

        if (formatString.empty())
            return sEmptyFormatStringError;

        va_list va;
        va_start(va, formatString);

        int dryRunChars = vsnprintf(nullptr, 0, formatString.c_str(), va);
        va_end(va);
        if (dryRunChars < 0)
            return sEncodingError;

        std::string out;
        out.resize(dryRunChars);

        va_start(va, formatString);
        int realRunChars = vsnprintf(out.data(), out.size() + 1, formatString.c_str(), va);
        va_end(va);

        return realRunChars >= 0 ? out : sEncodingError;
    }

    std::string Replace(const std::string& string, const std::string& before, const std::string& after)
    {
        std::string out(string);
        size_t startPos = 0;
        while ((startPos = out.find(before, startPos)) != std::string::npos)
        {
            out.replace(startPos, before.size(), after);
            startPos += after.size();
        }

        return out;
    }
}
