#pragma once
#include "Gimmick.h"

class Player;

class Key :
    public Gimmick
{
protected:
    Player* player = nullptr;
    Player* newPlayer = nullptr;

public:
    Key(nlohmann::json j);

    void Reset() override;
    void Update(float dt) override;
};

