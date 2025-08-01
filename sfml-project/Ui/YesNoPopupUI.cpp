#include "stdafx.h"
#include "YesNoPopupUI.h"
#include "UiButton.h"
#include "Scene.h"
#include "PopupWindowUI.h"

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

	yesnobut.push_back(yesbut);
	yesnobut.push_back(nobut);
	

	/*this->SetActive(false);*/
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
	Utils::SetOrigin(sprite, Origins::MC);
	spr = sprite.getGlobalBounds();

	float sprx = spr.left + spr.width * 0.5f;
	float spry = spr.top + spr.height * 0.5f;
	sprite.setPosition(winSize.x * 0.5f, winSize.y * 0.62f);	

	sf::Vector2f sprpos = sprite.getPosition();

	text.setPosition(sprpos.x -220.f, sprpos.y);
	text.setFillColor(sf::Color::Black);

	yesbut->SetText("YES", "fonts/Pixelownfont-Regular.ttf", 50);
	yesbut->SetColor(sf::Color::Black);
	yesbut->GetGlobalBounds();
	yesbut->SetPosition({ sprpos.x -200.f,sprpos.y + 100.f });
	yesbut->useeffect = true;

	nobut->SetText("NO", "fonts/Pixelownfont-Regular.ttf", 50);
	nobut->SetColor(sf::Color::Black);
	nobut->GetGlobalBounds();
	nobut->SetPosition({ sprpos.x +200.f,sprpos.y + 100.f });
	nobut->useeffect = true;
	nobut->SetCallBack([this]()
		{
			this->Reset();
			this->SetActive(false);
		});

	closebut->SetText("X", "fonts/PixelOperator8.ttf", 25);
	closebut->SetColor(sf::Color::Black);
	closebut->GetGlobalBounds();
	closebut->SetPosition({ sprpos.x + spr.width*0.5f-40.f ,spr.top+10.f });	
	closebut->SetCallBack([this]()
		{
			this->Reset();
			this->SetActive(false);			
		});
	for (auto btn : yesnobut)
		btn->effectdrawon = false;
	yesnobutindex = 1;
	yesnobut[1]->effectdrawon = true;
	enterLock = true;
}

void YesNoPopupUI::Update(float dt)
{
	yesbut->Update(dt);
	nobut->Update(dt);
	closebut->Update(dt);

	if (yesnobut[0]->effectdrawon)
		yesnobut[1]->effectdrawon = false;
	if (yesnobut[1]->effectdrawon)
		yesnobut[0]->effectdrawon = false;

	if (enterLock)
	{
		if (!InputMgr::GetKey(sf::Keyboard::Enter)) // 키가 올라간 뒤
			enterLock = false;
		return; // 잠금 중이면 키 입력 안 받음
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Left)||
		InputMgr::GetKeyDown(sf::Keyboard::A))
	{
		for (auto btn : yesnobut)
			btn->effectdrawon = false;

		yesnobutindex = (yesnobutindex + yesnobut.size() + 1) % yesnobut.size();
		yesnobut[yesnobutindex]->effectdrawon=true;
		UiButton::lastHovered = yesnobut[yesnobutindex];
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Right) ||
		InputMgr::GetKeyDown(sf::Keyboard::D))
	{
		for (auto btn : yesnobut)
			btn->effectdrawon = false;

		yesnobutindex = (yesnobutindex + yesnobut.size() - 1) % yesnobut.size();
		yesnobut[yesnobutindex]->effectdrawon = true;
		UiButton::lastHovered = yesnobut[yesnobutindex];
	}	
	if (InputMgr::GetKeyDown(sf::Keyboard::Enter))
	{
		yesnobut[yesnobutindex]->Trigger();
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Escape))
	{
		closebut->Trigger();
	}
	

}

void YesNoPopupUI::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);
	window.draw(text);
	yesbut->Draw(window);
	nobut->Draw(window);
	closebut->Draw(window);
}
