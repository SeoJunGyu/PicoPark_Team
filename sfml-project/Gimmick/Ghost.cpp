#include "stdafx.h"
#include "Ghost.h"

Ghost::Ghost(nlohmann::json j)
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

void Ghost::Reset()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;

	body.setTexture(TEXTURE_MGR.Get("graphics/Item/ghost.png"));

	shyGhost = properties.value("shyGhost", false);

	SetOrigin(Origins::MC);
	SetPosition(GetPosition());
	SetScale(GetScale());
	SetRotation(GetRotation());

	hitBox.UpdateTransform(body, body.getLocalBounds());

	Gimmick::Reset();
}

void Ghost::Update(float dt)
{
	//뒤돈 플레이어 사냥 유령 속성
	if (shyGhost)
	{
		bool allBehind = true; //전부 뒤돈지 확인
		Player* nearest = nullptr; //가까운 플레이어 저장
		float minDist = std::numeric_limits<float>::infinity(); //가장 작은 거리를 저장하기 전 가장 큰 값으로 초기화

		for (Player* p : Variables::players)
		{
			int dir = p->ghostDirection;
			float dx = GetPosition().x - p->GetPosition().x;
			if (dx * dir >= 0.f)
			{
				allBehind = false;
				break;
			}

			//뒤돌린 플레이어 거리 계산
			float dist = std::abs(dx);
			if (dist < minDist)
			{
				minDist = dist;
				nearest = p;
			}
		}

		if (allBehind && nearest)
		{
			if (state != GhostState::Chasing)
			{
				state = GhostState::Chasing;
				target = nearest;   //target 설정
				body.setTexture(TEXTURE_MGR.Get("graphics/Item/ghost3.png"));
			}
		}
		else
		{
			if (state != GhostState::Scanning)
			{
				state = GhostState::Scanning;
				target = nullptr;
				body.setTexture(TEXTURE_MGR.Get("graphics/Item/ghost.png"));
			}
		}
	}

	hitBox.UpdateTransform(body, body.getGlobalBounds());

	if (!shyGhost)
	{
		//플레이어 속도 계산
		UpdateDetectionMeter(dt);

		//감지 게이지 증감
		DetectionMeterUpDown(dt);

		//구간별 상태 전환
		ChangeState();
	}
	

	//상태별 행동
	UpdateState(dt);

	//스케일 조정
	UpdateScale();

	//플레이어 충돌 및 사망처리
	CollisionPlayer();

	hitBox.UpdateTransform(body, body.getGlobalBounds());

	Gimmick::Update(dt);
}

void Ghost::UpdateDetectionMeter(float dt)
{
	highestSpeed = 0.f;
	fastest = nullptr;
	for (Player* p : Variables::players)
	{
		sf::Vector2f pVelocity = p->velocity;
		float speed = Utils::Magnitude(pVelocity);
		if (speed > highestSpeed)
		{
			highestSpeed = speed;
			fastest = p;
		}
	}
}

void Ghost::DetectionMeterUpDown(float dt)
{
	float norm = Utils::Clamp(highestSpeed / maxPlayerSpeed, 0.f, 1.f);
	detectionMeter += norm * dt;
	detectionMeter -= meterDecayRate * dt;
	detectionMeter = Utils::Clamp(detectionMeter, 0.f, 1.f);
}

void Ghost::ChangeState()
{
	if (detectionMeter >= detectThreshold && state != GhostState::Chasing)
	{
		state = GhostState::Chasing;
		body.setTexture(TEXTURE_MGR.Get("graphics/Item/ghost3.png"));
		target = fastest;
	}
	else if (detectionMeter >= detectSecondPhase && state == GhostState::Scanning)
	{
		state = GhostState::Alert;
		body.setTexture(TEXTURE_MGR.Get("graphics/Item/ghost2.png"));
	}
	else if (detectionMeter < detectSecondPhase && state != GhostState::Scanning)
	{
		state = GhostState::Scanning;
		body.setTexture(TEXTURE_MGR.Get("graphics/Item/ghost.png"));
		target = nullptr;
	}
}

void Ghost::UpdateState(float dt)
{
	if (state == GhostState::Chasing && target && !target->isDead)
	{
		sf::Vector2f pos = GetPosition();
		sf::Vector2f playerPos = target->GetPosition();
		sf::Vector2f delta = playerPos - pos;
		float ax = std::abs(delta.x);
		float ay = std::abs(delta.y);

		sf::Vector2f move{ 0.f, 0.f };
		if (ax > ay)
		{
			move.x = maxPlayerSpeed * sign(delta.x);
		}
		else
		{
			move.y = maxPlayerSpeed * sign(delta.y);
		}

		sf::Vector2f newPos = pos + move * dt;
		SetPosition(newPos);
		hitBox.UpdateTransform(body, body.getGlobalBounds());
	}
}

void Ghost::UpdateScale()
{
	if (target)
	{
		float dx = target->GetPosition().x - GetPosition().x; //타겟 방향
		float  sx = std::abs(GetScale().x);
		float  sy = GetScale().y;

		// 플레이어가 우측
		if (dx > 0.f && GetScale().x < 0.f)
		{
			SetScale({ sx, sy });
		}
		// 플레이어가 좌측
		else if (dx < 0.f && GetScale().x > 0.f)
		{
			SetScale({ -sx, sy });
		}
	}
}

void Ghost::CollisionPlayer()
{
	for (Player* p : Variables::players)
	{
		if (Utils::CheckCollision(hitBox.rect, p->GetHitBox().rect))
		{
			for (Player* q : Variables::players)
			{
				if (q->isDead)
				{
					continue;
				}

				q->OnDie();
			}

			break;
		}
	}
}
