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
	// 타일 그리기
	for (auto& r : solids)
	{
		window.draw(r);
	}

	// 히트박스 그리기
	if (Variables::isDrawHitBox) 
	{
		for (auto& r : solids) 
		{
			if (r.getOutlineThickness() <= 0)
			{
				continue;
				}
			window.draw(r);
		}
	}
}

bool TileMap::load(const Level& lvl, int solidStart)
{
	width = lvl.gridWidth; //가로 타일 수
	height = lvl.gridHeight; //세로 타일 수
	tileSize = lvl.tileSize; //한 타일 픽셀
	firstSolid = solidStart; //충돌 ID 기준
	tiles = lvl.tiles; //전체 복사

	solids.clear();
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int id = getTile(x, y);
			if (id < firstSolid || id == 0) //충돌 아닐 땐 타일 안 그림
			{
				continue;
			}

			sf::RectangleShape rect;
			rect.setSize({ (float)tileSize, (float)tileSize }); //사이즈 지정
			rect.setPosition((float)(x * tileSize), (float)(y * tileSize)); //위치 지정
			rect.setFillColor(sf::Color(255, 133, 77)); //타일 색 지정 -> 변경 가능
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

//벽 또는 바닥 확인
bool TileMap::isSolid(int tx, int ty) const
{
	int id = getTile(tx, ty);

	return id >= firstSolid && id != 0; //타일 ID가 1(충돌)보다 크거나 0이 아닐 경우 true
}

int TileMap::getTile(int tx, int ty) const
{
	if (tx < 0 || tx >= width || ty < 0 || ty >= height)
	{
		return 0;
	}

	//width : 가로 당 칸 수 / (tx, ty) : 타일 좌표
	return tiles[ty * width + tx]; //1D 행렬 좌표
}

void TileMap::setTile(int tx, int ty, int id)
{
	if (tx < 0 || tx >= width || ty <= 0 || ty >= height)
	{
		return;
	}

	tiles[ty * width + tx] = id;
	//load({ width, height, tileSize, tiles }, firstSolid); //전체 타일맵 재빌드
}
