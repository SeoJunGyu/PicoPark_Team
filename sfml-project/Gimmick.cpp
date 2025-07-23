#include "stdafx.h"
#include "Gimmick.h"
#include "Player.h"
#include "GimmickTypeConv.hpp"

Gimmick* Gimmick::CreateFromJson(const nlohmann::json& j)
{
	int id = j.value("id", 0); //키 있으면 그대로, 없으면 0
	auto type = StrToType(j.at("type").get<std::string>()); //타입 문자열을 GimmickType 열거형으로 저장

	if (type == GimmickType::PlayerSpawn)
		return nullptr;

	//위치 좌표 픽셀 단위
	float x = j.at("x").get<float>();
	float y = j.at("y").get<float>();

	float rot = j.value("rotation", 0.f); //키 있으면 그대로, 없으면 0
	sf::Vector2f scl{ 1.f, 1.f }; //기존 scale 유지
	//sf::Vector2f sizePx{ 0.f, 0.f }; //픽셀 절대값 (선택)

	//스케일 작성 되어있으면 변환
	if (j.contains("scale"))
	{
		if (j["scale"].is_array())
		{
			scl.x = j["scale"][0].get<float>();
			scl.y = j["scale"][1].get<float>();
		}
		else
		{
			float uni = j["scale"].get<float>();
			scl = { uni, uni };
		}
	}

	nlohmann::json props = j.value("properties", nlohmann::json::object()); //특정 기믹 전용 속성 묶음

	return new Gimmick(id, type, { x, y }, scl, rot, props);
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

	switch (type)
	{
		// 움직임 없음
	case GimmickType::Key:
		body.setTexture(TEXTURE_MGR.Get("graphics/Item/key.png"));
		break;
	case GimmickType::Door:
		body.setTexture(TEXTURE_MGR.Get("graphics/Item/door.png"));
		locked = properties.value("locked", true);
		opened = false;
		break;
	case GimmickType::Buton:
		body.setTexture(TEXTURE_MGR.Get("graphics/Item/Button.png"));
		break;
	case GimmickType::PlayerSpawn:
		break;

		//움직임 있음
	case GimmickType::MovingPlatform:
		body.setTexture(TEXTURE_MGR.Get("graphics/Item/Pad.png"));
		break;
	case GimmickType::WeightBlock:
		body.setTexture(TEXTURE_MGR.Get("graphics/Item/WeightBlock.png"));
		break;

	default:
		body.setColor(sf::Color::Red);
		break;
	}

	SetOrigin(Origins::MC);
	SetPosition(GetPosition());
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

	switch (type)
	{
		//움직임 없음
	case GimmickType::Key:
		for (Player* p : Variables::players)
		{
			if (Utils::CheckCollision(hitBox.rect, p->GetHitBox().rect))
			{
				Variables::KeyObtained = true;
				body.setPosition({ p->GetPosition().x, p->GetPosition().y - GetLocalBounds().height });
				break;
			}
		}
		break;
	case GimmickType::Door:
		{
			bool canOpen = locked == false || Variables::KeyObtained;
			bool collide = std::any_of(
				Variables::players.begin(), Variables::players.end(),
				[&](Player* p) { return Utils::CheckCollision(hitBox.rect, p->GetHitBox().rect); }
			);

			if (!waColliding && collide)
			{
				if (canOpen && !opened)
				{
					opened = true;
					locked = false;
					body.setTexture(TEXTURE_MGR.Get("graphics/Item/doorOpen.png"));
				}
				else if (opened)
				{
					SCENE_MGR.ChangeScene(SceneIds::Game);
				}
			}
			else if (!canOpen)
			{
				body.setTexture(TEXTURE_MGR.Get("graphics/Item/door.png"));
			}

			waColliding = collide;
		}
		
		break;
	case GimmickType::Buton:
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