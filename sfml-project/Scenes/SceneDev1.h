#pragma once
#include "Scene.h"

class TextGo;
class SpriteGo;


class SceneDev1 : public Scene
{
protected:

	enum class CollisionMode
	{
		Rectangle,
		Circle
	};

	std::vector<sf::Texture> tiletextures;

	sf::RectangleShape* rect1;
	sf::RectangleShape* rect2;
	sf::CircleShape* circle1;
	sf::CircleShape* circle2;
	TextGo* collisionText;
	TextGo* modeText;

	int tileID = -1;
	int mapwidth;
	int mapheight;
	int tilesize;
	std::vector<std::vector<int>> mapArray;

	CollisionMode currentMode = CollisionMode::Rectangle;
	bool isColliding = false;
	bool showCheckbox = false;
	bool buttonClicked = false;
public:
	SceneDev1();
	~SceneDev1() override;

	void Init() override;
	void Enter() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

