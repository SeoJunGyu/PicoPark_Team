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

	if (isGrounded && InputMgr::GetJump(index))
	{
		isGrounded = false;
		a = false;
		velocity.y = -jumpPower;    // 충분히 큰 값, e.g. 300.f
	}
	else
	{
		a = true;
	}

	float h = InputMgr::GetAxis(index, Axis::Horizontal);
	velocity.x = h * speed;

	if (!isGrounded)
	{
		velocity.y += gravity.y * dt;
	}
	
	prvPos = position;
	position += velocity * dt;

	//Collision

	body.setPosition(position);
	hitBox.UpdateTransform(body, body.getLocalBounds());

	int ts = tilemap->GetTileSize();
	int leftTx = int(hitBox.GetLeft() / ts);
	int rightTx = int((hitBox.GetRight() - 1) / ts);
	int topTy = int((hitBox.GetTop() + 1) / ts);
	int botTy = int((hitBox.GetBottom() - 1) / ts);

	// 가로
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
	rightTx = int((hitBox.GetRight() - 1) / ts);
	topTy = int((hitBox.GetTop() + 1) / ts);
	botTy = int((hitBox.GetBottom() - 1) / ts);

	bool hitBottom = tilemap->isSolid(leftTx, botTy) || tilemap->isSolid(rightTx, botTy);
	bool hitTop = tilemap->isSolid(leftTx, topTy) || tilemap->isSolid(rightTx, topTy);
	if (velocity.y >= 0.f && hitBottom)
	{
		position.y = prvPos.y;
		velocity.y = 0.f;
		isGrounded = true;  // 착지
	}
	else if (a)
	{
		velocity.y = 100.f;
		isGrounded = true;
	}
	if (velocity.y < 0.f && hitTop)
	{
		position.y = prvPos.y;
		velocity.y = 0.f;
	}

	if (h != 0.f)
	{
		sf::Vector2f s = GetScale();
		s.x = std::abs(s.x) * (h > 0.f ? 1.f : -1.f);
		SetScale(s);
	}
	
	SetPosition(position);
	hitBox.UpdateTransform(body, body.getLocalBounds());
	
	//std::cout << hitBox.rect.getPosition().x << " / " << hitBox.rect.getPosition().y << std::endl;
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