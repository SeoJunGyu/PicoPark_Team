#include "stdafx.h"
#include "Variables.h"

bool Variables::isDrawHitBox = false;

bool Variables::KeyObtained = false;
std::vector<Player*> Variables::players{};
std::vector<Gimmick*> Variables::gimmicks{};
std::vector<MovingPlatform*> Variables::platforms{};
std::vector<PushBlock*> Variables::blocks{};

TileMap* Variables::tilemap = nullptr;

void Variables::ResetStage()
{
	KeyObtained = false;

	for (auto* g : gimmicks) delete g;
	gimmicks.clear();
	for (auto* p : players) delete p;
	players.clear();
}

sf::Vector2f Variables::CalScl(nlohmann::json j)
{
	sf::Vector2f scl{ 1.f, 1.f }; //기존 scale 유지

	if (j.contains("scale"))
	{
		if (j["scale"].is_array())
		{
			scl.x = j["scale"][0].get<float>();
			scl.y = j["scale"][1].get<float>();

			return scl;
		}
		else
		{
			float uni = j["scale"].get<float>();
			scl = { uni, uni };

			return scl;
		}
	}

	return scl;
}
