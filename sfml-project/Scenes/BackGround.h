#pragma once
#include "GameObject.h"
class BackGround :
    public GameObject
{
protected:
    sf::Sprite sprite;
    

    
    float fadeAlpha = 0.f;
    float fadeSpeed = 255.f;
    bool fadeComplete = false;

public:
    sf::RectangleShape whiteOverlay;    

    static bool isFadingout;

    BackGround(const std::string& name ="");
    virtual ~BackGround() = default;

    void Settext(const std::string& textid);
    // GameObject을(를) 통해 상속됨
    void Init() override;
    void Release() override;
    void Reset() override;
    void Update(float dt) override;
    void Draw(sf::RenderWindow& window) override;

    void StartFadeOut();
    bool IsFadeOutComplete() const;
};

