#pragma once
#include "Gimmick.h"
class RoundPlatform :
    public Gimmick
{
    Player* player = nullptr;

    bool collidable = true; 
    bool isOne = false;

    float randFall = 100.f;

    int channel;
    bool active = false;

    bool playerKill = false;
    sf::RectangleShape thickLine;

public:
    RoundPlatform(nlohmann::json j);

    void Reset() override;
    void Update(float dt) override;
    void Draw(sf::RenderWindow window);

    void UpdateChannel();
};

