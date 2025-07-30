#include "stdafx.h"
#include "Door.h"
#include "Player.h"
#include "SceneGame.h"

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
	Player* player = nullptr;
	for (Player* p : Variables::players)
	{
		if (Utils::CheckCollision(hitBox.rect, p->GetHitBox().rect))
		{
			player = p;
			break;
		}
	}

	bool collide = (player != nullptr);
	bool playerHasKey = (player && player->hasKey);
	bool canOpen = locked == false || playerHasKey;

	if (!waColliding && collide)
	{
		if (canOpen && !opened)
		{
			opened = true;
			locked = false;
			body.setTexture(TEXTURE_MGR.Get("graphics/Item/doorOpen.png"));
			player->hasKey = false;

			Variables::KeyObtained = false;
		}
		else if (opened)
		{
			//SCENE_MGR.ChangeScene(SceneIds::Select);
			auto* gScene = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
			if (gScene) gScene->StartStageClear();  
			return;
		}
	}
	else if (!canOpen)
	{
		body.setTexture(TEXTURE_MGR.Get("graphics/Item/door.png"));
	}

	waColliding = collide;

	Gimmick::Update(dt);
}
