#include "stdafx.h"
#include "SceneTitle.h"
#include "UiButton.h"
#include "BackGround.h"

SceneTitle::SceneTitle()
	: Scene(SceneIds::Title)
{
}

void SceneTitle::Init()
{
	texIds.push_back("graphics/titlebackground.PNG");
	fontIds.push_back("fonts/BACKTO1982.TTF");

	Scene::Init();
}
void SceneTitle::Enter()
{
	Scene::Enter();
	auto size = FRAMEWORK.GetWindowSizeF();

	uiView.setSize(size);
	uiView.setCenter(size * 0.5f);   

	titlebackground = new BackGround("Startbackground");
	titlebackground->Settext("graphics/titlebackground.PNG");
	titlebackground->SetActive(true);
	titlebackground->Init();
	//AddGameObject(titlebackground);

	startButton = new UiButton("Startbutton");
	startButton->SetText("PRESS ENTER KEY", "fonts/BACKTO1982.TTF", 50);
	startButton->SetOrigin(Origins::MC);
	startButton->UiButton::SetPosition({ uiView.getSize().x * 0.5f, uiView.getSize().y * 0.5f });
	startButton->SetColor(sf::Color(255, 134, 77, 255));
	startButton->Init();
	//AddGameObject(startButton);
}
void SceneTitle::Update(float dt)
{
	sf::Vector2i mousepos = sf::Mouse::getPosition(FRAMEWORK.GetWindow());
	Scene::Update(dt);
	startButton->Update(dt);
	cooltime += dt;
	if (cooltime > 0.9f)
	{
		coolOn = !coolOn;
		startButton->Effect(coolOn);
		cooltime = 0;
	}

}
void SceneTitle::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
	window.setView(uiView);
	titlebackground->Draw(window);
	startButton->Draw(window);
}