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
}

void Player::Release()
{
}

void Player::Reset()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;

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

	SetOrigin(Origins::BC);
	hitBox.UpdateTransform(body, body.getLocalBounds());
}

void Player::Update(float dt)
{
	animator.Update(dt);

	float h = 0.f;
	if (isGrounded)
	{
		switch (index)
		{
		case 0:
			h = InputMgr::GetAxis(index, Axis::Horizontal);
			velocity.x = h * speed;
			break;
		}
		velocity.x = h * speed;
	}
	if (isGrounded && InputMgr::GetJump(index))
	{
		isGrounded = false;
		velocity.y = -150.f;
	}
	if (!isGrounded)
	{
		velocity += gravity * dt;
	}
	//position += velocity * dt;
	if (position.y > 0.f)
	{
		velocity.y = 0.f;
		//position.y = 0.f;
		isGrounded = true;
	}

	//Collision
	//가로 충돌 검사
	position.x += velocity.x * dt;
	hitBox.UpdateTransform(body, body.getLocalBounds());

	//-1 +2 -2 : 틈새 떨림 방지용 여유 픽셀
	int leftTx = int((hitBox.GetLeft()) / tilemap->GetTileSize());
	int rightTx = int((hitBox.GetRight() - 1) / tilemap->GetTileSize());
	int topTy = int((hitBox.GetTop() + 2) / tilemap->GetTileSize());
	int bottomTy = int((hitBox.GetBottom() - 2) / tilemap->GetTileSize());

	bool collideX = false;

	//경계검사
	if (velocity.x > 0.f && (tilemap->isSolid(rightTx, topTy) || tilemap->isSolid(rightTx, bottomTy)))
	{
		position.x = (rightTx * tilemap->GetTileSize()) - hitBox.GetWidth(); //오른쪽 벽에서 히트박스만큼 왼쪽으로
		collideX = true;
	}
	else if (velocity.x < 0.f && (tilemap->isSolid(leftTx, topTy) || tilemap->isSolid(rightTx, bottomTy)))
	{
		position.x = (leftTx + 1) * tilemap->GetTileSize(); //왼쪽 벽에서 오른쪽으로
		collideX = true;
	}

	if (collideX)
	{
		velocity.x = 0.f;
	}

	//세로 충돌
	position.y += velocity.y * dt;
	hitBox.UpdateTransform(body, body.getLocalBounds());

	leftTx = int((hitBox.GetLeft() + 1) / tilemap->GetTileSize());
	rightTx = int((hitBox.GetRight() - 1) / tilemap->GetTileSize());

	if (velocity.y > 0.f) //낙하
	{
		bottomTy = int((hitBox.GetBottom()) / tilemap->GetTileSize());
		if (tilemap->isSolid(leftTx, bottomTy) || tilemap->isSolid(rightTx, bottomTy))
		{
			position.y = bottomTy * tilemap->GetTileSize() - hitBox.GetHeight(); //바닥 위로 상승
			velocity.y = 0.f;
			isGrounded = true;
		}
	}
	else if (velocity.y < 0.f) //상승
	{
		topTy = int((hitBox.GetTop()) / tilemap->GetTileSize());
		if (tilemap->isSolid(leftTx, topTy) || tilemap->isSolid(rightTx, topTy))
		{
			position.y = (topTy + 1) * tilemap->GetTileSize(); //천장 밑
			velocity.y = 0.f;
		}
	}

	//위치 및 크기 설정
	SetPosition(position);
	
	if (h != 0.f)
	{
		sf::Vector2f s = GetScale();
		s.x = std::abs(s.x) * (h > 0.f ? 1.f : -1.f);
		SetScale(s);
	}

	hitBox.UpdateTransform(body, body.getLocalBounds());

	// Ani
	
}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}