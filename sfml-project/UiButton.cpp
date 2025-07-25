#include "stdafx.h"
#include "UiButton.h"

UiButton::UiButton(const std::string& name)
	: GameObject(name)
{
}

void UiButton::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	sprite.setPosition(pos);
	text.setPosition(pos - sf::Vector2f(0.f, 10.f));
}

void UiButton::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	sprite.setRotation(rot);
	text.setRotation(rot);
}

void UiButton::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	sprite.setScale(s);
	text.setScale(s);
}

void UiButton::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	sprite.setOrigin(o);
	text.setOrigin(o);
}

void UiButton::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	if (preset != Origins::Custom)
	{
		Utils::SetOrigin(sprite, preset);
		Utils::SetOrigin(text, preset);
	}
}

void UiButton::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 1;

	SetOrigin(Origins::MC);
	text.setPosition(sprite.getPosition() - sf::Vector2f(0.f, 10.f));
}

void UiButton::Release()
{
}

void UiButton::Reset()
{
}

void UiButton::Update(float dt)
{
	if (InputMgr::GetMouseButtonDown(sf::Mouse::Left) &&
		sprite.getGlobalBounds().contains((sf::Vector2f)InputMgr::GetMousePosition()))
	{
		if (event)
			event();
	}
}

void UiButton::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);
	window.draw(text);
}