#pragma once
#include "GameObject.h"
class BackGround :
    public GameObject
{
protected:
    sf::Sprite sprite;


public:
    BackGround(const std::string& name ="");
    virtual ~BackGround() = default;

    void Settext(const std::string& textid);
    // GameObject을(를) 통해 상속됨
    void Init() override;
    void Release() override;
    void Reset() override;
    void Update(float dt) override;
    void Draw(sf::RenderWindow& window) override;
};

