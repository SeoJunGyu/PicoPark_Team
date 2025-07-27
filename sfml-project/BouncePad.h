#pragma once
#include "Gimmick.h"
class BouncePad :
    public Gimmick
{
protected:
    Player* player = nullptr;

    float bounce;

    bool isPressed = false;
    float pressTimer = 0.f;
    float pressDuration = 0.2f;

    sf::Vector2f preScale;

public:
    BouncePad(nlohmann::json j);

    void Reset() override;
    void Update(float dt) override;
};

