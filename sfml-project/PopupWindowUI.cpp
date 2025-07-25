#include "stdafx.h"
#include "PopupWindowUI.h"

PopupWindowUI::PopupWindowUI(const std::string name)
	:GameObject(name)
{
}

void PopupWindowUI::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	sprite.setPosition(pos);
}
void PopupWindowUI::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	sprite.setRotation(rot);
}
void PopupWindowUI::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	sprite.setScale(s);
}
void PopupWindowUI::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	sprite.setOrigin(o);
}
void PopupWindowUI::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	Utils::SetOrigin(sprite,preset);
}

void PopupWindowUI::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 1;
	SetOrigin(Origins::MC);






}

void PopupWindowUI::Release()
{
}

void PopupWindowUI::Reset()
{
}

void PopupWindowUI::Update(float dt)
{
}

void PopupWindowUI::Draw(sf::RenderWindow& window)
{
}
