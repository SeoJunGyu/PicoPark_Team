#include "stdafx.h"
#include "PopupWindowUI.h"
#include "UiButton.h"
#include "Scene.h"
#include "YesNoPopupUI.h"
#include "SceneGame.h"
#include "Framework.h"



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
	Utils::SetOrigin(sprite, preset);
}

void PopupWindowUI::Init()
{

	sortingLayer = SortingLayers::UI;
	sortingOrder = 1;
	SetOrigin(Origins::MC);

	Local = new UiButton("Local");
	Editor = new UiButton("Editor");
	Exit = new UiButton("Exit");

	Rightbut = new UiButton("Right");
	Leftbut = new UiButton("Left");
	closebut = new UiButton("Close");

	yesno = new YesNoPopupUI("YesNoPopup");

	startbut.push_back(Local);
	startbut.push_back(Editor);
	startbut.push_back(Exit);

	Local->SetCallBack([this]()
		{
			yesno->SetText("START GAME ? ", "fonts/BACKTO1982.TTF", 20);
			yesno->SetActive(true);
			yesno->yesbut->SetCallBack([]() {
				SCENE_MGR.ChangeScene(SceneIds::Select);
				});
		});
	Editor->SetCallBack([this]()
		{
			yesno->SetText("EDTIOR MODE ? ", "fonts/BACKTO1982.TTF", 20);
			yesno->SetActive(true);
			yesno->yesbut->SetCallBack([]() {
				SCENE_MGR.ChangeScene(SceneIds::Select);
				});
		});
	Exit->SetCallBack([this]() {
		yesno->SetText("EXIT GAME ? ", "fonts/BACKTO1982.TTF", 20);
		yesno->SetActive(true);
		yesno->yesbut->SetCallBack([]() {
			FRAMEWORK.GetWindow().close();
			}); });


		Rightbut->SetCallBack([this]()
			{
				startbut[currentPage]->SetActive(false);
				currentPage++;
				if (currentPage >= startbut.size())
				{
					currentPage = 0;
				}
				startbut[currentPage]->SetActive(true);
			});
		Leftbut->SetCallBack([this]()
			{
				startbut[currentPage]->SetActive(false);
				currentPage--;
				if (currentPage < 0)
				{
					currentPage = startbut.size() - 1;
				}
				startbut[currentPage]->SetActive(true);
			});
		closebut->SetCallBack([this]() {this->SetActive(false); });


}

void PopupWindowUI::Release()
{
	for (auto btn : startbut)
	{
		btn->Release();  // 버튼 내부 리소스 해제
		delete btn;      // 버튼 메모리 해제
	}
	startbut.clear();

	delete Rightbut;
	delete Leftbut;
	delete closebut;
	delete yesno;
}

void PopupWindowUI::Reset()
{
	sf::Vector2f winSize = FRAMEWORK.GetWindowSizeF();
	sprite.setTexture(TEXTURE_MGR.Get("graphics/MainMenuButton.png"));

	spr = sprite.getGlobalBounds();

	float sprx = spr.left + spr.width * 0.5f;
	float spry = spr.top + spr.height * 0.5f;

	sprite.setPosition(winSize.x * 0.5f - sprx, winSize.y * 0.5f);

	sf::Vector2f sprpos = sprite.getPosition();

	std::vector<ButtonStyle> button = {
	   {"LOCAL PLAY MODE","fonts/BACKTO1982.TTF",40},
	   {"       OPTION    ","fonts/BACKTO1982.TTF",40},
	   {"     EXIT GAME  ","fonts/BACKTO1982.TTF",40}
	};

	for (int i = 0; i < startbut.size(); ++i)
	{
		startbut[i]->SetTextstyle(button[i]);
		startbut[i]->GetGlobalBounds();
		startbut[i]->SetColor(sf::Color::Black);
		startbut[i]->SetPosition({ sprpos.x + 140.f, sprpos.y + spry + 20.f });
		startbut[i]->SetActive(false);

	}

	Rightbut->SetSprit("graphics/rightbut.png");
	Rightbut->GetGlobalBounds();
	Rightbut->SetScale({ 2.f,2.f });
	Rightbut->SetPosition({ sprpos.x + spr.width - 100.f,sprpos.y + spr.height * 0.6f });

	Leftbut->SetSprit("graphics/rightbut.png");
	Leftbut->SetScale({ -2.f, 2.f });
	Leftbut->GetGlobalBounds();
	Leftbut->SetPosition({ sprpos.x + 100.f,sprpos.y + spr.height * 0.6f });

	closebut->SetText("X", "fonts/BACKTO1982.TTF", 20);
	closebut->SetColor(sf::Color::Black);
	closebut->GetGlobalBounds();
	closebut->SetPosition({ sprpos.x + spr.width - 35.f,sprpos.y + 20.f });

	startbut[currentPage]->SetActive(true);

	yesno->SetActive(false);
}


void PopupWindowUI::Update(float dt)
{

	if (startbut[currentPage]->GetActive())
		startbut[currentPage]->Update(dt);

	Rightbut->Update(dt);
	Leftbut->Update(dt);
	closebut->Update(dt);


	if (Rightbut->isOn)
	{
		Rightbut->SetSprit("graphics/rightburpull.png");
	}
	else
	{
		Rightbut->SetSprit("graphics/rightbut.png");
	}

	if (Leftbut->isOn)
	{
		Leftbut->SetSprit("graphics/rightburpull.png");
	}
	else
	{
		Leftbut->SetSprit("graphics/rightbut.png");
	}



}

void PopupWindowUI::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);

	if (startbut[currentPage]->GetActive())
		startbut[currentPage]->Draw(window);

	Rightbut->Draw(window);
	Leftbut->Draw(window);
	closebut->Draw(window);
}
