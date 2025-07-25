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

	startButton.text.setFont(FONT_MGR.Get("fonts/BACKTO1982.TTF"));
	startButton.text.setCharacterSize(50);
	Utils::SetOrigin(startButton.text, Origins::MC);
	startButton.text.setString("PRESS ENTER KEY");

	auto size = FRAMEWORK.GetWindowSizeF();
	startButton.text.setPosition(size.x * 0.35f, size.y * 0.5f);
}
void SceneTitle::Update(float dt)
{
	sf::Vector2i mousepos = sf::Mouse::getPosition(FRAMEWORK.GetWindow());
	Scene::Update(dt);
	cooltime += dt;
	if (cooltime > 0.9f)
	{
		coolOn = !coolOn;
		cooltime = 0;
	}
	startButton.text.setFillColor
	(coolOn ? sf::Color(255, 134, 77, 255) : sf::Color(255, 134, 77, 0));



}
void SceneTitle::Draw(sf::RenderWindow& window)
{
	window.setView(window.getDefaultView());
	window.draw(titleBackground);
	window.draw(startButton.text);

	Scene::Draw(window);
}