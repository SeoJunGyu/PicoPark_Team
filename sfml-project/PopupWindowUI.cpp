#include "stdafx.h"
#include "PopupWindowUI.h"
#include "UiButton.h"
#include "Scene.h"
#include "YesNoPopupUI.h"
#include "SceneGame.h"
#include "Framework.h"

//bool UiButton::yesnolock = false;

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
	if (preset != Origins::Custom)
	{
		Utils::SetOrigin(sprite, preset);
	}
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
	yesno->Init();
	startbut.push_back(Local);
	startbut.push_back(Editor);
	startbut.push_back(Exit);


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
	closebut->SetCallBack([this]() {this->SetActive(false);	enterLock = true; });


}

void PopupWindowUI::Release()
{
	for (auto btn : startbut)
	{
		btn->Release();  // ��ư ���� ���ҽ� ����
		delete btn;      // ��ư �޸� ����
	}
	startbut.clear();

	delete Rightbut;
	delete Leftbut;
	delete closebut;
	yesno->Release();
	delete yesno;
	yesno = nullptr;
}

void PopupWindowUI::Reset()
{
	enterLock = true;
	sf::Vector2f winSize = FRAMEWORK.GetWindowSizeF();
	sprite.setTexture(TEXTURE_MGR.Get("graphics/startbut.png"));
	sprite.setScale(1.5f, 1.5f);
	Utils::SetOrigin(sprite, Origins::MC);
	
	

	spr = sprite.getLocalBounds();
	
	float sprlocalx = (spr.width * sprite.getScale().x) * 0.5f;
	float sprlocaly = (spr.height * sprite.getScale().y) * 0.5f;


	sprite.setPosition(winSize.x * 0.5f, winSize.y * 0.65f);

	sf::Vector2f sprpos = sprite.getPosition();

	std::vector<ButtonStyle> button = {
	   {"LOCAL PLAY MODE","fonts/BACKTO1982.TTF",50},
	   {"EDITOR MODE","fonts/BACKTO1982.TTF",50},
	   {"EXIT GAME","fonts/BACKTO1982.TTF",50}
	};

	for (int i = 0; i < startbut.size(); ++i)
	{
		startbut[i]->SetTextstyle(button[i]);
		startbut[i]->GetGlobalBounds();
		startbut[i]->SetColor(sf::Color::Black);
		startbut[i]->SetPosition({ sprlocalx +425.f, sprite.getPosition().y+70.f });
		startbut[i]->SetOrigin(Origins::MC);
		startbut[i]->SetActive(false);
		/*switch (i)
		{
		case 0:
			startbut[i]->SetCallBack([this]()
				{
					SceneGame::SetPendingStage("levels/stage_01.json");
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
		}*/

	}

	Rightbut->SetSprit("graphics/rightbut.png");
	Rightbut->GetGlobalBounds();
	Rightbut->SetScale({ 2.f,2.f });
	Rightbut->SetPosition({ sprite.getPosition().x+sprlocalx-120.f ,sprite.getPosition().y + 50.f });

	Leftbut->SetSprit("graphics/rightbut.png");
	Leftbut->SetScale({ -2.f, 2.f });
	Leftbut->GetGlobalBounds();
	Leftbut->SetPosition({ sprite.getPosition().x-sprlocalx+120.f,sprite.getPosition().y + 50.f });

	closebut->SetText("X", "fonts/PixelOperator8.ttf", 20);
	closebut->SetColor(sf::Color::Black);
	closebut->GetGlobalBounds();
	closebut->SetPosition({ sprite.getPosition().x + sprlocalx -30.f,sprite.getPosition().y-sprlocaly+30.f });

	startbut[currentPage]->SetActive(true);

	Local->SetCallBack([this]()
		{

			yesno->SetText("START GAME", "fonts/Pixelownfont-Regular.ttf", 50);
			yesno->Reset();
			yesno->SetActive(true);
			drawon = false;
			yesno->yesbut->SetCallBack([=]() {SCENE_MGR.ChangeScene(SceneIds::Select); });

		});
	Editor->SetCallBack([this]()
		{
			yesno->SetText("EDTIOR MODE", "fonts/Pixelownfont-Regular.ttf", 50);
			yesno->Reset();
			yesno->SetActive(true);
			drawon = false;
			yesno->yesbut->SetCallBack([]() {SCENE_MGR.ChangeScene(SceneIds::Editor); });

		});
	Exit->SetCallBack([this]() {
		yesno->SetText("    EXIT GAME", "fonts/Pixelownfont-Regular.ttf", 50);
		yesno->Reset();
		yesno->SetActive(true);
		drawon = false;
		yesno->yesbut->SetCallBack([]() {FRAMEWORK.GetWindow().close();	});

		});

	if (yesno != nullptr)
		yesno->Reset();
	yesno->SetActive(false);

}


void PopupWindowUI::Update(float dt)
{
	if (InputMgr::GetKeyDown(sf::Keyboard::Right))
	{
		Rightbut->Trigger();
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Left))
	{
		Leftbut->Trigger();
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Escape))
	{
		closebut->Trigger();
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Enter))
	{
		if (Exit->GetActive()) {
			Exit->Trigger();
		}
		if (Editor->GetActive()) {
			Editor->Trigger();
		}
		if (Local->GetActive()) {
			Local->Trigger();
		}
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
	if (yesno->GetActive())

		
			yesno->Update(dt);
		
}

void PopupWindowUI::Draw(sf::RenderWindow& window)
{
	if (drawon)
	{
		window.draw(sprite);

		if (startbut[currentPage]->GetActive())
			startbut[currentPage]->Draw(window);

		Rightbut->Draw(window);
		Leftbut->Draw(window);
		closebut->Draw(window);
	}
	if (yesno->GetActive())
	{
		yesno->Draw(window);
	}


}
