#include "stdafx.h"
#include "MovingPlatform.h"

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
	// ① 다음 위치 계산
	//    dir=+1 이면 unitDir, dir=-1 이면 -unitDir
	sf::Vector2f stepVec = unitDir * (speed * dt * dir);
	sf::Vector2f pos = GetPosition();
	sf::Vector2f next = pos + stepVec;

	// ② 투영(projection)으로 오버슈트 체크
	//    l = (next - startPos)·unitDir
	float l = (next.x - startPos.x) * unitDir.x
		+ (next.y - startPos.y) * unitDir.y;

	if (l >= pathLength) {
		// end를 넘어섰으면 clamping
		next = endPos;
	}
	else if (l <= 0.f) {
		// start보다 앞섰으면 clamping
		next = startPos;
	}

	// ③ 실제 위치 적용 및 hitBox 갱신
	SetPosition(next);
	hitBox.UpdateTransform(body, body.getLocalBounds());
}

