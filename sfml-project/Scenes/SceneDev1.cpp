﻿#include "stdafx.h"
#include "SceneDev1.h"
#include "TextGo.h"
#include "Framework.h"
#include "ResourceMgr.h"
#include "InputMgr.h"
#include "SceneMgr.h"

SceneDev1::SceneDev1() : Scene(SceneIds::Dev1), rect1(nullptr), rect2(nullptr),
circle1(nullptr), circle2(nullptr), collisionText(nullptr), modeText(nullptr)
{
}

SceneDev1::~SceneDev1()
{
	delete rect1;
	delete rect2;
	delete circle1;
	delete circle2;
}

void SceneDev1::Init()
{

	fontIds.push_back("fonts/DS-DIGIT.ttf");

	// 첫 번째 사각형 (키보드로 회전)
	rect1 = new sf::RectangleShape(sf::Vector2f(100.f, 150.f));
	rect1->setFillColor(sf::Color::Green);
	rect1->setOutlineColor(sf::Color::White);
	rect1->setOutlineThickness(2.f);
	Utils::SetOrigin(*rect1, Origins::MC);
	rect1->setPosition(FRAMEWORK.GetWindowSizeF().x * 0.5f, FRAMEWORK.GetWindowSizeF().y * 0.5f);

	// 두 번째 사각형 (마우스로 이동 및 회전)
	rect2 = new sf::RectangleShape(sf::Vector2f(80.f, 80.f));
	rect2->setFillColor(sf::Color::Blue);
	rect2->setOutlineColor(sf::Color::White);
	rect2->setOutlineThickness(2.f);
	Utils::SetOrigin(*rect2, Origins::MC);

	// 첫 번째 원 (키보드로 회전)
	circle1 = new sf::CircleShape(75.f);
	circle1->setFillColor(sf::Color::Green);
	circle1->setOutlineColor(sf::Color::White);
	circle1->setOutlineThickness(2.f);
	Utils::SetOrigin(*circle1, Origins::MC);
	circle1->setPosition(FRAMEWORK.GetWindowSizeF().x * 0.5f, FRAMEWORK.GetWindowSizeF().y * 0.5f);

	// 두 번째 원 (마우스로 이동)
	circle2 = new sf::CircleShape(40.f);
	circle2->setFillColor(sf::Color::Blue);
	circle2->setOutlineColor(sf::Color::White);
	circle2->setOutlineThickness(2.f);
	Utils::SetOrigin(*circle2, Origins::MC);

	// 충돌 상태 텍스트
	collisionText = new TextGo("fonts/DS-DIGIT.ttf");
	collisionText->SetString("No Collision");
	collisionText->SetCharacterSize(30);
	collisionText->SetFillColor(sf::Color::White);
	collisionText->SetPosition(sf::Vector2f(10.f, 10.f));
	AddGameObject(collisionText);

	// 모드 표시 텍스트
	modeText = new TextGo("fonts/DS-DIGIT.ttf");
	modeText->SetString("Mode: Rectangle");
	modeText->SetCharacterSize(25);
	modeText->SetFillColor(sf::Color::Cyan);
	modeText->SetPosition(sf::Vector2f(10.f, 50.f));
	AddGameObject(modeText);

	// 안내 텍스트
	TextGo* instructionText = new TextGo("fonts/DS-DIGIT.ttf");
	instructionText->SetString("Tab: Switch Mode | Q/E: Rotate Green | Mouse: Move Blue | L/R Click: Rotate Blue | Enter: Next Scene");
	instructionText->SetCharacterSize(20);
	instructionText->SetFillColor(sf::Color::Yellow);
	instructionText->SetPosition(sf::Vector2f(10.f, FRAMEWORK.GetWindowSizeF().y - 30.f));
	AddGameObject(instructionText);

	std::vector<std::string> tilepng = //배열에 문자열 이미지 경로를 넣음
	{
		"graphics/Floor.png",
		"graphics/Spikes.png",
		"graphics/Door_Button.png",
		"graphics/Button.png",
	};

	for (auto& f : tilepng) //문자열 배열을 순회하며 tex에 로드파일 넣고 이미지데이터배열에 넣음
	{
		sf::Texture tex;
		tex.loadFromFile(f);
		tiletextures.push_back(tex);
	};
	tilesize = 16; //타일크기 설정해주고
	sf::Vector2u winSz = FRAMEWORK.GetWindowSize(); //윈도우 사이즈 가져와주고
	mapwidth = gridwidth; // 맵의 너비 설정해주고
	mapheight = gridheight;// 맵의 높이설정해주고
	
	gridLineColor = sf::Color(255, 255, 255, 128); // 레드 그린 블루 투명도
	gridCellShape = sf::RectangleShape(sf::Vector2f(float(tilesize), float(tilesize)));	// 사이즈
	gridCellShape.setOutlineThickness(3.f); //두께

	mapArray.assign(mapheight, std::vector<int>(mapwidth, -1)); // 맵행에 열만큼 -1로 초기화한다.

	Scene::Init();
}

void SceneDev1::Enter()
{
	auto size = FRAMEWORK.GetWindowSizeF();


	sf::Vector2f center{ size.x * 0.5f, size.y * 0.5f };
	uiView.setSize(size);
	uiView.setCenter(center);
	worldView.setSize(size);
	worldView.setCenter(center);

	Scene::Enter();
}

void SceneDev1::Update(float dt)
{
	Scene::Update(dt);

	// 모드 전환 (Tab 키)
	if (InputMgr::GetKeyDown(sf::Keyboard::Tab))
	{
		currentMode = (currentMode == CollisionMode::Rectangle) ? CollisionMode::Circle : CollisionMode::Rectangle;
		modeText->SetString(currentMode == CollisionMode::Rectangle ? "Mode: Rectangle" : "Mode: Circle");
	}

	// 마우스 위치 가져오기
	sf::Vector2i screenPos = InputMgr::GetMousePosition();
	sf::Vector2f mousePos(static_cast<float>(screenPos.x), static_cast<float>(screenPos.y));

	if (currentMode == CollisionMode::Rectangle)
	{
		// 첫 번째 사각형 회전 (Q/E 키)
		if (InputMgr::GetKey(sf::Keyboard::Q))
		{
			rect1->rotate(-90.f * dt);
		}
		if (InputMgr::GetKey(sf::Keyboard::E))
		{
			rect1->rotate(90.f * dt);
		}

		// 두 번째 사각형 마우스 위치로 이동
		rect2->setPosition(mousePos);

		// 마우스 좌클릭으로 두 번째 사각형 회전
		if (InputMgr::GetMouseButton(sf::Mouse::Left))
		{
			rect2->rotate(180.f * dt);
		}
		if (InputMgr::GetMouseButton(sf::Mouse::Right))
		{
			rect2->rotate(-180.f * dt);
		}

		// 충돌 검사
		isColliding = Utils::CheckCollision(*rect1, *rect2);

		// 충돌 상태에 따른 색상 변경
		if (isColliding)
		{
			rect1->setFillColor(sf::Color::Red);
			rect2->setFillColor(sf::Color::Red);
			collisionText->SetString("COLLISION DETECTED!");
			collisionText->SetFillColor(sf::Color::Red);
		}
		else
		{
			rect1->setFillColor(sf::Color::Green);
			rect2->setFillColor(sf::Color::Blue);
			collisionText->SetString("No Collision");
			collisionText->SetFillColor(sf::Color::White);
		}
	}
	else // CollisionMode::Circle
	{
		// 첫 번째 원은 고정 위치 (중앙)
		// 두 번째 원 마우스 위치로 이동
		circle2->setPosition(mousePos);

		// 원형 충돌 검사
		sf::Vector2f center1 = circle1->getPosition();
		float radius1 = circle1->getRadius();
		sf::Vector2f center2 = circle2->getPosition();
		float radius2 = circle2->getRadius();

		isColliding = Utils::CheckCircleCollision(center1, radius1, center2, radius2);

		// 충돌 상태에 따른 색상 변경
		if (isColliding)
		{
			circle1->setFillColor(sf::Color::Red);
			circle2->setFillColor(sf::Color::Red);
			collisionText->SetString("CIRCLE COLLISION DETECTED!");
			collisionText->SetFillColor(sf::Color::Red);
		}
		else
		{
			circle1->setFillColor(sf::Color::Green);
			circle2->setFillColor(sf::Color::Blue);
			collisionText->SetString("No Collision");
			collisionText->SetFillColor(sf::Color::White);
		}
	}
	//테스트코드용
	ImGui::Begin("UI");  // 창을 띄우고

	const int cols = 4; // 열의 개수
	if (ImGui::Button("tile UI"))//내용을 채우고
	{
		showCheckbox = !showCheckbox;
	}
	if (showCheckbox)
	{
		for (int i = 0; i < (int)tiletextures.size(); i++) //텍스쳐 할당된만큼 돌고
		{
			ImTextureID img = (ImTextureID)tiletextures[i].getNativeHandle();

			ImVec2 btnsize(48, 48); //버튼 크기 설저애훚고

			if (ImGui::ImageButton(img, btnsize))
			{
				tileID = i;
			};
			if (tileID == i)
			{
				ImGui::SameLine(0 , 0);
				ImGui::GetWindowDrawList()->AddRect(
					ImGui::GetItemRectMin(),
					ImGui::GetItemRectMax(),
					IM_COL32(255, 255, 0, 255),
					0.0F, 0, 2.0F);
			}
			if ((i + 1) % cols != 0)
			{
				ImGui::SameLine(); // 마지막 줄에 빈 공간 추가
			}
		}
		auto pos = InputMgr::GetMousePosition();
		int tx = pos.x / tilesize;
		int ty = pos.y / tilesize;
		if (InputMgr::GetMouseButtonDown(sf::Mouse::Left) && tileID >= 0)
		{
			if (tx >= 0 && tx < mapwidth && ty >= 0 && ty < mapheight)
			{
				mapArray[ty][tx] = tileID;
			}
		}
		if (InputMgr::GetMouseButtonDown(sf::Mouse::Right))
		{
			if (tx >= 0 && tx < mapwidth && ty >= 0 && ty < mapheight)
			{
				mapArray[ty][tx] = -1;
			}
		}
		if (InputMgr::GetMouseButton(sf::Mouse::Left) && tileID >= 0)
		{
			if (tx >= 0 && tx < mapwidth && ty >= 0 && ty < mapheight)
			{
				mapArray[ty][tx] = tileID;
			}
		}
		if (InputMgr::GetMouseButton(sf::Mouse::Right) && tileID >= 0)
		{
			if (tx >= 0 && tx < mapwidth && ty >= 0 && ty < mapheight)
			{
				mapArray[ty][tx] = -1;
			}
		}

	}

	ImGui::End(); //종료




	// 씬 전환
	if (InputMgr::GetKeyDown(sf::Keyboard::Enter))
	{
		SCENE_MGR.ChangeScene(SceneIds::Dev2);
	}
}

void SceneDev1::Draw(sf::RenderWindow& window)
{
	window.setView(worldView);
	Scene::Draw(window);
	if (buttonClicked)
	{
		if (currentMode == CollisionMode::Rectangle)
		{
			if (rect1)
				window.draw(*rect1);
			if (rect2)
				window.draw(*rect2);
		}
		else // CollisionMode::Circle
		{
			if (circle1)
				window.draw(*circle1);
			if (circle2)
				window.draw(*circle2);
		}
	}
	for (int y = 0; y < mapheight; ++y)
	{
		for (int x = 0; x < mapwidth; ++x)
		{
			gridCellShape.setPosition(
				float(x * tilesize),
				float(y * tilesize)
			);
			window.draw(gridCellShape);
		}
	}

	for (int y = 0; y < mapheight; ++y) {
		for (int x = 0; x < mapwidth; ++x) {
			int id = mapArray[y][x];
			// id가 유효하다면(여기선 0 이상)
			if (id < 0 || id >= (int)tiletextures.size()) continue;
			sf::Sprite spr(tiletextures[id]);
			sf::Vector2u texSz = tiletextures[id].getSize();
			spr.setScale(
				float(tilesize) / float(texSz.x),
				float(tilesize) / float(texSz.y)
			);
			spr.setPosition(float(x * tilesize), float(y * tilesize));
			window.draw(spr);
		}
		
	}
	window.setView(uiView);
	ImGui::SFML::Render(window);
}





