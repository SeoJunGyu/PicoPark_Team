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
	channel = properties.value("channel", 0);
	playerKill = properties.value("playerKill", false);
	transparency = properties.value("transparency", false);

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
	//신호 받을때 처리
	UpdateChannel();

	if (transparency) //투명처리
	{
		body.setColor(sf::Color(255, 255, 255, 0));
	}

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
				p->velocity.y = 0.f;
				continue;
			}

			// 윗면 충돌
			if (info.normal.y < -0.5f && p->velocity.y > 0.f)
			{
				if (playerKill)
				{
					for (Player* player : Variables::players)
					{
						if (player->isDead)
						{
							continue;
						}

						player->OnDie();
					}
					collidable = false;
				}

				float separationY = info.normal.y * info.depth;
				p->SetPosition({ p->GetPosition().x, p->GetPosition().y + separationY });

				// 착지 처리
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

	Gimmick::Update(dt);
}

void RoundPlatform::Draw(sf::RenderWindow& window)
{
	Gimmick::Draw(window);

	/*
	thickLine.setPosition(GetPosition());
	thickLine.setFillColor(sf::Color(0, 0, 0, 0));
	thickLine.setOutlineColor(sf::Color::Red);
	thickLine.setOutlineThickness(2.f);

	window.draw(thickLine);
	*/
	
}

void RoundPlatform::UpdateChannel()
{
	if (channel > 0)
	{
		active = Button::IsActive(channel);
	}
	if (channel > 0 && !active)
	{
		body.setColor(sf::Color(255, 255, 255, 0));
		collidable = false;
	}
	else if (channel > 0 && active)
	{
		body.setColor(sf::Color(255, 255, 255, 255));
		collidable = true;
	}
}
