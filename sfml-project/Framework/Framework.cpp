#include "stdafx.h"
#include "Framework.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include "PrefabMgr.h"




void Framework::Init(int w, int h, const std::string& t)
{
	window.create(sf::VideoMode(w, h), t);

    ImGui::SFML::Init(window);
    /*ImGuiIO& io = ImGui::GetIO();
    ImFont* font = io.Fonts->AddFontFromFileTTF("fonts/Galmuri11-Bold.ttf", 16.f, nullptr,
        io.Fonts->GetGlyphRangesKorean());
    io.FontDefault = font;
    ImGui::SFML::UpdateFontTexture();*/
	TEXTURE_MGR.Load(texIds);
	FONT_MGR.Load(fontIds);
	SOUNDBUFFER_MGR.Load(soundIds);

    PrefabMgr::I().LoadAll("prefabs");
    Utils::Init();
	InputMgr::Init();
    SOUND_MGR.Init();
	SCENE_MGR.Init();
}

void Framework::Do()
{
    
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
            bool isUnknownKey =
                (event.type == sf::Event::KeyPressed ||
                    event.type == sf::Event::KeyReleased) &&
                event.key.code == sf::Keyboard::Unknown;

            if (!isUnknownKey)
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

        SCENE_MGR.GetCurrentScene()->DrawImGui(); //각 다를수있는 gui 가르키는용도
   

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
