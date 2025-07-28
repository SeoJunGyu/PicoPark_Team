#include "stdafx.h"
#include "RoundPlatform.h"
#include "Player.h"

RoundPlatform::RoundPlatform(nlohmann::json j)
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

void RoundPlatform::Reset()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;

	body.setTexture(TEXTURE_MGR.Get("graphics/Item/RoundPlatform.png"));

	player = nullptr;

	randFall = properties.value("RandFall", 100.f);

	collidable = true;
	isOne = false;

	SetOrigin(Origins::MC);
	SetPosition(GetPosition());
	SetScale(GetScale());
	SetRotation(GetRotation());
	SetActive(true);

	hitBox.UpdateTransform(body, body.getLocalBounds());

	Gimmick::Reset();
}

void RoundPlatform::Update(float dt)
{
	sf::FloatRect platBox = hitBox.rect.getGlobalBounds();

	if (collidable)
	{
		bool playerOnTop = false;

		for (Player* p : Variables::players)
		{
			sf::FloatRect pBox = p->GetHitBox().rect.getGlobalBounds();

			CollisionInfo info = Utils::GetAABBCollision(pBox, platBox);
			if (info.depth <= 0.f)
			{
				continue;
			}

			// ¿·¸é Ãæµ¹
			if (std::abs(info.normal.x) > 0.5f)
			{
				float separationX = info.normal.x * info.depth;
				p->SetPosition({ p->GetPosition().x + separationX, p->GetPosition().y });
				p->velocity.x = 0.f;
				continue;
			}

			// ¾Æ·§¸é Ãæµ¹
			if (info.normal.y > 0.5f && p->velocity.y < 0.f)
			{
				float separationY = info.normal.y * info.depth;
				p->SetPosition({ p->GetPosition().x, p->GetPosition().y + separationY });

				p->SetPosition({ p->GetPosition().x, p->GetPosition().y + separationY });
				p->velocity.y = 0.f;
				continue;
			}

			// À­¸é Ãæµ¹
			if (info.normal.y < -0.5f)
			{
				float separationY = info.normal.y * info.depth;
				p->SetPosition({ p->GetPosition().x, p->GetPosition().y + separationY });

				// ÂøÁö Ã³¸®
				p->velocity.y = 0.f;
				p->isGrounded = true;

				playerOnTop = true;
				continue;
			}
		}

		if (playerOnTop && !isOne)
		{
			if (randFall != 0.f && Utils::RandomRange(1, 100) < randFall) {
				collidable = false;
			}
			isOne = true;
		}

		hitBox.UpdateTransform(body, body.getLocalBounds());
	}
	else
	{
		
	}

	Gimmick::Update(dt);
}