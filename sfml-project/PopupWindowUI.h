#pragma once
#include "GameObject.h"

class UiButton;

class PopupWindowUI :
    public GameObject
{
protected:    
    sf::Sprite sprite;
    std::vector<UiButton*> startbut;
    std::vector<UiButton*> moodchoi;
    UiButton* Local;
    UiButton* Option;
    UiButton* Exit;





public:
    PopupWindowUI(const std::string name ="");
    ~PopupWindowUI() override = default;

    void SetPosition(const sf::Vector2f& pos) override;
    void SetRotation(float rot) override;
    void SetScale(const sf::Vector2f& s) override;
    void SetOrigin(const sf::Vector2f& o) override;
    void SetOrigin(Origins preset) override;

    
    void Init() override;

    void Release() override;

    void Reset() override;

    void Update(float dt) override;

    void Draw(sf::RenderWindow& window) override;

};

