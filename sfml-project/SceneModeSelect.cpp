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

    back = new BackGround("back");
    AddGameObject(back);

    mood1 = new UiButton("mood1");
    AddGameObject(mood1);

    stage1 = new UiButton("stage1");
    AddGameObject(stage1);

}
void SceneModeSelect::Enter()
{
    back->Settext("graphics/moodback.png");
    mood1->SetSprit("graphics/RigidBox.png");
    stage1->SetSprit("graphics/RigidBox.png");
}

void SceneModeSelect::Update(float dt)
{

}
void SceneModeSelect::Draw(sf::RenderWindow& window)
{

}