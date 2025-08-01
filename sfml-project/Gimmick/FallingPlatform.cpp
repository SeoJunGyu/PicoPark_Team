#include "stdafx.h"
#include "FallingPlatform.h"
#include "Player.h"

FallingPlatform::FallingPlatform(nlohmann::json j)
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

void FallingPlatform::Reset()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;

	body.setTexture(TEXTURE_MGR.Get("graphics/Item/FallingPlatform.png"));

	player = nullptr;

	fallDelay = properties.value("fallDelay", 0.f);
	respawnDelay = properties.value("respawnDelay", 0.f);
	fallTimer = fallDelay;
	respawnTimer = respawnDelay;

	collidable = true;

	SetOrigin(Origins::MC);
	SetPosition(GetPosition());
	SetScale(GetScale());
	SetRotation(GetRotation());
	SetActive(true);

	hitBox.UpdateTransform(body, body.getLocalBounds());

	Gimmick::Reset();
}

void FallingPlatform::Update(float dt)
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

			// 옆면 충돌
			if (std::abs(info.normal.x) > 0.5f)
			{
				float separationX = info.normal.x * info.depth;
				p->SetPosition({ p->GetPosition().x + separationX, p->GetPosition().y });
				p->velocity.x = 0.f;
				continue;
			}

			// 아랫면 충돌
			if (info.normal.y > 0.5f && p->velocity.y < 0.f)
			{
				float separationY = info.normal.y * info.depth;
				p->SetPosition({ p->GetPosition().x, p->GetPosition().y + separationY });

				p->SetPosition({ p->GetPosition().x, p->GetPosition().y + separationY });
				p->velocity.y = 0.f;
				continue;
			}

			// 윗면 충돌
			if (info.normal.y < -0.5f && p->velocity.y > 0.f)
			{
				bool prevGround = p->isGrounded;
				float separationY = info.normal.y * info.depth;
				p->SetPosition({ p->GetPosition().x, p->GetPosition().y + separationY });

				// 착지 처리
				p->velocity.y = 0.f;
				p->isGrounded = true;
							
				if (!prevGround)
					p->PlayLandAnim();

				playerOnTop = true;
				continue;
			}
		}

		if (playerOnTop)
		{
			fallTimer -= dt;
			if (fallTimer <= 0.f)
			{
				collidable = false;
				respawnTimer = respawnDelay;
				body.setColor(sf::Color(255, 255, 255, 0));
			}
		}

		hitBox.UpdateTransform(body, body.getLocalBounds());
	}
	else
	{
		respawnTimer -= dt;
		if (respawnTimer <= 0.f)
		{
			collidable = true;
			fallTimer = fallDelay;
			body.setColor(sf::Color(255, 255, 255, 255));
		}
	}
	
	Gimmick::Update(dt);
}
