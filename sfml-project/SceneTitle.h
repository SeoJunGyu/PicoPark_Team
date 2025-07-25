#pragma once
#include "Scene.h"
#include "UiButton.h"

class SceneTitle : public Scene
{
protected:
	sf::Sprite titleBackground;
	sf::Sprite menubox;
	sf::Text titleMessage;
	
	UiButton startButton;

	float cooltime = 0.f;
	bool coolOn = true;

public:
	SceneTitle();
	~SceneTitle() = default;

	void Init() override;
	void Enter() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};