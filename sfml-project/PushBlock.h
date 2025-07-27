#pragma once
#include "Gimmick.h"

class TileMap;

class PushBlock :
    public Gimmick
{
protected:
    Player* player = nullptr;

    int requireCount; //몇명이 밀어야하는지 확인

    sf::Color blockColor; //블럭의 색과 같은 플레이어가 미는지 확인

    sf::Vector2f moveDir; //이동방향
    float moveSpeed; //밀리는 속도

    sf::Vector2f velocity{ 0.f, 0.f };
    sf::Vector2f gravity = { 0.f, 500.f };
    bool isGrounded = false;

    TileMap* tilemap;
    sf::Vector2f prvPos;

public:
    PushBlock(nlohmann::json j);

    void Reset() override;
    void Update(float dt) override;
};

