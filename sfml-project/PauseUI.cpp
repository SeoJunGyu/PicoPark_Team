#include "stdafx.h"
#include "PauseUI.h"
#include "UiButton.h"
#include "SceneGame.h"
#include "SceneTitle.h"
#include "SceneModeSelect.h"

PauseUI::PauseUI(const std::string name)
	:GameObject(name)
{
}

void PauseUI::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	sprite.setPosition(pos);
	/*text.setPosition(pos);*/
}
void PauseUI::SetRotation(float rot)
{
	PauseUI::SetRotation(rot);
	sprite.setRotation(rot);
	/*text.setRotation(rot);*/
}
void PauseUI::SetScale(const sf::Vector2f& s)
{
	PauseUI::SetScale(s);
	sprite.setScale(s);
	/*text.setScale(s);*/
}
void PauseUI::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	sprite.setOrigin(o);
	//text.setOrigin(o);
}
void PauseUI::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	if (preset != Origins::Custom)
	{
		Utils::SetOrigin(sprite, preset);
		/*Utils::SetOrigin(text, preset);*/
	}
}

void PauseUI::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 1;
	SetOrigin(Origins::MC);

	returnbut = new UiButton("ReturnButton");
	retrybut = new UiButton("RetryButton");
	selectbut = new UiButton("SELECTButton");
	titlebut = new UiButton("ExitButton");

	closebut = new UiButton("CloseButton");

	pusebuts.push_back(returnbut);
	pusebuts.push_back(retrybut);
	pusebuts.push_back(selectbut);
	pusebuts.push_back(titlebut);


	returnbut->SetCallBack([this]()\
	{

		this->SetActive(false);
	});

	returnbut->SetCallBack([]()\
	{
		SCENE_MGR.ChangeScene(SceneIds::Game);
	});

	selectbut->SetCallBack([]()\
	{
		SCENE_MGR.ChangeScene(SceneIds::Select);
	});
	titlebut->SetCallBack([]()\
	{
		SCENE_MGR.ChangeScene(SceneIds::Title);
	});


	closebut->SetCallBack([this]() {this->SetActive(false);	});
}

void PauseUI::Release()
{
	for (auto btn : pusebuts)
	{
		btn->Release();
		delete btn;
	}
	pusebuts.clear();

	delete closebut;
}

void PauseUI::Reset()
{
	sf::Vector2f winSize = FRAMEWORK.GetWindowSizeF();
	sprite.setTexture(TEXTURE_MGR.Get("graphics/puse.png"));
	sprite.setScale(1.5f, 1.5f);
	Utils::SetOrigin(sprite, Origins::MC);

	spr = sprite.getLocalBounds();

	float sprlocalx = (spr.width * sprite.getScale().x) * 0.5f;
	float sprlocaly = (spr.height * sprite.getScale().y) * 0.5f;

	sprite.setPosition(winSize.x * 0.5f, winSize.y * 0.65f);

	sf::Vector2f sprpos = sprite.getPosition();

	std::vector<ButtonStyle> button = {
	   {"RETURN GAME","fonts/BACKTO1982.TTF",50},
		{"RETRY","fonts/BACKTO1982.TTF",50},
	   {"LEVEL SELECT","fonts/BACKTO1982.TTF",50},
	   {"TITLE","fonts/BACKTO1982.TTF",50}
	};

	for (int i = 0; i < pusebuts.size(); ++i)
	{
		pusebuts[i]->SetTextstyle(button[i]);
		pusebuts[i]->GetGlobalBounds();
		pusebuts[i]->SetColor(sf::Color::Black);
		pusebuts[i]->SetPosition({ sprlocalx + 425.f, sprite.getPosition().y + 70.f + (layout * i) });
		pusebuts[i]->SetOrigin(Origins::MC);
		pusebuts[i]->SetActive(true);
		pusebuts[i]->useeffect = true;

	}

	closebut->SetText("X", "fonts/PixelOperator8.ttf", 20);
	closebut->SetColor(sf::Color::Black);
	closebut->GetGlobalBounds();
	closebut->SetPosition({ sprite.getPosition().x + sprlocalx - 30.f,sprite.getPosition().y - sprlocaly + 30.f });

	currentPage = 0;
	pusebuts[currentPage]->effectdrawon = true;
}

void PauseUI::Update(float dt)
{
	returnbut->Update(dt);
	retrybut->Update(dt);
	selectbut->Update(dt);
	titlebut->Update(dt);
	closebut->Update(dt);

	found = false;

	for (int i = 0; i < pusebuts.size(); ++i)
	{
		if (!found && pusebuts[i]->effectdrawon)
		{
			found = true; // 첫 번째 켜진 것만 유지
		}
		else
		{
			pusebuts[i]->effectdrawon = false; // 나머지 꺼버림
		}
	}


	if (InputMgr::GetKeyDown(sf::Keyboard::W) ||
		InputMgr::GetKeyDown(sf::Keyboard::Up))
	{
		for (auto btn : pusebuts)
			btn->effectdrawon = false;

		currentPage--;
		if (currentPage < 0)
		{
			currentPage = pusebuts.size() - 1;
		}
		pusebuts[currentPage]->effectdrawon = true;
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::S) ||
		InputMgr::GetKeyDown(sf::Keyboard::Down))
	{
		for (auto btn : pusebuts)
			btn->effectdrawon = false;

		currentPage++;
		if (currentPage >= pusebuts.size())
		{
			currentPage = 0;
		}
		pusebuts[currentPage]->effectdrawon = true;
	}

}

void PauseUI::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);
	returnbut->Draw(window);
	retrybut->Draw(window);
	selectbut->Draw(window);
	titlebut->Draw(window);
	closebut->Draw(window);
}
