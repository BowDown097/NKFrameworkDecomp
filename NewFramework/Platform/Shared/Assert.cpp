#include "Assert.h"
#include "NewFramework/Utilities/StringHelper.h"
#include "Logging.h"

bool _NKAssert(bool condition, const char* message, const char* file, const char* function, int line)
{
    if (condition)
        return condition;

    LOG_ERROR(StringHelper::Format("%s in %s %s, line %d", message, file, function, line).c_str());
    /*
    DGAnalyticsData analyticsData("NKAssert");
    analyticsData.AddPair("message", message);
    analyticsData.AddPair("file", file);
    analyticsData.AddPair("function", function);
    analyticsData.AddPair("line", line);
    DGAnalytics::Instance()->SendDataEvent(analyticsData, true, 6, 2);
    */
    return false;
}
