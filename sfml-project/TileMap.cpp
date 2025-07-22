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
}