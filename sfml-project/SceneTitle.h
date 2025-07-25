#pragma once
#include "Scene.h"

class UiButton;
class BackGround;

class SceneTitle : public Scene
{
protected:
		
	UiButton* startButton;
	BackGround* titlebackground;

	float cooltime = 0.f;
	bool coolOn = true;

public:
	SceneTitle();
	virtual ~SceneTitle() =default;

	void Init() override;
	void Enter() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};