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

	const float VIEW_W = 160.f;     // 임시값
	const float VIEW_H = 96.f;     // 임시값

	static std::string pendingStage;

public:
	SceneGame();
	~SceneGame() override;

	void Init() override;
	void Enter() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void LoadStage(const std::string& jsonPath);
	void ClearStage();
	void updateCamera(float dt);
	static void SetPendingStage(const std::string& p) { pendingStage = p; }
};

