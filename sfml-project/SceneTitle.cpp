#include "stdafx.h"
#include "SceneTitle.h"
#include "UiButton.h"
#include "BackGround.h"
#include "PopupWindowUI.h"


SceneTitle::SceneTitle()
	: Scene(SceneIds::Title)
{
}

void SceneTitle::Init()
{
	texIds.push_back("graphics/titlebackground.PNG");
	texIds.push_back("graphics/MainMenuButton.png");
	texIds.push_back("graphics/rightbut.png");
	texIds.push_back("graphics/rightburpull.png");
	texIds.push_back("graphics/MainMenuButton.png");
	fontIds.push_back("fonts/BACKTO1982.TTF");
	
	titlebackground = new BackGround("Startbackground");
	startButton = new UiButton("Startbutton");
	popup = new PopupWindowUI("popup");

	AddGameObject(titlebackground);
	AddGameObject(startButton);
	AddGameObject(popup);

	Scene::Init();
}
void SceneTitle::Enter()
{
	Scene::Enter();
	auto size = FRAMEWORK.GetWindowSizeF();

	worldView.setSize(size);
	worldView.setCenter(size * 0.5f);
	uiView.setSize(size);
	uiView.setCenter(size * 0.5f);   
	
	titlebackground->Settext("graphics/titlebackground.PNG");
	titlebackground->SetActive(true);
			
	startButton->SetText("PRESS ENTER KEY", "fonts/BACKTO1982.TTF", 50);
	startButton->UiButton::SetPosition({ size.x * 0.35f, size.y * 0.5f });
	startButton->SetColor(sf::Color(255, 134, 77, 255));	
	startButton->GetGlobalBounds();	
	startButton->SetActive(true);
	startButton->SetCallBack([this]() {popup->SetActive(true); });

	popup->SetActive(false);
}
void SceneTitle::Update(float dt)
{	
	Scene::Update(dt);	
	cooltime += dt;
	if (cooltime > 0.9f)
	{
		coolOn = !coolOn;
		startButton->Effect(coolOn);
		cooltime = 0;
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Enter)) {
		//std::cout << "엔터 호출" << std::endl;
		startButton->Trigger();
	}

}
void SceneTitle::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
	
}