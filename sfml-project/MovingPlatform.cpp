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

	channel = properties.value("channel", -1);
	requireCount = properties.value("requireCount", 0);

	body.setTexture(TEXTURE_MGR.Get("graphics/Item/Pad.png"));

	countText.setFont(FONT_MGR.Get("fonts/DS-DIGIT.ttf"));

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

	/*
	countText.setString(std::to_string(requireCount));
	countText.setCharacterSize(8);
	countText.setFillColor(sf::Color::Black);
	countText.setPosition({ position.x, position.y - 50.f });
	Utils::SetOrigin(countText, Origins::MC);
	*/
	

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

	//bool signal = Button::IsActive(channel);
	bool active;
	if (requireCount > 0)
	{
		hitBox.UpdateTransform(body, body.getLocalBounds());  // (Optional) hitBox 최신화
		int count = 0;
		for (auto* p : Variables::players)
		{
			// 최종 지지 객체가 바로 이 플랫폼인지 확인
			Player* support = p;
			// 플레이어 위에 플레이어가 올라탄 체인 climb
			while (support->standing.type == StandType::Player)
				support = support->standing.asPlayer();

			// 지지 타입이 Platform 이고, ptr 이 this 인 경우만 카운트
			if (support->standing.type == StandType::Platform &&
				support->standing.ptr == this)
			{
				++count;
			}
		}
		active = (count >= requireCount);

		/*
		countText.setString(std::to_string(onPad.size()) + "/" + std::to_string(requireCount));
		sf::FloatRect bb = body.getGlobalBounds();
		countText.setPosition(bb.left + bb.width * 0.5f - countText.getLocalBounds().width * 0.5f, bb.top - countText.getCharacterSize() * 1.2f);
		*/

	}
	else
	{
		//기존 채널 모드
		active = Button::IsActive(channel);
	}

	auto pos = GetPosition();
	bool atStart = std::abs((pos - startPos).x) + std::abs((pos - startPos).y) < 0.01f;
	bool atEnd = std::abs((pos - endPos).x) + std::abs((pos - endPos).y) < 0.01f;

	if (active)
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

void MovingPlatform::Draw(sf::RenderWindow& window)
{
	Gimmick::Draw(window);

	
	//window.draw(countText);
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

