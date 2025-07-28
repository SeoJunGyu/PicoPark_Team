#include "stdafx.h"
#include "SceneModeSelect.h"
#include "BackGround.h"
#include "UiButton.h"
SceneModeSelect::SceneModeSelect()
      :Scene(SceneIds::Select)
{
}

void SceneModeSelect::Init()
{
    texIds.push_back("graphics/moodback.png");
    texIds.push_back("graphics/anibox.png");
    fontIds.push_back("fonts/BACKTO1982.TTF");
        
    back = new BackGround("back");
    AddGameObject(back);    

    stage = new UiButton("stage");
    AddGameObject(stage);

    stage->SetCallBack([]()
        {
            SCENE_MGR.ChangeScene(SceneIds::Game);
        });

    Scene::Init();
}
void SceneModeSelect::Enter()
{
    Scene::Enter();

    auto size = FRAMEWORK.GetWindowSizeF();

    worldView.setSize(size);
    worldView.setCenter(size * 0.5f);
    uiView.setSize(size);
    uiView.setCenter(size * 0.5f);

    sf::Vector2f worldpos = worldView.getCenter();

    text.setFont(FONT_MGR.Get("fonts/BACKTO1982.TTF"));
    text.setString("HELLO PICO PARK");
    text.setFillColor(sf::Color(255, 134, 77, 255));
    text.setPosition(size.x * 0.1f - 30.f, size.y * 0.1f - 50.f);
    text.setCharacterSize(80);
    
    back->Settext("graphics/moodback.png");
    back->SetActive(true);

    stage->SetSprit("graphics/anibox.png");
    stage->GetGlobalBounds();
    stage->SetPosition({ size.x * 0.1f, size.y *0.3f});
    stage->SetActive(true);
    stage->SetScale({3.f, 3.f});
    stage->SetOrigin(Origins::MC);
    stage->useeffect = true;

    stagetext.setFont(FONT_MGR.Get("fonts/BACKTO1982.TTF"));
    stagetext.setString("1");
    stagetext.setFillColor(sf::Color(255, 134, 77, 255));
    stagetext.setPosition(stage->GetPosition().x-10.f, stage->GetPosition().y-35.f);
    stagetext.setCharacterSize(50);

}

void SceneModeSelect::Update(float dt)
{
    Scene::Update(dt);



}
void SceneModeSelect::Draw(sf::RenderWindow& window)
{
    Scene::Draw(window);   
    window.draw(text);
    window.draw(stagetext);
}