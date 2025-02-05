#pragma once
#include <sstream>
#include <vector>

enum class eEllipsesPosition { Front, Middle, End };

namespace StringHelper
{
    struct SCharAndHex
    {
        char _char;
        std::string hex;

        SCharAndHex(char _char, std::string hex)
            : _char(_char), hex(hex) {}
    };

    std::string Join(const std::vector<std::string>& values, const std::string& separator);
    void Explode(const std::string& in, const std::string& sep, std::vector<std::string>* out);
    void ExplodeInts(const std::string& in, char sep, std::vector<int>& out);
    void ExplodeFloats(const std::string& in, char sep, std::vector<float>& out);
    std::string Format(std::string formatString, ...);
    void Parse(const char*, ...) {} // appears to be unimplemented. maybe remnant of old framework revision
    void Parse(std::string, ...) {} // see above comment
    std::string Replace(const std::string& in, const std::string& before, const std::string& after);
    bool IsNewline(unsigned int c);
    bool IsTab(unsigned int c);
    bool IsWhitespace(unsigned int c);
    bool IsWordBreak(unsigned int c);
    bool IsAllWhiteSpace(const std::string& in);
    std::string TrimLeadingWhiteSpace(std::string in);
    std::string TrimTrailingWhiteSpace(std::string in);
    std::string TrimLeadingAndTrailingWhiteSpace(std::string in);
    bool CompareNoCase(const std::string& s1, const std::string& s2);
    std::string ToLower(const std::string& in);
    std::string ToUpper(const std::string& in);
    std::string EscapeString(const std::string& in);
    std::string UnescapeString(const std::string& in);
    std::string SubStringUTF8(const std::string& in, size_t pos, size_t len = std::string::npos);
    std::string TruncateStringWithEllipses(const std::string& in, unsigned int len, eEllipsesPosition ellipsesPosition);
    std::string AddDigitGrouping(const std::string& num, const std::string& sep);
    std::string CreateDigitGrouping(unsigned int num, const std::string& sep);
    std::string CreateDigitGroupingAllowNegative(int num, const std::string& sep);
    unsigned int CalculateFNV(const char* in, unsigned long hash = 0x811C9DC5, unsigned long prime = 0x1000193);
    unsigned int GetHash(const char* in);
    void Split(const std::string& in, const std::string& sep, std::vector<std::string>& out);
    std::string ReverseStringUTF8(const std::string& in);
    int CountWordsInString(const std::string& in);
    int CountOccurrences(const std::string& str, const std::string& sub);
    size_t FindNthOccurrence(const std::string& str, const std::string& sub, int n);
    std::string SigFloatFormat(std::string formatString, float num, unsigned int decimals, bool plusPrefix = false);
    std::string SigFloat(float num, unsigned int decimals, bool plusPrefix = false);
    std::string ReplaceBadCharactersInFilename(const std::string& in, char bad, char replace);
    void SplitPathAndFilename(const std::string& in, std::string& path, std::string& filename);

    template<typename T>
    std::string ValueToString(T value)
    {
        std::stringstream ss;
        ss << value;
        return ss.str();
    }
}
