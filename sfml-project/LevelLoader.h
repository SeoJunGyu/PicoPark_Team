#pragma once
#include "stdafx.h"

struct Level {
	int gridWidth = 0;
	int gridHeight = 0;
	int tileSize = 0;
	std::vector<int> tiles;
	nlohmann::json entities;
};

bool loadLevel_(const std::string& path, Level& out);
