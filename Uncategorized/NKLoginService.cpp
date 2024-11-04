#include "NKLoginService.h"
#include <algorithm>

void I_NKLoginService::Attach(I_NKLoginServiceObserver* observer)
{
    auto it = std::find(observers.begin(), observers.end(), observer);
    if (it != observers.end())
        observers.push_back(observer);
}

void I_NKLoginService::Detach(I_NKLoginServiceObserver* observer)
{
    auto it = std::find(observers.begin(), observers.end(), observer);
    if (it != observers.end())
        observers.erase(it);
}
