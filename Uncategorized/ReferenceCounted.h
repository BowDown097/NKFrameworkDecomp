#pragma once

#include <climits>

class CReferenceCounted
{
public:
    mutable int mRefCount = INT_MAX;
    virtual ~CReferenceCounted() = default;
};
