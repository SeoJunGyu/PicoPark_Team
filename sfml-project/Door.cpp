#include "stdafx.h"
#include "Door.h"
#include "Player.h"

Door::Door(nlohmann::json j)
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

void Door::Reset()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;

	body.setTexture(TEXTURE_MGR.Get("graphics/Item/door.png"));
	locked = properties.value("locked", true);
	opened = false;

	/*
	SetOrigin(Origins::MC);
	SetPosition(GetPosition());
	SetScale(GetScale());
	SetRotation(GetRotation());

	hitBox.UpdateTransform(body, body.getLocalBounds());
	*/
	

	Gimmick::Reset();
}

void Door::Update(float dt)
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

	Gimmick::Update(dt);
}
