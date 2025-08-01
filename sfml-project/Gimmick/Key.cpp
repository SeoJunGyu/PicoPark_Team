﻿#include "stdafx.h"
#include "Key.h"
#include "Player.h"

Key::Key(nlohmann::json j)
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

void Key::Reset()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;

	body.setTexture(TEXTURE_MGR.Get("graphics/Item/key.png"));

	player = nullptr;
	newPlayer = nullptr;
	firstkey = true;
	SetOrigin(Origins::MC);
	SetPosition(GetPosition());
	SetScale(GetScale());
	SetRotation(GetRotation());

	hitBox.UpdateTransform(body, body.getLocalBounds());

	Gimmick::Reset();
}

void Key::Update(float dt)
{
	for (Player* p : Variables::players)
	{
		if (p->hasKey)
		{
			continue;
		}

		if (Utils::CheckCollision(hitBox.rect, p->GetHitBox().rect))
		{
			if (firstkey)
			{
				SOUND_MGR.PlaySfx("audio/key.mp3", false);
				firstkey = false;
			}
			if (newPlayer && newPlayer->hasKey)
			{
				newPlayer->hasKey = false;
			}
			p->hasKey = true;
			player = p;
			newPlayer = p;

			Variables::KeyObtained = true;
			break;
		}
	}
	if (player != nullptr)
	{
		body.setPosition({ player->GetPosition().x, player->GetPosition().y - player->GetGlobalBounds().height - 6.f});
		hitBox.UpdateTransform(body, body.getLocalBounds());
	}

	if (player && !Variables::KeyObtained)
	{
		SetActive(false);
	}

	Gimmick::Update(dt);
}
