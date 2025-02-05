#include "StringHelper.h"
#include "NewFramework/ThirdParty/utfcpp/utf8/checked.h"
#include <boost/algorithm/string/case_conv.hpp>
#include <cstdarg>

namespace StringHelper
{
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

    void Explode(const std::string& in, const std::string& sep, std::vector<std::string>* out)
    {
        std::string inCopy = in;
        size_t pos;

        while ((pos = inCopy.find(sep)) != std::string::npos)
        {
            out->push_back(inCopy.substr(0, pos));
            inCopy.erase(0, pos + sep.size());
        }

        out->push_back(inCopy);
    }

    void ExplodeInts(const std::string& in, char sep, std::vector<int>& out)
    {
        std::string inCopy = in;
        size_t pos;

        while ((pos = inCopy.find(sep)) != std::string::npos)
        {
            out.push_back(atoi(inCopy.substr(0, pos).c_str()));
            inCopy.erase(0, pos + 1);
        }

        out.push_back(atoi(inCopy.c_str()));
    }

    void ExplodeFloats(const std::string& in, char sep, std::vector<float>& out)
    {
        std::string inCopy = in;
        size_t pos;

        while ((pos = inCopy.find(sep)) != std::string::npos)
        {
            out.push_back(atof(inCopy.substr(0, pos).c_str()));
            inCopy.erase(0, pos + 1);
        }

        out.push_back(atof(inCopy.c_str()));
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

    std::string Replace(const std::string& in, const std::string& before, const std::string& after)
    {
        std::string out = in;
        size_t startPos = 0;

        while ((startPos = out.find(before, startPos)) != std::string::npos)
        {
            out.replace(startPos, before.size(), after);
            startPos += after.size();
        }

        return out;
    }

    bool IsNewline(unsigned int c)
    {
        return c == '\n';
    }

    bool IsTab(unsigned int c)
    {
        return c == '\t';
    }

    bool IsWhitespace(unsigned int c)
    {
        return c == '\t' || c == '\n' || c == ' ' || c == 160 || c == 12288;
    }

    bool IsWordBreak(unsigned int c)
    {
        return c == '\t' || c == '\n' || c == ' ' || c == '-' || c == 160 || c == 12288;
    }

    bool IsAllWhiteSpace(const std::string& in)
    {
        return std::all_of(in.begin(), in.end(), IsWhitespace);
    }

    std::string TrimLeadingWhiteSpace(std::string in)
    {
        if (IsAllWhiteSpace(in))
            return "";

        in.erase(in.begin(), std::find_if_not(in.begin(), in.end(), IsWhitespace));
        return in;
    }

    std::string TrimTrailingWhiteSpace(std::string in)
    {
        if (IsAllWhiteSpace(in))
            return "";

        in.erase(std::find_if_not(in.rbegin(), in.rend(), IsWhitespace).base(), in.end());
        return in;
    }

    std::string TrimLeadingAndTrailingWhiteSpace(std::string in)
    {
        if (IsAllWhiteSpace(in))
            return "";
        return TrimLeadingWhiteSpace(TrimTrailingWhiteSpace(in));
    }

    bool CompareNoCase(const std::string& s1, const std::string& s2)
    {
        for (size_t i = 0; i < std::min(s1.size(), s2.size()); ++i)
        {
            char c1 = tolower(s1[i]);
            char c2 = tolower(s2[i]);
            if (c1 != c2)
                return c1 < c2;
        }

        return s1.size() < s2.size();
    }

    std::string ToLower(const std::string& in)
    {
        return boost::algorithm::to_lower_copy(in);
    }

    std::string ToUpper(const std::string& in)
    {
        return boost::algorithm::to_upper_copy(in);
    }

    std::string EscapeString(const std::string& in)
    {
        static std::vector<SCharAndHex> _vectorCharHex;
        static char CharsToEscape[] = " $&`:<>[]{}\"+#%@/;=?\\^|~',";

        if (_vectorCharHex.empty())
        {
            for (int i = 0; i < sizeof(CharsToEscape) - 1; ++i)
            {
                char c = CharsToEscape[i];
                _vectorCharHex.emplace_back(c, Format("%%%x", c));
            }
        }

        std::string out;

        for (char c : in)
        {
            auto it = std::find_if(_vectorCharHex.begin(), _vectorCharHex.end(), [c](const SCharAndHex& charAndHex) {
                return charAndHex._char == c;
            });

            if (it != _vectorCharHex.end())
                out += it->hex;
            else
                out += c;
        }

        return out;
    }

    std::string UnescapeString(const std::string& in)
    {
        std::string out;

        for (int i = 0; i < in.size(); ++i)
        {
            if (in[i] != '%' || i + 2 >= in.size())
            {
                out += in[i];
            }
            else
            {
                std::string hex = in.substr(i + 1, 2);
                out += strtol(hex.c_str(), nullptr, 16);
            }
        }

        return out;
    }

    std::string SubStringUTF8(const std::string& in, size_t pos, size_t len)
    {
        if (in.empty())
            return "";

        size_t utf8size = utf8::distance(in.begin(), in.end());
        if (utf8size <= pos || !len)
            return "";

        auto begin = in.begin();
        utf8::advance(begin, pos, in.end());

        auto end = begin;
        if (len == std::string::npos)
            end = in.end();
        else
            utf8::advance(end, std::min(utf8size - pos, len), in.end());

        return std::string(begin, end);
    }

    std::string TruncateStringWithEllipses(const std::string& in, unsigned int len, eEllipsesPosition ellipsesPosition)
    {
        std::string ellipses(3, '.');

        if (in.empty())
            return "";

        size_t utf8size = utf8::distance(in.begin(), in.end());
        if (utf8size - 1 < len)
            return "";
        if (len <= 3)
            return ellipses.substr(len);

        switch (ellipsesPosition)
        {
        case eEllipsesPosition::Front:
            return ellipses + SubStringUTF8(in, utf8size - (len - 3));
        case eEllipsesPosition::Middle:
        {
            std::string firstHalf = SubStringUTF8(in, 0, utf8size / 2);
            std::string secondHalf = SubStringUTF8(in, utf8size / 2);

            size_t firstHalfUtf8Size = utf8::distance(firstHalf.begin(), firstHalf.end());
            size_t secondHalfUtf8Size = utf8::distance(secondHalf.begin(), secondHalf.end());

            while (firstHalfUtf8Size + secondHalfUtf8Size + 3 > len)
            {
                if (firstHalfUtf8Size <= secondHalfUtf8Size)
                {
                    secondHalf = SubStringUTF8(secondHalf, 1);
                    --secondHalfUtf8Size;
                }
                else
                {
                    firstHalf = SubStringUTF8(firstHalf, 0, --firstHalfUtf8Size);
                }
            }

            return firstHalf + ellipses + secondHalf;
        }
        default:
            return SubStringUTF8(in, 0, len - 3) + ellipses;
        }
    }

    std::string AddDigitGrouping(const std::string& num, const std::string& sep)
    {
        std::string out;
        std::string absNum = num;

        if (!num.empty() && num.front() == '-')
        {
            absNum = num.substr(1);
            out = num.front();
        }

        if (absNum.size() + 2 >= 5 || absNum.size() % 3 != 0)
        {
            for (int i = 0; i < absNum.size(); ++i)
            {
                out += absNum[i];
                if ((i + 1 - (absNum.size() % 3)) % 3 == 0 && i != absNum.size() - 1)
                    out += sep;
            }
        }
        else
        {
            out = num;
        }

        return out;
    }

    std::string CreateDigitGrouping(unsigned int num, const std::string& sep)
    {
        std::string remainder;

        while (num >= 1000)
        {
            remainder = sep + Format("%03d", num % 1000) + remainder;
            num /= 1000;
        }

        return Format("%d%s", num, remainder.c_str());
    }

    std::string CreateDigitGroupingAllowNegative(int num, const std::string& sep)
    {
        std::string prefix;

        if (num < 0)
        {
            prefix = "-";
            num = -num;
        }

        std::string grouping = CreateDigitGrouping(num, sep);
        grouping.insert(0, prefix);
        return grouping;
    }

    unsigned int CalculateFNV(const char* in, unsigned long hash, unsigned long prime)
    {
        while (*in)
            hash = prime * (*in++ ^ hash);

        return hash;
    }

    unsigned int GetHash(const char* in)
    {
        unsigned int result = 0;

        while (*in)
            result = *in++ ^ (result + 32 * result);

        return result;
    }

    void Split(const std::string& in, const std::string& sep, std::vector<std::string>& out)
    {
        size_t posStart = 0, posEnd;

        while ((posEnd = in.find(sep, posStart)) != std::string::npos)
        {
            out.push_back(in.substr(posStart, posEnd - posStart));
            posStart = posEnd + sep.size();
        }

        out.push_back(in.substr(posStart));
    }

    std::string ReverseStringUTF8(const std::string& in)
    {
        std::string out = in;
        int i = 0;

        for (auto it = in.begin(); it != in.end(); utf8::next(it, in.end()))
        {
            size_t sequenceLength = utf8::internal::sequence_length(it);
            out.insert(out.size() - (i + sequenceLength), in, i, sequenceLength);
            i += sequenceLength;
        }

        return out;
    }

    int CountWordsInString(const std::string& in)
    {
        std::vector<std::string> words;
        Split(in, " ", words);
        return words.size();
    }

    int CountOccurrences(const std::string& str, const std::string& sub)
    {
        int result = 0;
        size_t pos = 0;

        while ((pos = str.find(sub, pos)) != std::string::npos)
        {
            ++result;
            pos += sub.size();
        }

        return result;
    }

    size_t FindNthOccurrence(const std::string& str, const std::string& sub, int n)
    {
        size_t pos = 0;

        while (n-- > 0)
        {
            pos += sub.size();
            if ((pos = str.find(sub, pos)) == std::string::npos)
                return std::string::npos;
        }

        return pos;
    }

    std::string SigFloatFormat(std::string formatString, float num, unsigned int decimals, bool plusPrefix)
    {
        return Format(formatString, SigFloat(num, decimals, plusPrefix).c_str());
    }

    std::string SigFloat(float num, unsigned int decimals, bool plusPrefix)
    {
        std::string numFormatString = "%f";
        if (plusPrefix)
            numFormatString = Format("%%+.%df", decimals);
        else
            numFormatString = Format("%%.%df", decimals);

        std::string formattedNum = Format(numFormatString, num);

        char c;
        while ((c = formattedNum[formattedNum.size() - 1]) == '0')
            formattedNum = formattedNum.substr(0, formattedNum.size() - 1);
        if (c == '.')
            formattedNum = formattedNum.substr(0, formattedNum.size() - 1);

        return formattedNum;
    }

    std::string ReplaceBadCharactersInFilename(const std::string& in, char bad, char replace)
    {
        std::string out = in;

        auto it = out.begin();
        size_t pos = out.find_last_of("/\\");
        if (pos != std::string::npos)
            it += pos;

        for (; it != out.end(); ++it)
            if (*it == bad)
                *it = replace;

        return out;
    }

    void SplitPathAndFilename(const std::string& in, std::string& path, std::string& filename)
    {
        size_t pos = in.find_last_of("/\\");

        if (pos == std::string::npos)
        {
            path = "";
            filename = in;
        }
        else
        {
            path = in.substr(0, pos);
            filename = in.substr(pos + 1);
        }
    }
}
