#pragma once
#include "NKSDKApp.h"

class CApp;

class A_NKApplication : public I_NKSDKApp {
public:
    explicit A_NKApplication(CApp* pApp);
    bool IsOnline() override;
    int GetScreenW() override;
    int GetScreenH() override;
    float GetViewScale() override;
    std::string GetLanguageCode() override;
    int GetVersionCode() override;
    int GetAgeGate() override;
private:
    CApp* m_pApp;
};
