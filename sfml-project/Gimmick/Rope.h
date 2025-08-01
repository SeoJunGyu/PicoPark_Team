#pragma once
#include "Gimmick.h"
class Rope : public Gimmick
{
public:
    Rope(nlohmann::json j);         

    void Reset()   override;         
    void Update(float dt) override;  
    void Draw(sf::RenderWindow& w) override; 

private:
    std::vector<Player*> nodes;      
    float segLen = 36.f;      // �� ���� �ִ� ����   
    float springK = 20.f;     // ������� �� ũ��
};

