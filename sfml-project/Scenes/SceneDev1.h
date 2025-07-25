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



	int tileID = -1; //Ÿ�� �⺻�� None
	int gridwidth = 10; //�׸��� �ʺ�
	int gridheight = 6; //����
	int mapwidth; 
	int mapheight;
	int tilesize;

	sf::RectangleShape gridCellShape; //�׸��� ���
	sf::Color gridLineColor; //�׸��� ������
	std::vector<std::vector<int>> mapArray; // �� �迭


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

