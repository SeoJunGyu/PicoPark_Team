#pragma once
#include "Scene.h"

class UiButton;
class BackGround;
class PopupWindowUI;


class SceneTitle : public Scene
{
protected:
		
	UiButton* startButton;
	BackGround* titlebackground;	
	PopupWindowUI* popup;
	float cooltime = 0.f;
	bool coolOn = true;

public:
	SceneTitle();
	~SceneTitle()override = default;

	void Init() override;
	void Enter() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};