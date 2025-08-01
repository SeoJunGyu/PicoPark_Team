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
    float segLen = 36.f;      // 한 구간 최대 길이   
    float springK = 20.f;     // 끌어당기는 힘 크기
};

