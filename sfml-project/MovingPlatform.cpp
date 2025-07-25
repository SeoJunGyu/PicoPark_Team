#include "stdafx.h"
#include "MovingPlatform.h"
#include "Player.h"

MovingPlatform::MovingPlatform(nlohmann::json j)
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

void MovingPlatform::Reset()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;

	body.setTexture(TEXTURE_MGR.Get("graphics/Item/Pad.png"));

	channel = properties.value("channel", -1);
	startPos = { properties["path"][0][0].get<float>(), properties["path"][0][1].get<float>() };
	endPos = { properties["path"][1][0].get<float>(), properties["path"][1][1].get<float>() };
	speed = properties.value("speed", 0.f);

	sf::Vector2f delta = endPos - startPos;
	pathLength = std::sqrt(delta.x * delta.x + delta.y * delta.y);
	if (pathLength > 0.f)
	{
		unitDir = { delta.x / pathLength, delta.y / pathLength };
	}
	else
	{
		unitDir = { 0.f, 0.f };
	}

	dir = 0;

	SetOrigin(Origins::MC);
	SetPosition(startPos);
	SetScale(GetScale());
	SetRotation(GetRotation());

	hitBox.UpdateTransform(body, body.getLocalBounds());
}

void MovingPlatform::Update(float dt)
{
	bool signal = (channel < 0 ? true : Variables::signals[channel]);

	auto pos = GetPosition();
	bool atStart = std::abs((pos - startPos).x) + std::abs((pos - startPos).y) < 0.01f;
	bool atEnd = std::abs((pos - endPos).x) + std::abs((pos - endPos).y) < 0.01f;

	if (signal)
	{
		if (atStart)
		{
			dir = 1;
		}
		else if (atEnd)
		{
			dir = -1;
		}
	}
	else //신호 off
	{
		if (atStart)
		{
			dir = 0;
		}
		else
		{
			dir = -1;
		}
	}

	if (dir != 0)
	{
		moveOneStep(dt);
	}
}

void MovingPlatform::moveOneStep(float dt)
{
	sf::FloatRect prevBox = hitBox.rect.getGlobalBounds(); //이전 히트박스 저장

	sf::Vector2f movement = unitDir * (speed * dt * dir);
	SetPosition(GetPosition() + movement);
	hitBox.UpdateTransform(body, body.getLocalBounds());

	//충돌 처리
	for (auto* p : Variables::players)
	{
		if (!Utils::CheckCollision(hitBox.rect, p->GetHitBox().rect))
		{
			continue;
		}

		//경계 검사
		float overlapX = std::min(hitBox.GetRight(), p->GetHitBox().GetLeft() + p->GetHitBox().GetWidth()) - std::max(hitBox.GetLeft(), p->GetHitBox().GetLeft());
		float overlapY = std::min(hitBox.GetBottom(), p->GetHitBox().GetTop() + p->GetHitBox().GetHeight()) - std::max(hitBox.GetTop(), p->GetHitBox().GetTop());

		if (overlapX < overlapY)
		{
			//가로 충돌
			float push = (movement.x > 0) ? -overlapX : overlapX;
			p->SetPosition(p->GetPosition() + sf::Vector2f{ push, 0.f });
			p->velocity.x = 0.f;
		}
		else
		{
			//세로 충돌
			if (movement.y > 0) //하강
			{
				p->SetPosition(p->GetPosition() + sf::Vector2f{ 0.f, -overlapY });
				p->velocity.y = std::min(p->velocity.y, 0.f);
			}
			else //상승
			{
				p->SetPosition(p->GetPosition() + sf::Vector2f{ 0.f, overlapY });
				p->velocity.y = 0.f;
				p->isGrounded = true;
			}
		}
	}
	
}

