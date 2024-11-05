#pragma once

#include <string>

class CBaseScreen;
class CAssetBag {
public:
    enum eAssetType {
        TEXTURE
    };

    explicit CAssetBag(CBaseScreen* screen) {};

    bool Ready();
    void Reset();
    void StartUsing(const eAssetType& type, const std::string&);
};