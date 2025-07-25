#pragma once

class Player;
class Gimmick;
class MovingPlatform;

class Variables
{
public:
	static bool isDrawHitBox;

	static bool KeyObtained; //열쇠 습득 여부
	static std::vector<Player*> players;
	static std::vector<Gimmick*> gimmicks;
	static std::vector<MovingPlatform*> platforms;

	static void ResetStage();

	static sf::Vector2f CalScl(nlohmann::json j);
};

