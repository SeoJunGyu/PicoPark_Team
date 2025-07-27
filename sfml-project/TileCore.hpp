#pragma once
#include "stdafx.h"

struct TileSet {
    std::vector<sf::Texture> textures;
    int tileSize = 16;
};

struct LevelGrid {
    int width = 0;
    int height = 0;
    int tileSize = 16;
    std::vector<int> tiles;
    std::vector<int> entities;
    std::vector<std::string> entitiesType;
    std::vector<nlohmann::json> entitiesProps;

    LevelGrid() = default;                     
    LevelGrid(int w, int h, int ts)
        : width(w), height(h), tileSize(ts),
        tiles(w* h, 0), entities(w* h, 0), entitiesType(w* h, ""), entitiesProps(w* h, nlohmann::json::object()){
    }
};