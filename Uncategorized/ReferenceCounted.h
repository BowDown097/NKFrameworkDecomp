#pragma once

class CReferenceCounted
{
public:
    CReferenceCounted() : mRefCount(0x7FFFFFFF) {}
    virtual ~CReferenceCounted() = default;
private:
    mutable int mRefCount;
};
