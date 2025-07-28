#pragma once
#include "Gimmick.h"
class RoundPlatform :
    public Gimmick
{
    Player* player = nullptr;

    bool collidable = true; 
    bool isOne = false;

    float randFall = 100.f;

public:
    RoundPlatform(nlohmann::json j);

    void Reset() override;
    void Update(float dt) override;
};

