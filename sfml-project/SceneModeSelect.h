#pragma once
#include "Scene.h"

class BackGround;
class UiButton;

class SceneModeSelect :   public Scene
{
protected:
	sf::Text text;


	BackGround* back;

	UiButton* mood1;

	UiButton* stage1;



public:

	SceneModeSelect();
	~SceneModeSelect()override = default;
	
	void Init() override;
	void Enter() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	
};

