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
		if (!dest || !Utils::CheckCollision(hitBox.rect, p->GetHitBox().rect))
		{
			continue;
		}

		if ()
		{
			p->SetPosition(dest->GetPosition());
		}
		break;
	}

	Gimmick::Update(dt);
}
