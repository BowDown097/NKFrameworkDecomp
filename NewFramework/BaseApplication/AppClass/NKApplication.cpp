#include "NKApplication.h"
#include "AppClass.h"
#include "Uncategorized/GlobalVariables.h"

bool A_NKApplication::IsOnline()
{
    return app->GetGoogleReachable() != static_cast<eREACH_Availability>(0);
}

int A_NKApplication::GetScreenW()
{
    return CGlobalVariables::Instance()->screenWidth;
}

int A_NKApplication::GetScreenH()
{
    return CGlobalVariables::Instance()->screenHeight;
}

float A_NKApplication::GetViewScale()
{
    return CGlobalVariables::Instance()->viewScale;
}

std::string A_NKApplication::GetLanguageCode()
{
    return app->GetLanguageCode();
}

int A_NKApplication::GetVersionCode()
{
    return app->version.versionCode;
}

int A_NKApplication::GetAgeGate()
{
    return app->GetAgeGate();
}
