#include "stdafx.h"
#include "Player.h"
#include "TileMap.h"

Player::Player(int idx, const sf::Color& c, const std::string& name)
	: GameObject(name), index(idx), tint(c)
{
}

void Player::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	body.setPosition(pos);
}

void Player::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	body.setRotation(rot);
}

void Player::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	body.setScale(s);
}

void Player::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	body.setOrigin(o);
}

void Player::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	if (preset != Origins::Custom)
	{
		Utils::SetOrigin(body, preset);
	}
}

void Player::Init()
{
	animator.SetTarget(&body);

	animator.AddEvent("Idle", 0,
		[]()
		{
			std::cout << "!!" << std::endl;
		}
	);

	animator.AddEvent("Idle", 0,
		[]()
		{
			std::cout << "??" << std::endl;
		}
	);

	isGrounded = false;
}

void Player::Release()
{
}

void Player::Reset()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 100;

	switch (index)
	{
	case 0:
		body.setTexture(TEXTURE_MGR.Get("graphics/Characters/Icon/Player0.png"));
		break;
	case 1:
		body.setTexture(TEXTURE_MGR.Get("graphics/Characters/Icon/Player1.png"));
		break;
	case 2:
		body.setTexture(TEXTURE_MGR.Get("graphics/Characters/Icon/Player2.png"));
		break;
	case 3:
		body.setTexture(TEXTURE_MGR.Get("graphics/Characters/Icon/Player3.png"));
		break;
	}

	float ox = 16.f * 0.5f;
	float oy = 16.f * 0.5f;

	float realH = body.getTexture()->getSize().y * body.getScale().y;
	float oy_fix = std::abs((realH - 16.f) * 0.5f);

	sf::Vector2f pos(GetPosition().x + ox, GetPosition().y + oy + oy_fix);
	SetPosition(pos);
	
	SetOrigin(Origins::BC);
	hitBox.UpdateTransform(body, body.getLocalBounds());
}

void Player::Update(float dt)
{
	animator.Update(dt);

	isPlayerRider = false; //플레이어가 내 머리 위에 있는지 확인

	if (standing.type != StandType::None)
	{
		if (standing.type == StandType::Platform)
		{
			position += standing.asPlatform()->GetDeltaPos();
		}
		else
		{
			Player* base = standing.asPlayer();
			position += (base->position - base->prvPos); //현재 프레임 이동량
		}
	}

	if (InputMgr::GetJump(index) && !isPlayerRider)
	{

		jumpBufferCounter = jumpBuffer; //점프 입력 기록
	}

	//코요테 타임 갱신
	if (isGrounded)
	{
		coyoteCounter = coyoteTime; //지면에 붙어 있으면 리필
	}
	else
	{
		coyoteCounter -= dt; //공중이면 감소
	}

	//점프 수행 조건
	if (coyoteCounter > 0.f && jumpBufferCounter > 0.f && !isPlayerRider)
	{
		isGrounded = false;
		velocity.y = -jumpPower;
		coyoteCounter = 0.f; //소모
		jumpBufferCounter = 0.f;
	}

	//중력 적용
	jumpBufferCounter -= dt; //시간이 흐르면 버퍼 소멸

	float h = InputMgr::GetAxis(index, Axis::Horizontal);
	velocity.x = h * speed;

	if (!isGrounded)
	{
		velocity.y += gravity.y * dt;
		//standingPlatform = nullptr; //딛고 선 플랫폼 초기화
		standing.clear();
	}
	
	sf::FloatRect prevRect = hitBox.rect.getGlobalBounds();
	prvPos = position;

	//position += velocity * dt;
	position.x += velocity.x * dt;

	//Collision
	// 타일 가로
	body.setPosition(position);
	hitBox.UpdateTransform(body, body.getLocalBounds());
	int ts = tilemap->GetTileSize();
	int leftTx = int(hitBox.GetLeft() / ts);
	int rightTx = int((hitBox.GetRight() + 0.2f) / ts);
	int topTy = int((hitBox.GetTop() + 0.2f) / ts);
	int botTy = int((hitBox.GetBottom() + 0.2f) / ts);

	bool hitRight = tilemap->isSolid(rightTx, topTy) || tilemap->isSolid(rightTx, botTy);
	bool hitLeft = tilemap->isSolid(leftTx, topTy) || tilemap->isSolid(leftTx, botTy);
	if (velocity.x > 0.f && hitRight)
	{
		position.x = prvPos.x;
		velocity.x = 0.f;
	}

	if (velocity.x < 0.f && hitLeft)
	{
		position.x = prvPos.x;
		velocity.x = 0.f;
	}

	// 플랫폼 가로
	body.setPosition(position);
	hitBox.UpdateTransform(body, body.getLocalBounds());
	for (auto* plat : Variables::platforms)
	{
		if (!Utils::CheckCollision(hitBox.rect, plat->GetHitBox().rect))
		{
			continue;
		}

		float prevBottom = prevRect.top + prevRect.height;

		sf::FloatRect playerRect = hitBox.rect.getGlobalBounds();
		sf::FloatRect platRect = plat->GetHitBox().rect.getGlobalBounds();

		if (velocity.y > 0.f && prevBottom <= platRect.top + 0.1f)
		{
			//낙하중이고, 이전 발바닥이 플래폼 윗면보다 위였으면 -> ㅊ옆면 처리 스킵 (착지 후보니까)
			continue;
		}

		if (velocity.y < 0.f && prevRect.top + platRect.height - 0.1f)
		{
			continue;
		}

		//진짜 옆면만 처리
		CollisionInfo info = Utils::GetAABBCollision(playerRect, platRect);

		if (info.depth > 0.f && std::abs(info.normal.x) > 0.5f)
		{
			position.x += info.normal.x * info.depth;
			velocity.x = 0.f;
			SetPosition(position);
			hitBox.UpdateTransform(body, body.getLocalBounds());
			break;
		}
	}

	//수직 이동 갱신
	position.y += velocity.y * dt;

	// 세로
	body.setPosition(position);
	hitBox.UpdateTransform(body, body.getLocalBounds());
	leftTx = int(hitBox.GetLeft() / ts);
	rightTx = int((hitBox.GetRight() + 0.2f) / ts);
	topTy = int((hitBox.GetTop() + 0.2f) / ts);
	botTy = int((hitBox.GetBottom() + 0.2f) / ts);

	bool hitBottom = tilemap->isSolid(leftTx, botTy) || tilemap->isSolid(rightTx, botTy);
	bool hitTop = tilemap->isSolid(leftTx, topTy) || tilemap->isSolid(rightTx, topTy);

	bool wasGround = isGrounded;

	if (velocity.y >= 0.f)
	{
		if (hitBottom)
		{
			position.y = prvPos.y;
			//position.y = std::floor(botTy * ts - hitBox.GetHeight());
			velocity.y = 0.f;
			isGrounded = true;  // 착지
			//isFallen = false;
		}
		else
		{
			if (wasGround)
			{
				isGrounded = false;
				//isFallen = true;
			}
		}
	}
	else if (velocity.y < 0.f && hitTop)
	{
		position.y = prvPos.y;
		//position.y = std::floor((topTy + 1) * ts);
		velocity.y = 0.f;
		//isGrounded = true;
	}

	//플랫폼 세로 충돌
	body.setPosition(position);
	hitBox.UpdateTransform(body, body.getLocalBounds());
	for (auto* plat : Variables::platforms)
	{
		if (!Utils::CheckCollision(hitBox.rect, plat->GetHitBox().rect))
		{
			continue;
		}

		sf::FloatRect playerRect = hitBox.rect.getGlobalBounds();
		sf::FloatRect platRect = plat->GetHitBox().rect.getGlobalBounds();
		CollisionInfo info = Utils::GetAABBCollision(playerRect, platRect);

		if (info.depth <= 0.f || std::abs(info.normal.y) < 0.5f)
		{
			continue;
		}

		if (info.depth > 0.f && std::abs(info.normal.y) > 0.5f)
		{
			position.y += info.normal.y * info.depth;
			SetPosition(position);
			hitBox.UpdateTransform(body, body.getLocalBounds());

			if (info.normal.y < 0.f) //플랫폼 착지
			{
				isGrounded = true;
				//standingPlatform = plat;
				standing.type = StandType::Platform;
				standing.ptr = plat;
				velocity.y = 0.f;
			}
			else if(info.normal.y > 0.f) //플랫폼 하단 충돌
			{
				//velocity.y = std::min(velocity.y, 0.f);
				velocity.y = 0.f;
			}
			break;
		}
		
	}
	body.setPosition(position);
	hitBox.UpdateTransform(body, body.getLocalBounds());

	// 플레이어 세로 충돌
	SetPosition(position);
	hitBox.UpdateTransform(body, body.getLocalBounds());

	bool playerHead = false;

	for (auto* other : Variables::players)
	{
		if (other == this)
		{
			continue;
		}

		if (!Utils::CheckCollision(hitBox.rect, other->GetHitBox().rect))
		{
			continue;
		}

		//겹침 검사
		CollisionInfo info = Utils::GetAABBCollision(hitBox.rect.getGlobalBounds(), other->GetHitBox().rect.getGlobalBounds());

		if (info.depth <= 0.f)
		{
			continue;
		}

		if (info.normal.y < -0.5f) //내가 위 -> 착지
		{
			position.y += info.normal.y * info.depth;
			velocity.y = 0.f;
			isGrounded = true;
			standing.type = StandType::Player;
			standing.ptr = other;

			other->isPlayerRider = true; //너 머리에 플레이어 있다고 알림
			//standingPlatform = other;
		}
		
	}
	
	// 플레이어 가로 충돌
	body.setPosition(position);
	hitBox.UpdateTransform(body, body.getLocalBounds());
	
	for (auto* other : Variables::players)
	{
		if (other == this)
		{
			continue;
		}

		if (!Utils::CheckCollision(hitBox.rect, other->GetHitBox().rect))
		{
			continue;
		}

		//겹침 검사
		float overlapX = std::min(hitBox.GetLeft() + hitBox.GetWidth(), other->GetHitBox().GetLeft() + other->GetHitBox().GetWidth()) - std::max(hitBox.GetLeft(), other->GetHitBox().GetLeft());
		float overlapY = std::min(hitBox.GetTop() + hitBox.GetHeight(), other->GetHitBox().GetTop() + other->GetHitBox().GetHeight()) - std::max(hitBox.GetTop(), other->GetHitBox().GetTop());

		//더 작은 축으로 분리
		if (overlapX < overlapY)
		{
			float dir = position.x < other->position.x ? -1.f : 1.f;
			position.x += dir * overlapX;
			velocity.x = 0.f;
		}
	}

	if (h != 0.f)
	{
		sf::Vector2f s = GetScale();
		s.x = std::abs(s.x) * (h > 0.f ? 1.f : -1.f);
		SetScale(s);
	}

	//화면 경계 검사
	float halfW = hitBox.GetWidth() * 0.5f;

	position.x = Utils::Clamp(position.x, 0.f + halfW, tilemap->GetTileMapSize().x - halfW);

	//최종 위치 설정
	SetPosition(position);
	hitBox.UpdateTransform(body, body.getLocalBounds());
	
	//std::cout << hitBottom << " / " << hitTop << " / " << velocity.y << " / " << isGrounded << std::endl;
	// Ani
}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(body);

	if (Variables::isDrawHitBox)
	{
		hitBox.Draw(window);
	}
	
}
