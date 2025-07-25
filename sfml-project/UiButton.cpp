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
	text.setPosition(pos);
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

void UiButton::SetColor(const sf::Color& color)
{
	text.setFillColor(color);
	sprite.setColor(color);
}

void UiButton::SetText(const std::string& str, const std::string& fontids, unsigned int size)
{
	text.setString(str);
	text.setFont(FONT_MGR.Get(fontids));
	text.setCharacterSize(size);
}

void UiButton::SetSprit(const std::string& texID)
{
	sprite.setTexture(TEXTURE_MGR.Get(texID));
}

void UiButton::Effect(bool on)
{
	sf::Color textcolor = text.getFillColor();
	on ? textcolor.a = 255 : textcolor.a = 0;
	text.setFillColor(textcolor);

	sf::Color spritecolor = sprite.getColor();
	on ? spritecolor.a = 255 : spritecolor.a = 0;
	sprite.setColor(spritecolor);
}



void UiButton::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 1;

	SetOrigin(Origins::MC);	
}

void UiButton::Release()
{
}

void UiButton::Reset()
{
}

void UiButton::Update(float dt)
{
	if ((InputMgr::GetMouseButtonDown(sf::Mouse::Left)||InputMgr::GetKeyDown(sf::Keyboard::Space)) &&
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