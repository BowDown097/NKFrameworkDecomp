#ifndef CORE_H
#define CORE_H
#include <string>

class CCore
{
public:
    static std::string ThreadSafeStrError(int errnum);
};

#endif // CORE_H
