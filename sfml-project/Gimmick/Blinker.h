#pragma once
#include "Gimmick.h"
class Blinker :
    public Gimmick
{
    enum State
    {
        Green,
        Yellow,
        Red,
    };

protected:
    float minTime; //최소 시간
    float maxTime; //최대 시간

    float totalTime;
    float currentTime;
    float resetTime;
    float resetDuration;

    float changeYellow; //주황불로 전환 시간 - totalTime * 0.3 -> 30% 남은 시간일때

    float rnd;

    sf::Texture texGreen;
    sf::Texture texYellow;
    sf::Texture texRed;

    State state;

    bool playerKill = false;
    bool enterRed = false;

public:
    Blinker(nlohmann::json j);

    void Reset() override;
    void Update(float dt) override;
};

