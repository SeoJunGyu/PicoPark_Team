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

	if (InputMgr::GetJump(index))
	{
		
		//isFallen = false;
		

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
	if (coyoteCounter > 0.f && jumpBufferCounter > 0.f)
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
	}
	
	sf::FloatRect prevRect = hitBox.rect.getGlobalBounds();
	prvPos = position;

	position += velocity * dt;

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
		float overlapX = std::min(hitBox.GetLeft() + hitBox.GetWidth(), other->GetHitBox().GetLeft() + other->GetHitBox().GetWidth()) - std::max(hitBox.GetLeft(), other->GetHitBox().GetLeft());
		float overlapY = std::min(hitBox.GetTop() + hitBox.GetHeight(), other->GetHitBox().GetTop() + other->GetHitBox().GetHeight()) - std::max(hitBox.GetTop(), other->GetHitBox().GetTop());

		float prevBottom = prevRect.top + prevRect.height;
		float otherTop = other->GetHitBox().GetTop();

		bool fallingHead = (velocity.y > 0.f) && (prevBottom <= otherTop) && (overlapY <= overlapX);

		//y 겹침이 더 크고, 머리에 떨어진것이면
		if (fallingHead)
		{
			position.y -= overlapY;
			velocity.y = jumpPower;
			isGrounded = true;

			jumpBufferCounter = 0.f;
			coyoteCounter = 0.f;

			playerHead = true;
			break;
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
		/*
		else
		{
			float dir = position.y < other->position.y ? -1.f : 1.f;
			position.x += dir * overlapY;
			velocity.y = 0.f;
			if (dir > 0)
			{
				isGrounded = true;
			}
		}
		*/
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
