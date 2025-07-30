#pragma once
#include "Gimmick.h"
class FallingPlatform :
    public Gimmick
{
protected:
    Player* player = nullptr;

    float fallDelay;
    float respawnDelay;
    float fallTimer = 0.f;
    float respawnTimer = 0.f;

    bool collidable = true; //�浹 ��Ȱ��ȭ �÷���

public:
    FallingPlatform(nlohmann::json j);

    void Reset() override;
    void Update(float dt) override;
};

