﻿#pragma once
#include "Gimmick.h"
class MovingPlatform :
    public Gimmick
{
    enum MState
    {
        Idle,
        Ascending,
        Descending,
    };

protected:
    int channel = 0; //같은 채널의 플랫폼을 이동시키기 위해 사용

    int requireCount = 0; //몇명을 태워야 이동하는지 확인
    sf::Text countText;

    sf::Vector2f startPos;
    sf::Vector2f endPos;
    sf::Vector2f prevPos; //이동 전 위치 저장
    sf::Vector2f deltaPos;

    float speed;
    float prvSpeed;
    int dir; //+1 : 끝으로 출발 , -1 : 시작으로 출발 , 0 : 정지

    sf::Vector2f unitDir; //방향 벡터
    float pathLength = 0.f; //왕복거리

    bool blocked = false; //플레이어 머리에 닿을 시 정지

    bool loop;

    MState state = MState::Idle; //플랫폼 움직임 방향

public:
    MovingPlatform(nlohmann::json j);

    void Reset() override;
    void Update(float dt) override;
    void Draw(sf::RenderWindow& window) override;

    void moveOneStep(float dt);

    sf::Vector2f GetDeltaPos() const { return deltaPos; }

    bool IsBlocked();
};

