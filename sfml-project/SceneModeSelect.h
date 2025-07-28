#pragma once
#include "Scene.h"

class BackGround;
class UiButton;

class SceneModeSelect :   public Scene
{
protected:
	sf::Text text;
	sf::Text stagetext;

	BackGround* back;	

	UiButton* stage;



public:

	SceneModeSelect();
	~SceneModeSelect()override = default;
	
	void Init() override;
	void Enter() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	
};

