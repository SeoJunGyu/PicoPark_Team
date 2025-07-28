#pragma once
#include "Gimmick.h"
#include "Player.h"

class TileMap;

class PushBlock :
    public Gimmick
{
public:
    sf::Vector2f velocity{ 0.f, 0.f };
    bool isGrounded = false;
    bool hasRider = false; //내 머리 위에 누가 올라탔음
    bool isOnPlayer = false; //내가 다른사람 밟고 있음

    StandMethod standing;

protected:
    Player* player = nullptr;

    int requireCount; //몇명이 밀어야하는지 확인

    int colorIndex;
    sf::Color blockColor; //블럭의 색과 같은 플레이어가 미는지 확인
    bool colorPush = false;

    sf::Vector2f moveDir; //이동방향

    sf::Vector2f gravity = { 0.f, 500.f };

    TileMap* tilemap;
    sf::Vector2f prvPos;

public:
    PushBlock(nlohmann::json j);

    void Reset() override;
    void Update(float dt) override;

    sf::Vector2f GetSupportDelta();
    void ApplySupport();
    void ScanUpStack();
};

