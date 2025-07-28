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
    fontIds.push_back("fonts/BACKTO1982.TTF");
        
    back = new BackGround("back");
    AddGameObject(back);

    mood1 = new UiButton("mood1");
    AddGameObject(mood1);

    stage1 = new UiButton("stage1");
    AddGameObject(stage1);

    Scene::Init();
}
void SceneModeSelect::Enter()
{
    Scene::Enter();

    auto size = FRAMEWORK.GetWindowSizeF();

    worldView.setSize(size);
    worldView.setCenter(size * 0.5f);
    uiView.setSize(size.x*0.5f,size.y);
    uiView.setCenter(uiView.getSize() * 0.5f);

    sf::Vector2f uiviewbase = uiView.getSize();
    sf::Vector2f worldpos = worldView.getCenter();
        
    text.setFont(FONT_MGR.Get("fonts/BACKTO1982.TTF"));    
    text.setString("HELLO PICO PARK");
    text.setColor(sf::Color(255, 134, 77, 255));
    text.setPosition(uiviewbase.x + 30.f, uiviewbase.y + 30.f);
   
    back->Settext("graphics/moodback.png");
    back->SetActive(true);


    mood1->SetSprit("graphics/stagebox.png");
    mood1->GetGlobalBounds();
    mood1->SetPosition({ worldpos.x *0.5f,worldpos.y*0.3f });
    mood1->SetActive(true);
    mood1->useeffect = true;

    stage1->SetSprit("graphics/stagebox.png");
    stage1->GetGlobalBounds();
    stage1->SetPosition({ worldpos.x * 1.5f,worldpos.y * 0.3f });
    stage1->SetActive(true);
    stage1->useeffect = true;

}

void SceneModeSelect::Update(float dt)
{
    Scene::Update(dt);



}
void SceneModeSelect::Draw(sf::RenderWindow& window)
{
    Scene::Draw(window);
    window.setView(uiView);
    window.draw(text);
}