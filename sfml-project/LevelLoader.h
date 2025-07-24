#pragma once
#include "stdafx.h"

struct Tile
{
	int id = 0;
};

struct Layer
{
	std::string name;
	enum class Kind { TILE, OBJECT } type = Kind::TILE;
	bool visible = true;
	float parallax = 1.0f;
	std::vector<int> data;     // TILE: id 배열  / OBJECT: idx 리스트
};

struct Entity
{
	int id = 0;
	GimmickType type = GimmickType::Count;
	int x = 0, y = 0;
	int w = 16, h = 16;
	sf::Vector2f scale = { 1.f, 1.f };
	nlohmann::json properties;
};

struct Level {
	int gridWidth = 0;
	int gridHeight = 0;
	int tileSize = 16;
	std::string author;
	std::string description;
	std::vector<Layer> layers;
	std::vector<Entity> entities;
	std::vector<int> tiles;
};

bool loadLevel_(const std::string& path, Level& out);
