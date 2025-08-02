#pragma once
#include "GameObject.h"

class UiButton;

class PauseUI :
    public GameObject
{
protected:
    sf::Sprite sprite;
  
    std::vector<UiButton*> pusebuts;
    UiButton* returnbut;
    UiButton* retrybut;
    UiButton* selectbut;
    UiButton* titlebut;

    UiButton* closebut;

    sf::FloatRect spr;

    float layout = 50.f;

    int currentPage = 0;

    bool found = false;

public:
    PauseUI(const std::string name = "");
    ~PauseUI() override = default;

    void SetPosition(const sf::Vector2f& pos) override;
    void SetRotation(float rot) override;
    void SetScale(const sf::Vector2f& s) override;
    void SetOrigin(const sf::Vector2f& o) override;
    void SetOrigin(Origins preset) override;

    // GameObject을(를) 통해 상속됨
    void Init() override;

    void Release() override;

    void Reset() override;

    void Update(float dt) override;

    void Draw(sf::RenderWindow& window) override;

};

