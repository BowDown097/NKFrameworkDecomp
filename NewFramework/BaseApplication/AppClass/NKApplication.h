#pragma once
#include "NKSDKApp.h"

class CApp;

class A_NKApplication : public I_NKSDKApp
{
public:
    A_NKApplication(CApp* app) : app(app) {}
    bool IsOnline() override;
    int GetScreenW() override;
    int GetScreenH() override;
    float GetViewScale() override;
    std::string GetLanguageCode() override;
    int GetVersionCode() override;
    int GetAgeGate() override;
private:
    CApp* app;
};
