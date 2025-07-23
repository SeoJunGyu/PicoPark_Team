#include "stdafx.h"
#include "Gimmick.h"
#include "Player.h"

//Json���� ���� Ÿ���� GimmickType���� ��ȯ
static GimmickType StrToType(const std::string& str)
{
	if (str == "Key")
	{
		return GimmickType::Key;
	}
	if (str == "Door")
	{
		return GimmickType::Door;
	}
	if (str == "PlayerSpawn")
	{
		return GimmickType::PlayerSpawn;
	}

	return GimmickType::Key;
}

Gimmick* Gimmick::CreateFromJson(const nlohmann::json& j)
{
	int id = j.value("id", 0); //Ű ������ �״��, ������ 0
	auto type = StrToType(j.at("type").get<std::string>()); //Ÿ�� ���ڿ��� GimmickType ���������� ����

	//��ġ ��ǥ �ȼ� ����
	float x = j.at("x").get<float>();
	float y = j.at("y").get<float>();

	float rot = j.value("rotation", 0.f); //Ű ������ �״��, ������ 0
	sf::Vector2f scl{ 1.f, 1.f }; //���� scale ����
	//sf::Vector2f sizePx{ 0.f, 0.f }; //�ȼ� ���밪 (����)

	//������ �ۼ� �Ǿ������� ��ȯ
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

	nlohmann::json props = j.value("properties", nlohmann::json::object()); //Ư�� ��� ���� �Ӽ� ����

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
	switch (type)
	{
		// ������ ����
	case GimmickType::Key:
		body.setTexture(TEXTURE_MGR.Get("graphics/Item/key.png"));
		break;
	case GimmickType::Door:
		body.setTexture(TEXTURE_MGR.Get("graphics/Item/door.png"));
		break;
	case GimmickType::Buton:
		body.setTexture(TEXTURE_MGR.Get("graphics/Item/Button.png"));
		break;

		//������ ����
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
	//Ȱ��ȭ ���ϸ� �ƹ��͵� ����
	if (!GetActive())
	{
		return;
	}

	switch (type)
	{
		//������ ����
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
			bool locked = properties.value("locked", true); //�̷��� properties ���
			bool canOpen = locked || Variables::KeyObtained;

			if (canOpen)
			{
				for (Player* p : Variables::players)
				{
					if (Utils::CheckCollision(hitBox.rect, p->GetHitBox().rect))
					{
						body.setTexture(TEXTURE_MGR.Get("graphics/Item/doorOpen.png"));
						//SetActive(false);
						break;
					}
				}
			}
			else
			{
				body.setTexture(TEXTURE_MGR.Get("graphics/Item/door.png"));
			}
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

		//������ ����
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