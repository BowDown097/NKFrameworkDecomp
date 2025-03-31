#pragma once

#include "NewFramework/AdServer/AdInterface.h"

class C_IronSourceInterface : public C_AdInterface {
public:
    C_IronSourceInterface(C_AdManager*, const sAdProviderData&, const sCurrencyData&) {}
    ~C_IronSourceInterface() override = default;
};
