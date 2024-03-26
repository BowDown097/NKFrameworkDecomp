#ifndef VECTORDATASTRUCTURES_H
#define VECTORDATASTRUCTURES_H
#include <string>

class CVec2
{
public:
    float x, y;
    CVec2(const class CVec2i& intVec2);
    CVec2(const std::string& pointString);
    std::string ToString();
    void Set(const std::string& pointString);
    void operator=(const std::string& pointString);
private:
    inline void Set(const std::string& pointString, bool zeroIfFail); // speculative
};

class CVec2i
{
public:
    int x, y;
    CVec2i(const CVec2& vec2);
    CVec2i(const CVec2* vec2);
};

#endif // VECTORDATASTRUCTURES_H
