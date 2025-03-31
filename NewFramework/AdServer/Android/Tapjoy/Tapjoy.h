#pragma once

#include "NewFramework/AdServer/AdInterface.h"

class C_TapjoyInterface : public C_AdInterface {
public:
    C_TapjoyInterface(C_AdManager*, const sAdProviderData&, const sCurrencyData&) {}
    ~C_TapjoyInterface() override = default;
};
