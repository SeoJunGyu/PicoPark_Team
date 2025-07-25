#pragma once
#include "Scene.h"
#include "TileMap.h"

class Level;

class SceneGame : public Scene
{
protected:
	Level* level = nullptr;
	TileMap* tileMap;
	sf::Texture bgTex;
	sf::Sprite bgSpr;

public:
	SceneGame();
	~SceneGame() override;

	void Init() override;
	void Enter() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void LoadStage(const std::string& jsonPath);
	void ClearStage();
};

