#ifndef STRINGHELPER_H
#define STRINGHELPER_H
#include <string>
#include <vector>

namespace StringHelper
{
    size_t Explode(const std::string& in, const std::string& sep, std::vector<std::string>* out);
    std::string Join(const std::vector<std::string>& values, const std::string& separator);
    std::string Format(std::string formatString, ...);
    std::string Replace(const std::string& string, const std::string& before, const std::string& after);
}

#endif // STRINGHELPER_H
