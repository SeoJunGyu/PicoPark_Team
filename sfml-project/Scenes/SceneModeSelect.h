#pragma once
#include "Scene.h"
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

class BackGround;
class UiButton;

class SceneModeSelect :   public Scene
{
protected:
	sf::Text text;
	//sf::Text stagetext;

	BackGround* back;	

	//UiButton* stage;

	struct StageEntry {
		std::string file;   // levels/stage_01.json
		int index;
		UiButton* btn;

		std::string author;
		std::string desc;
	};

	std::vector<StageEntry> stages;   
	const fs::path LEVEL_DIR = "levels";



public:

	SceneModeSelect();
	~SceneModeSelect()override = default;
	
	void Init() override;
	void Enter() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	
};

