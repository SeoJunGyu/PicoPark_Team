#include "stdafx.h"
#include "Player.h"

Player::Player(const std::string& name)
	: GameObject(name)
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

	body.setTexture(TEXTURE_MGR.Get(texId));

	SetOrigin(Origins::BC);
}

void Player::Update(float dt)
{
	animator.Update(dt);

	float h = 0.f;
	if (isGrounded)
	{
		h = InputMgr::GetAxis(Axis::Horizontal);
		velocity.x = h * speed;
	}
	if (isGrounded && InputMgr::GetKeyDown(sf::Keyboard::W))
	{
		isGrounded = false;
		velocity.y = -250.f;
	}
	if (!isGrounded)
	{
		velocity += gravity * dt;
	}
	prvPos = position;
	position += velocity * dt;
	if (position.y > 0.f)
	{
		velocity.y = 0.f;
		//position.y = 0.f;
		isGrounded = true;
	}
	SetPosition(position);

	if (h != 0.f)
	{
		SetScale({ 0.1f * (h > 0.f ? 1.f : -1.f), 0.1f });
		//SetScale(h > 0.f ? sf::Vector2f(1.0f, 1.0) : sf::Vector2f(-1.f, 1.0f));
	}

	// Ani
	
}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}