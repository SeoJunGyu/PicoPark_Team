#pragma once
#include "GameObject.h"

class UiButton;

class YesNoPopupUI :
    public GameObject
{
protected:
    sf::Sprite sprite;
    sf::Text text;
  

    
    UiButton* nobut;
    UiButton* closebut;
        
    sf::FloatRect spr;

    sf::RectangleShape outline;
    float animTime = 0.f;
    float speed = 5.f;       
    float amplitude = 0.05f; 


public:
    UiButton* yesbut;

    YesNoPopupUI(const std::string name ="");
    ~YesNoPopupUI() override = default;

    void SetPosition(const sf::Vector2f& pos) override;
    void SetRotation(float rot) override;
    void SetScale(const sf::Vector2f& s) override;
    void SetOrigin(const sf::Vector2f& o) override;
    void SetOrigin(Origins preset) override;
    void SetText(const std::string& t, const std::string& fontid, int size);
    /*void Choiceevent(const UiButton* t, float dt);*/

    void Init() override;

    void Release() override;

    void Reset() override;

    void Update(float dt) override;

    void Draw(sf::RenderWindow& window) override;

};

