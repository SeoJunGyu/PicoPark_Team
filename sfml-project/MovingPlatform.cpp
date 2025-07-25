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
	prvSpeed = speed;

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
	prevPos = startPos;
	deltaPos = { 0.f, 0.f };

	SetOrigin(Origins::MC);
	SetPosition(startPos);
	SetScale(GetScale());
	SetRotation(GetRotation());

	hitBox.UpdateTransform(body, body.getLocalBounds());
}

void MovingPlatform::Update(float dt)
{
	if (blocked)
	{
		if (IsBlocked())
		{
			return;
		}
		blocked = false;
	}

	bool signal = Button::IsActive(channel);

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
			deltaPos = { 0.f, 0.f };
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
	sf::Vector2f stepVec = unitDir * (speed * dt * dir);
	prevPos = GetPosition();
	sf::Vector2f next = prevPos + stepVec;

	float distance = (next.x - startPos.x) * unitDir.x + (next.y - startPos.y) * unitDir.y;

	if (distance >= pathLength)
	{
		next = endPos;
	}
	else if (distance <= 0.f)
	{
		next = startPos;
	}

	sf::FloatRect oldBox = hitBox.rect.getGlobalBounds();
	sf::FloatRect testBox = oldBox;
	testBox.left += (next.x - prevPos.x);
	testBox.top += (next.y - prevPos.y);

	for (auto* p : Variables::players)
	{
		sf::FloatRect pBox = p->GetHitBox().rect.getGlobalBounds();
		if (!testBox.intersects(pBox))
			continue;

		// 분리 축과 깊이 계산
		CollisionInfo info = Utils::GetAABBCollision(pBox, testBox);
		// info.normal.y > 0 이면 “플레이어 머리가 아래에서 플랫폼을 밀어올린” 상황
		if (info.depth > 0.f && info.normal.y > 0.f)
		{
			// 머리 박힘 → 멈춤
			blocked = true;
			deltaPos = { 0.f, 0.f };
			return;    // 실제 이동( SetPosition ) 없이 탈출
		}
	}

	SetPosition(next);
	hitBox.UpdateTransform(body, body.getLocalBounds());

	deltaPos = next - prevPos;
	blocked = IsBlocked();
}

bool MovingPlatform::IsBlocked()
{
	for (auto* p : Variables::players)
	{
		if (!Utils::CheckCollision(hitBox.rect, p->GetHitBox().rect))
		{
			continue;
		}

		sf::FloatRect pRect = hitBox.rect.getGlobalBounds();
		sf::FloatRect playerRect = p->GetHitBox().rect.getGlobalBounds();

		CollisionInfo info = Utils::GetAABBCollision(playerRect, pRect);

		bool headHit = (info.depth > 0.f && info.normal.y > 0.5f) || (p->velocity.y < 0.f) || (dir > 0 && unitDir.y > 0 && p->velocity.y <= 0.f);

		if (headHit)
		{
			return true;
		}
	}

	return false;
}

