#include "stdafx.h"
#include "YesNoPopupUI.h"
#include "UiButton.h"
#include "Scene.h"

YesNoPopupUI::YesNoPopupUI(const std::string name)
	: GameObject(name)
{
}

void YesNoPopupUI::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	sprite.setPosition(pos);
	text.setPosition(pos);
}
void YesNoPopupUI::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	sprite.setRotation(rot);
	text.setRotation(rot);
}
void YesNoPopupUI::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	sprite.setScale(s);
	text.setScale(s);
}
void YesNoPopupUI::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	sprite.setOrigin(o);
	text.setOrigin(o);
}
void YesNoPopupUI::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	if (preset != Origins::Custom)
	{
		Utils::SetOrigin(sprite, preset);
		Utils::SetOrigin(text, preset);
	}
}

void YesNoPopupUI::SetText(const std::string& t, const std::string& fontid, int size)
{
	text.setString(t);
	text.setFont(FONT_MGR.Get(fontid));
	text.setCharacterSize(size);
}


void YesNoPopupUI::Init()
{	

	sortingLayer = SortingLayers::UI;
	sortingOrder = 1;
	SetOrigin(Origins::MC);

	yesbut = new UiButton("Yes");
	nobut = new UiButton("No");
	closebut = new UiButton("Close");

	nobut->SetCallBack([this]()
		{
			this->SetActive(false);
		});
	closebut->SetCallBack([this]()
		{
			this->SetActive(false);
		});

	this->SetActive(false);
}

void YesNoPopupUI::Release()
{
	delete yesbut;
	delete nobut;
	delete closebut;
}

void YesNoPopupUI::Reset()
{
	sf::Vector2f winSize = FRAMEWORK.GetWindowSizeF();
	sprite.setTexture(TEXTURE_MGR.Get("graphics/yesnobut.png"));
	sprite.setScale(1.3f, 1.3f);
	spr = sprite.getGlobalBounds();

	float sprx = spr.left + spr.width * 0.5f;
	float spry = spr.top + spr.height * 0.5f;

	sprite.setPosition(winSize.x * 0.5f - sprx, winSize.y * 0.46f);

	sf::Vector2f sprpos = sprite.getPosition();

	text.setPosition(sprpos.x + 200.f, sprpos.y + spr.height * 0.5f);
	text.setFillColor(sf::Color::Black);

	yesbut->SetText("YES", "fonts/Pixelownfont-Regular.ttf", 50);
	yesbut->SetColor(sf::Color::Black);
	yesbut->GetGlobalBounds();
	yesbut->SetPosition({ sprpos.x + 130.f,sprpos.y + spr.height * 0.7f });
	yesbut->useeffect = true;


	nobut->SetText("NO", "fonts/Pixelownfont-Regular.ttf", 50);
	nobut->SetColor(sf::Color::Black);
	nobut->GetGlobalBounds();
	nobut->SetPosition({ sprpos.x + spr.width -200.f,sprpos.y + spr.height * 0.7f });
	nobut->useeffect = true;

	closebut->SetText("X", "fonts/PixelOperator8.ttf", 20);
	closebut->SetColor(sf::Color::Black);
	closebut->GetGlobalBounds();
	closebut->SetPosition({ sprpos.x + spr.width - 35.f,sprpos.y + 20.f });

}

void YesNoPopupUI::Update(float dt)
{
	yesbut->Update(dt);
	nobut->Update(dt);
	closebut->Update(dt);

}

void YesNoPopupUI::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);
	window.draw(text);
	yesbut->Draw(window);
	nobut->Draw(window);
	closebut->Draw(window);
}
