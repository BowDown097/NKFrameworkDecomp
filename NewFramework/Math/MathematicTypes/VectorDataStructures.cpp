#include "VectorDataStructures.h"
#include "NewFramework/Platform/Shared/Logging.h"
#include "NewFramework/Utilities/StringHelper.h"

CVec2::CVec2(const CVec2i& intVec2) : x(intVec2.x), y(intVec2.y) {}

CVec2::CVec2(const std::string& pointString)
{
    Set(pointString, true);
}

std::string CVec2::ToString()
{
    return StringHelper::Format("%f,%f", x, y);
}

void CVec2::Set(const std::string& pointString)
{
    Set(pointString, false);
}

void CVec2::operator=(const std::string& pointString)
{
    Set(pointString, false);
}

void CVec2::Set(const std::string& pointString, bool zeroIfFail)
{
    std::vector<std::string> points;
    StringHelper::Explode(pointString, ",", &points);

    if (points.size() != 2)
    {
        LOG_ERROR("Incorrect number of components (%d) in string '%s'.", points.size(), pointString.c_str());
        if (zeroIfFail)
            x = y = 0;
        return;
    }

    x = atof(points[0].c_str());
    y = atof(points[1].c_str());
}

CVec2i::CVec2i(const CVec2& vec2)
{
    x = vec2.x;
    y = vec2.y;
}

CVec2i::CVec2i(const CVec2* vec2)
{
    x = vec2->x;
    y = vec2->y;
}
