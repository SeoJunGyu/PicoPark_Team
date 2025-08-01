#include "stdafx.h"
#include "Portal.h"

Portal::Portal(nlohmann::json j)
	: Gimmick(
		j.value("id", 0),
		StrToType(j.at("type").get<std::string>()),
		{ j.at("x").get<float>() , j.at("y").get<float>() },
		Variables::CalScl(j),
		j.value("rotation", 0.f),
		j.value("properties", nlohmann::json::object())
	)
{
}

void Portal::Reset()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;

	body.setTexture(TEXTURE_MGR.Get("graphics/Item/portal.png"));

	channel = properties.value("channel", 0);
	destChannel = properties.value("destChannel", -1);
	cooldownSec = properties.value("cooldownSec", 1.0f);
	loop = properties.value("loop", false);

	entered.clear();

	SetOrigin(Origins::MC);
	SetPosition(GetPosition());
	SetScale(GetScale());
	SetRotation(GetRotation());

	hitBox.UpdateTransform(body, body.getLocalBounds());

	Gimmick::Reset();
}

void Portal::Update(float dt)
{
	hitBox.UpdateTransform(body, body.getLocalBounds());

	for (Player* p : Variables::players)
	{
		if (!dest)
		{
			continue;
		}

		if (p->portalTimer > 0.f)
		{
			continue;
		}

		bool colliding = Utils::CheckCollision(hitBox.rect, p->GetHitBox().rect);

		if (loop)
		{
			if (colliding && p->portalTimer <= 0.f)
			{
				Teleport(p);
			}
			continue;
		}
		
		if (colliding)
		{
			if (std::find(entered.begin(), entered.end(), p) == entered.end())
			{
				Teleport(p);
				entered.push_back(p);
			}
		}
		else
		{
			auto it = std::find(entered.begin(), entered.end(), p);
			if (it != entered.end())
			{
				entered.erase(it);
			}
		}

	}

	Gimmick::Update(dt);
}

void Portal::Teleport(Player* p)
{
	//도착 포탈 중심으로 이동 & 약간 오프셋
	sf::Vector2f targetPos = dest->GetPosition();
	//targetPos.y -= p->GetHitBox().rect.getSize().y + (hitBox.GetTop() + hitBox.GetHeight()) * 0.5f; // 위쪽에 살짝 띄우기
	p->SetPosition(targetPos);

	//서로 쿨타임 부여
	p->portalTimer = cooldownSec;

	if (!dest->loop)
	{
		auto& list = dest->entered;
		if (std::find(list.begin(), list.end(), p) == list.end())
			list.push_back(p);
	}
}
