#include "stdafx.h"
#include "TileMap.h"

TileMap::TileMap(const std::string& name)
	: GameObject(name)
{

}

void TileMap::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	//gameObject.setPosition(pos);
}

void TileMap::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	//gameObject.setRotation(rot);
}

void TileMap::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	//gameObject.setScale(s);
}

void TileMap::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	//gameObject.setOrigin(o);
}

void TileMap::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	if (preset != Origins::Custom)
	{
		//Utils::SetOrigin(gameObject, preset);
	}
}

void TileMap::Init()
{
}

void TileMap::Release()
{
}

void TileMap::Reset()
{
	
}

void TileMap::Update(float dt)
{
}

void TileMap::Draw(sf::RenderWindow& window)
{
	for (auto& r : solids)
	{
		window.draw(r);
	}

	if (Variables::isDrawHitBox) 
	{
		for (auto& r : solids) 
		{
			if (r.getOutlineThickness() <= 0)
			{
				continue;
				}
			window.draw(r);              // �̹� outline ������ ����
		}
	}
}

bool TileMap::load(const Level& lvl, int solidStart)
{
	width = lvl.gridWidth;
	height = lvl.gridHeight;
	tileSize = lvl.tileSize;
	firstSolid = solidStart;
	tiles = lvl.tiles;

	solids.clear();
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int id = getTile(x, y);
			if (id < firstSolid || id == 0)
			{
				continue;
			}

			sf::RectangleShape rect;
			rect.setSize({ (float)tileSize, (float)tileSize });
			rect.setPosition((float)(x * tileSize), (float)(y * tileSize));
			rect.setFillColor(sf::Color(255, 133, 77));
			//rect.setOutlineThickness(0.5f);
			//rect.setOutlineColor(sf::Color::Green);
			solids.push_back(rect);

			if (id >= firstSolid) {
				solids.back().setOutlineThickness(0.1f);
				solids.back().setOutlineColor(sf::Color::Green);
			}
		}
	}
	
	return true;
}

bool TileMap::isSolid(int tx, int ty) const
{
	int id = getTile(tx, ty);

	return id >= firstSolid && id != 0;
}

int TileMap::getTile(int tx, int ty) const
{
	if (tx < 0 || tx >= width || ty >= height)
	{
		return 0;
	}

	return tiles[ty * width + tx];
}

void TileMap::setTile(int tx, int ty, int id)
{
	if (tx < 0 || tx >= width || ty <= 0 || ty >= height)
	{
		return;
	}

	tiles[ty * width + tx] = id;
	load({ width, height, tileSize, tiles }, firstSolid);
}
