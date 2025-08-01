#pragma once
#include "Gimmick.h"
class Portal :
    public Gimmick
{
public:
    int  channel = 0; // 내 채널
    int  destChannel = -1; // 도착 채널
    Portal* dest = nullptr;
    float cooldownSec = 1.0f;// 중복 전송 방지용 쿨타임

protected:
    bool loop = false;
    std::vector<Player*> entered;

public:
    Portal(nlohmann::json j);

    void Reset() override;
    void Update(float dt) override;

    void Teleport(Player* p);
};

