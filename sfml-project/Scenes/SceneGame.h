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

	static std::string pendingStage;
	static bool isEditor;

	bool  stageClear = false;
	float clearTime = 0.f;

	sf::Text  clearTxt;
	sf::Font clearFont;
	sf::Vector2f fromPos;   // (-Æø, y)
	sf::Vector2f toPos;     // (¼¾ÅÍ, y)


	bool enterdoor = false;
	bool gameClearPlayed = false;

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
	void StartStageClear();
	static void SetPendingStage(const std::string& p) { pendingStage = p; }
	static void applyLetterBox(sf::View& v, sf::Vector2u win);
	static void SetEditor(const bool& b) { isEditor = b; }
	static const bool& GetEditor() { return isEditor; }
};

