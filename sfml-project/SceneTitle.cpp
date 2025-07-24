#include "stdafx.h"
#include "SceneTitle.h"

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
	
	titleBackground.setTexture(TEXTURE_MGR.Get("graphics/titlebackground.PNG"));

	titleMessage.setFont(FONT_MGR.Get("fonts/BACKTO1982.TTF"));
	titleMessage.setCharacterSize(50);
	Utils::SetOrigin(titleMessage, Origins::MC);
	titleMessage.setString("PRESS ENTER KEY");

	auto size = FRAMEWORK.GetWindowSizeF();
	titleMessage.setPosition(size.x * 0.35f, size.y * 0.5f);
}
void SceneTitle::Update(float dt)
{
	Scene::Update(dt);
	cooltime += dt;
	if (cooltime > 0.9f)
	{
		coolOn = !coolOn;
		cooltime = 0;
	}
	if (coolOn)
	{
		titleMessage.setFillColor(sf::Color(255, 134, 77, 255));
	}
	else
	{
		titleMessage.setFillColor(sf::Color(255, 134, 77, 0));
	}

}
void SceneTitle::Draw(sf::RenderWindow& window)
{
	window.setView(window.getDefaultView());
	window.draw(titleBackground);

	window.draw(titleMessage);
	Scene::Draw(window);
}