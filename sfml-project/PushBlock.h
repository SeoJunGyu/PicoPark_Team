#pragma once
#include "Gimmick.h"

class TileMap;

class PushBlock :
    public Gimmick
{
protected:
    Player* player = nullptr;

    int requireCount; //����� �о���ϴ��� Ȯ��

    sf::Color blockColor; //���� ���� ���� �÷��̾ �̴��� Ȯ��

    sf::Vector2f moveDir; //�̵�����
    float moveSpeed; //�и��� �ӵ�

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

