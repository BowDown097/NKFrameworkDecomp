#pragma once
#include <string>

struct I_NKSDKApp
{
    virtual ~I_NKSDKApp() = default;
    virtual bool IsOnline() = 0;
    virtual int GetScreenW() = 0;
    virtual int GetScreenH() = 0;
    virtual float GetViewScale() = 0;
    virtual std::string GetLanguageCode() = 0;
    virtual int GetVersionCode() = 0;
    virtual int GetAgeGate() = 0;
};
