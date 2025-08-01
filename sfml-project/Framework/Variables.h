#pragma once

class Player;
class Gimmick;
class MovingPlatform;
class PushBlock;
class Portal;
class TileMap;

class Variables
{
public:
	static bool isDrawHitBox;

	static bool KeyObtained; //열쇠 습득 여부
	static std::vector<Player*> players;
	static std::vector<Gimmick*> gimmicks;
	static std::vector<MovingPlatform*> platforms;
	static std::vector<PushBlock*> blocks;
	static std::vector<Portal*> portals;

	static TileMap* tilemap;

	static void ResetStage();

	static sf::Vector2f CalScl(nlohmann::json j);
};

