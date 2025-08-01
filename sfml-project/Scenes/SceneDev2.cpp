#include "stdafx.h"
#include "SceneDev2.h"
#include "TextGo.h"
#include "Player.h"
#include "Gimmick.h"
#include "json.hpp"
#include <fstream>


SceneDev2::SceneDev2() : Scene(SceneIds::Dev2)
{
}

void SceneDev2::Init()
{
	texIds.push_back("graphics/Characters/Icon/Player0.png");
	texIds.push_back("graphics/Item/key.png");
	texIds.push_back("graphics/Item/door.png");
	texIds.push_back("graphics/Item/doorOpen.png");
	texIds.push_back("graphics/Item/Button.png");
	texIds.push_back("graphics/Item/Pad.png");
	texIds.push_back("graphics/Item/Wall.png");
	texIds.push_back("graphics/Item/WeightBlock.png");

	fontIds.push_back("fonts/DS-DIGIT.ttf");

	TextGo* go = new TextGo("fonts/DS-DIGIT.ttf");
	go->SetString("Dev 2");
	go->SetCharacterSize(30);
	go->SetFillColor(sf::Color::White);
	go->sortingLayer = SortingLayers::UI;
	go->sortingOrder = 0;

	AddGameObject(go);

	//AddGameObject(new Player());

	Scene::Init();
}

void SceneDev2::Enter()
{
	auto size = FRAMEWORK.GetWindowSizeF();
	sf::Vector2f center { size.x * 0.5f, size.y * 0.5f };
	uiView.setSize(size);
	uiView.setCenter(center);
	worldView.setSize(size);
	worldView.setCenter({ 0.f, -200.f });

	Scene::Enter();
}

void SceneDev2::Update(float dt)
{
	Scene::Update(dt);

	if (InputMgr::GetKeyDown(sf::Keyboard::Enter))
	{
		SCENE_MGR.ChangeScene(SceneIds::Dev1);
	}
}

void SceneDev2::DrawImGui()
{
	//�׽�Ʈ�ڵ��
   ImGui::Begin("Debug");  // â�� ����

   
   if (ImGui::Button("Look at this pretty button"))//������ ä���
   {
	   // ��ư Ŭ�� �� ������ �׽�Ʈ �ڵ� ��
	   buttonClicked = !buttonClicked;                // ���
	   std::cout << "Debug ��ư�� ���Ƚ��ϴ�!\n";     // �ܼ� �α�
   }
   ImGui::Checkbox("checkbox", &buttonClicked);


  ImGui::End(); //����
}
