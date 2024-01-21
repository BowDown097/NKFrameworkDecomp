#include "DGAnalytics.h"

DGAnalytics* DGAnalytics::Instance()
{
    if (!_pInstance)
        _pInstance = new DGAnalytics;
    return _pInstance;
}
