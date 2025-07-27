#include "stdafx.h"
#include "Gimmick.h"
#include "Player.h"

Gimmick* Gimmick::CreateFromJson(const nlohmann::json& j)
{
	auto type = StrToType(j.at("type").get<std::string>());
	switch (type)
	{
	case GimmickType::Key:
		return new Key(j);
	case GimmickType::Door:
		return new Door(j);
	case GimmickType::Button:
		return new Button(j);
	case GimmickType::MovingPlatform:
		return new MovingPlatform(j);
	case GimmickType::BouncePad:
		return new BouncePad(j);
	case GimmickType::FallingPlatform:
		return new FallingPlatform(j);
	case GimmickType::PushBlock:
		return new PushBlock(j);
	}

	return nullptr;
}

Gimmick::Gimmick(int id, GimmickType t, const sf::Vector2f& pos, const sf::Vector2f& scl, float rot, nlohmann::json props)
	: id(id), type(t), properties(std::move(props))
{
	SetPosition(pos);
	SetScale(scl);
	SetRotation(rot);
}


void Gimmick::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	body.setPosition(pos);
	hitBox.UpdateTransform(body, body.getLocalBounds());
}

void Gimmick::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	body.setRotation(rot);
	hitBox.UpdateTransform(body, body.getLocalBounds());
}

void Gimmick::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	body.setScale(s);
	hitBox.UpdateTransform(body, body.getLocalBounds());
}

void Gimmick::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	body.setOrigin(o);
}

void Gimmick::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	if (preset != Origins::Custom)
	{
		Utils::SetOrigin(body, preset);
	}
}

void Gimmick::Init()
{
	
}

void Gimmick::Release()
{
}

void Gimmick::Reset()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;

	/*
	switch (type)
	{
		// 움직임 없음
	case GimmickType::Key:
		//body.setTexture(TEXTURE_MGR.Get("graphics/Item/key.png"));
		break;
	case GimmickType::Door:
		
		break;
	case GimmickType::Button:
		break;
	case GimmickType::PlayerSpawn:
		break;

		//움직임 있음
	case GimmickType::MovingPlatform:
		
		break;
	case GimmickType::WeightBlock:
		body.setTexture(TEXTURE_MGR.Get("graphics/Item/WeightBlock.png"));
		break;

	default:
		body.setColor(sf::Color::Red);
		break;
	}
	*/
	

	//SetOrigin(Origins::MC);
	//float ox = 16.f * 0.5f;
	//float oy = 16.f * 0.5f;

	//float realH = body.getTexture()->getSize().y * body.getScale().y;
	//float oy_fix = std::abs((realH - 16.f) * 0.5f);

	//sf::Vector2f pos(GetPosition().x + ox, GetPosition().y); 
	sf::Vector2f pos = { GetPosition().x, GetPosition().y};
	SetPosition(pos);
	Utils::alignSpriteBottomToTile(body, pos);
	SetScale(GetScale());
	SetRotation(GetRotation());

	hitBox.UpdateTransform(body, body.getLocalBounds());
}

void Gimmick::Update(float dt)
{
	//활성화 안하면 아무것도 안함
	if (!GetActive())
	{
		return;
	}

	/*
	switch (type)
	{
		//움직임 없음
	case GimmickType::Key:
		break;
	case GimmickType::Door:
		{
			
		}
		
		break;
	case GimmickType::Button:
		break;
	case GimmickType::BouncePad:
		break;
	case GimmickType::WeightPad:
		break;
	case GimmickType::KillLine:
		break;

		//움직임 있음
	case GimmickType::MovingPlatform:
		break;
	case GimmickType::FallPlatform:
		break;
	case GimmickType::WeightBlock:
		break;
	case GimmickType::ColorPushBlock:
		break;
	case GimmickType::Rope:
		break;
	case GimmickType::Count:
		break;

	default:
		break;
	}
	*/
	
}

void Gimmick::Draw(sf::RenderWindow& window)
{
	if (!GetActive())
	{
		return;
	}
	window.draw(body);

	if (Variables::isDrawHitBox)
	{
		hitBox.Draw(window);
	}
}