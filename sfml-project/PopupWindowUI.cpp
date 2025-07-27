#include "stdafx.h"
#include "PopupWindowUI.h"
#include "UiButton.h"
#include "Scene.h"
#include "SceneGame.h"


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
	Option = new UiButton("Option");
	Exit = new UiButton("Exit");

	Rightbut = new UiButton("Right");
	Leftbut = new UiButton("Left");
	closebut = new UiButton("Close");

	startbut.push_back(Local);
	startbut.push_back(Option);
	startbut.push_back(Exit);

	Local->SetCallBack([]() {std::cout << "버튼" << std::endl; });
	Option->SetCallBack([]() {std::cout << "버튼" << std::endl; });
	Exit->SetCallBack([]() {std::cout << "버튼" << std::endl; });
		

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
	closebut->SetCallBack([this]() {
		this->SetActive(false); 
		enterLock = true;
		});


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
}

void PopupWindowUI::Reset()
{
	enterLock = true;
	sf::Vector2f winSize = FRAMEWORK.GetWindowSizeF();
	sprite.setTexture(TEXTURE_MGR.Get("graphics/MainMenuButton.png"));
	
	spr = sprite.getGlobalBounds();

	float sprx = spr.left + spr.width * 0.5f;
	float spry = spr.top + spr.height * 0.5f;

	sprite.setPosition(winSize.x * 0.5f - sprx, winSize.y * 0.5f);
	
	sf::Vector2f sprpos = sprite.getPosition();

	std::vector<ButtonStyle> button = {
	   {"LOCAL PLAY MODE","fonts/BACKTO1982.TTF",40},
	   {"       EDITOR    ","fonts/BACKTO1982.TTF",40},
	   {"     EXIT GAME  ","fonts/BACKTO1982.TTF",40}
	};

	for (int i = 0; i < startbut.size(); ++i)
	{
		startbut[i]->SetTextstyle(button[i]);
		startbut[i]->GetGlobalBounds();
		startbut[i]->SetColor(sf::Color::Black);
		startbut[i]->SetPosition({ sprpos.x + 140.f, sprpos.y + spry+20.f  });
		startbut[i]->SetActive(false);
		switch (i)
		{
		case 0:
			startbut[i]->SetCallBack([this]()
				{
					SceneGame::SetPendingStage("levels/stageTest.json");
					SCENE_MGR.ChangeScene(SceneIds::Game);
				});
			break;
		case 1:
			startbut[i]->SetCallBack([this]()
				{
					SCENE_MGR.ChangeScene(SceneIds::Editor);
				});
			break;
		case 2:
			startbut[i]->SetCallBack([this]()
				{
					FRAMEWORK.GetWindow().close();
				});
			break;
		default:
			break;
		}

	}

	Rightbut->SetSprit("graphics/rightbut.png");
	Rightbut->GetGlobalBounds();
	Rightbut->SetScale({ 2.f,2.f });
	Rightbut->SetActive(true);
	Rightbut->SetPosition({ sprpos.x + spr.width - 100.f,sprpos.y + spr.height*0.6f });

	Leftbut->SetSprit("graphics/rightbut.png");
	Leftbut->SetScale({ -2.f, 2.f });
	Leftbut->GetGlobalBounds();
	Leftbut->SetActive(true);
	Leftbut->SetPosition({ sprpos.x + 100.f,sprpos.y + spr.height * 0.6f });

	closebut->SetText("X", "fonts/BACKTO1982.TTF", 20);
	closebut->SetPosition({ spr.width,spr.top });
	closebut->SetColor(sf::Color::Black);
	closebut->GetGlobalBounds();
	closebut->SetPosition({ sprpos.x +spr.width-35.f,sprpos.y+20.f });
	closebut->SetActive(true);

	startbut[currentPage]->SetActive(true);

}


void PopupWindowUI::Update(float dt)
{

	if (enterLock) {
		if (!InputMgr::GetKeyUp(sf::Keyboard::Enter))  
			enterLock = false;
		return;
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Right)) {
		Rightbut->Trigger();          
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Left)) {
		Leftbut->Trigger();
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Escape)) {
		closebut->Trigger();
	}


	if (InputMgr::GetKeyDown(sf::Keyboard::Enter)) {
		startbut[currentPage]->Trigger();
		enterLock = true;            
	}

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
