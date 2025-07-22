#include "stdafx.h"
#include "TileMap.h"

bool TileMap::load(const Level& lvl)
{
    return false;
}

void TileMap::draw(sf::RenderWindow& win)
{
}

bool TileMap::isSolid(int tx, int ty) const
{
    return false;
}

int TileMap::getTile(int tx, int ty) const
{
    return 0;
}

void TileMap::setTile(int tx, int ty, int id)
{
}
