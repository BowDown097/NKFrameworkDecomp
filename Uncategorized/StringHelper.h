#ifndef STRINGHELPER_H
#define STRINGHELPER_H
#include <string>
#include <vector>

namespace StringHelper
{
    std::string Format(std::string formatString, ...);
    std::string Join(const std::vector<std::string>& values, const std::string& separator);
}

#endif // STRINGHELPER_H
