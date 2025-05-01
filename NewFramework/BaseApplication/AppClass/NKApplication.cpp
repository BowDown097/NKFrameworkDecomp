#include "NKApplication.h"

#include "AppClass.h"
#include "Uncategorized/GlobalVariables.h"

A_NKApplication::A_NKApplication(CApp* pApp)
    : m_pApp(pApp) {}

bool A_NKApplication::IsOnline() {
    return m_pApp->GetGoogleReachable() != static_cast<eREACH_Availability>(0);
}

int A_NKApplication::GetScreenW() {
    return CGlobalVariables::Instance()->screenWidth;
}

int A_NKApplication::GetScreenH() {
    return CGlobalVariables::Instance()->screenHeight;
}

float A_NKApplication::GetViewScale() {
    return CGlobalVariables::Instance()->viewScale;
}

std::string A_NKApplication::GetLanguageCode() {
    return m_pApp->GetLanguageCode();
}

int A_NKApplication::GetVersionCode() {
    return m_pApp->version.versionCode;
}

int A_NKApplication::GetAgeGate() {
    return m_pApp->GetAgeGate();
}
