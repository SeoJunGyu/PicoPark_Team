#pragma once
#include "Gimmick.h"
class Ghost :
    public Gimmick
{
    enum GhostState
    {
        Scanning,
        Alert,
        Chasing,
    };

protected:
    GhostState state = GhostState::Scanning;
    Player* target = nullptr;

    float detectionMeter = 0.f; //감지 게이지 0 ~ 1
    float meterDecayRate = 0.1f; //감지 게이지 줄어드는 속도
    float detectSecondPhase = 0.3f; //2단계
    float detectThreshold = 0.7f; //3단계
    float maxPlayerSpeed = 150.f; //플레이어 속도 최대치(일단)

public:
    Ghost(nlohmann::json j);

    void Reset() override;
    void Update(float dt) override;

    int sign(float v) { return (v > 0) ? 1 : ((v < 0) ? -1 : 0); }
};

