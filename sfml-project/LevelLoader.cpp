#include "stdafx.h"

bool loadLevel_(const std::string& path, Level& out)
{
	std::ifstream f(path);
	if (!f) { std::cerr << "file open fail\n"; return false; }

	nlohmann::json j;
	try { f >> j; }
	catch (std::exception& e) {
		std::cerr << "parse error: " << e.what() << '\n';
		return false;
	}

	if (j["version"] != 1) {
		std::cerr << "unsupported version\n";
		return false;
	}

	out.gridWidth = j["gridWidth"];
	out.gridHeight = j["gridHeight"];
	out.tileSize = j["tileSize"];
	out.tiles = j["layers"][0]["data"].get<std::vector<int>>();
	out.entities = j["entities"];
	return true;
}
