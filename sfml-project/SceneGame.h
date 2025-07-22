#pragma once
#include "Scene.h"

class Level;
class SceneGame : public Scene
{
protected:
	Level* level = nullptr;
public:
	SceneGame();
	~SceneGame() override;

	void Init() override;
	void Enter() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

