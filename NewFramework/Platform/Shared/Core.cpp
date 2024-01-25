#include "Core.h"
#include <cstring>

std::string CCore::ThreadSafeStrError(int errnum)
{
#ifdef __unix__
    char* buf = new char[512];
    strerror_r(errnum, buf, sizeof(buf));
    std::string outString(buf);
    delete[] buf;
    return outString;
#else
    return strerror(errnum);
#endif
}
