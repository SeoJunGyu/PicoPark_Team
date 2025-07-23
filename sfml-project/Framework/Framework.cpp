#include "stdafx.h"
#include "Framework.h"
#include "imgui.h"
#include "imgui-SFML.h"




void Framework::Init(int w, int h, const std::string& t)
{
	window.create(sf::VideoMode(w, h), t);

    ImGui::SFML::Init(window);
	TEXTURE_MGR.Load(texIds);
	FONT_MGR.Load(fontIds);
	SOUNDBUFFER_MGR.Load(soundIds);

    Utils::Init();
	InputMgr::Init();
    SOUND_MGR.Init();
	SCENE_MGR.Init();
}

void Framework::Do()
{
    static bool buttonClicked = false; //테스트코드용
    while (window.isOpen())
    {
        sf::Time dt = clock.restart();
        realDeltaTime = deltaTime = dt.asSeconds();
        deltaTime *= timeScale;
        time += deltaTime;
        realTime = realDeltaTime;

        InputMgr::Clear();
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();
            InputMgr::UpdateEvent(event);
        }

        InputMgr::Update(deltaTime);

        // Update
        SOUND_MGR.Update(deltaTime);



#ifdef DEF_DEV
        if (InputMgr::GetKeyDown(sf::Keyboard::F10))
        {
            Variables::isDrawHitBox = !Variables::isDrawHitBox;
        }
#endif
        // sf::seconds 는 float 초 단위 값을 받아서 sf::Time 객체를 만들어 주는 헬퍼 함수
        ImGui::SFML::Update(window, sf::seconds(realDeltaTime));
        SCENE_MGR.Update(deltaTime);

        ImGui::Begin("Debug");  //테스트코드용

        if (ImGui::Button("Look at this pretty button"))
        {
            // 버튼 클릭 시 수행할 테스트 코드 ↓
            buttonClicked = !buttonClicked;                // 토글
            std::cout << "Debug 버튼이 눌렸습니다!\n";     // 콘솔 로그
        }


        ImGui::End();

        // Draw
        window.clear();
        SCENE_MGR.Draw(window);


        ImGui::Render();
        ImGui::SFML::Render(window);
        window.display();
    }
}

void Framework::Release()
{
    ImGui::SFML::Shutdown();
    SOUND_MGR.Release();
	SCENE_MGR.Release();

	SOUNDBUFFER_MGR.Unload(soundIds);
	FONT_MGR.Unload(fontIds);
	TEXTURE_MGR.Unload(texIds);
}
