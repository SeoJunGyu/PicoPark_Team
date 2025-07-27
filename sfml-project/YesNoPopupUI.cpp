#include "stdafx.h"
#include "YesNoPopupUI.h"
#include "UiButton.h"

void YesNoPopupUI::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	sprite.setPosition(pos);
}
void YesNoPopupUI::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	sprite.setRotation(rot);
}
void YesNoPopupUI::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	sprite.setScale(s);
}
void YesNoPopupUI::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	sprite.setOrigin(o);
}
void YesNoPopupUI::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	Utils::SetOrigin(sprite, preset);
}

void YesNoPopupUI::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 1;
	SetOrigin(Origins::MC);

	yesbut = new UiButton("Yes");
	nobut = new UiButton("No");

	yesbut->SetCallBack([]() {std::cout << "버튼" << std::endl; });
	nobut->SetCallBack([]() {std::cout << "버튼" << std::endl; });
}

void YesNoPopupUI::Release()
{
	delete yesbut;
	delete nobut;
}

void YesNoPopupUI::Reset()
{


}

void YesNoPopupUI::Update(float dt)
{
}

void YesNoPopupUI::Draw(sf::RenderWindow& window)
{
}
