#pragma once
#include "Scene.h"
class SceneTitle : public Scene
{
protected:
	sf::Sprite titleBackground;
	sf::Text titleMessage;

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