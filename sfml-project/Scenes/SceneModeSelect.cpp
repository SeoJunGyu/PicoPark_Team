#include "stdafx.h"
#include "SceneModeSelect.h"
#include "SceneGame.h"
#include "BackGround.h"
#include "UiButton.h"
#include <regex>
SceneModeSelect::SceneModeSelect()
      :Scene(SceneIds::Select)
{
}

void SceneModeSelect::Init()
{
    texIds.push_back("graphics/moodback.png");
    texIds.push_back("graphics/anibox.png");
    fontIds.push_back("fonts/BACKTO1982.TTF");
        
    SOUNDBUFFER_MGR.Load("audio/02Bounds.mp3");
    SOUND_MGR.SetBgmVolume(50);  

    back = new BackGround("back");
    AddGameObject(back);    

   /* stage = new UiButton("stage");
    AddGameObject(stage);*/

   /* stage->SetCallBack([]()
        {
            SCENE_MGR.ChangeScene(SceneIds::Game);
        });*/

    Scene::Init();
}
void SceneModeSelect::Enter()
{
    Scene::Enter();    
    stages.clear();
    SOUND_MGR.PlayBgm("audio/02Bounds.mp3");
    SOUND_MGR.SetBgmVolume(50);

    auto size = FRAMEWORK.GetWindowSizeF();

    worldView.setSize(size);
    worldView.setCenter(size * 0.5f);
    uiView.setSize(size);
    uiView.setCenter(size * 0.5f);

    sf::Vector2f worldpos = worldView.getCenter();

    back->Settext("graphics/moodback.png");
    back->SetActive(true);

    text.setFont(FONT_MGR.Get("fonts/BACKTO1982.TTF"));
    text.setString("HELLO PICO PARK");
    text.setFillColor(sf::Color(255, 134, 77, 255));
    text.setPosition(size.x * 0.1f - 30.f, size.y * 0.1f - 50.f);
    text.setCharacterSize(80);

    std::regex re(R"(stage_(\d+)\.json)", std::regex::icase);

    for (auto& f : fs::directory_iterator(LEVEL_DIR))
    {
        std::smatch m;
        std::string fname = f.path().filename().string();
        if (std::regex_match(fname, m, re))
        {
            int idx = std::stoi(m[1]);          // “stage_12.json” -> 12
            StageEntry e;
            e.file = f.path().string();
            e.index = idx;

            std::cout << "파일 경로 : " << e.file << std::endl;
            std::ifstream fin(e.file);
            nlohmann::json j;  fin >> j;
            e.author = j.value("author", "");
            e.desc = j.value("description", "");
            stages.push_back(std::move(e));
            //stages.push_back({ f.path().string(), idx, nullptr });
        }
    }
    std::sort(stages.begin(), stages.end(),
        [](auto& a, auto& b) { return a.index < b.index; });

    const int COLS = 10;        // 한 줄에 몇 개 여부
    const float GAP = 180.f;    // 버튼 사이 간격
    sf::Vector2f startPos(120.f, 300.f);

    for (size_t i = 0; i < stages.size(); ++i)
    {
        UiButton* b = new UiButton("stageBtn" + std::to_string(i));
        stages[i].btn = b;
        AddGameObject(b);

        b->SetSprit("graphics/anibox.png");
        b->SetScale({ 3.f,3.f });
        b->SetOrigin(Origins::MC);
        b->useeffect = true;

        sf::Text numText;
        numText.setFont(FONT_MGR.Get("fonts/BACKTO1982.TTF"));
        numText.setString(std::to_string(stages[i].index));
        numText.setFillColor(sf::Color(255, 134, 77));
        numText.setCharacterSize(48);
        b->SetInnerText(numText);        

        int row = static_cast<int>(i) / COLS;
        int col = static_cast<int>(i) % COLS;
        b->SetPosition(startPos + sf::Vector2f(col * GAP, row * GAP));

        b->SetCallBack([file = stages[i].file, back = this->back, this]()
            {
                SceneGame::SetPendingStage(file);
                if (back)
                {
                    back->StartFadeOut();
                }
                SOUND_MGR.StartFadeOut();
                isSceneChanging = true;
                onSceneChange = [this]() {
                    SCENE_MGR.ChangeScene(SceneIds::Game);
                    };              
            });
    }
}

void SceneModeSelect::Update(float dt)
{
    Scene::Update(dt);

    if (isSceneChanging)
    {
        if (back->IsFadeOutComplete())
        {
            if (onSceneChange)
            {
                onSceneChange();  // fade 완료 후 씬 전환
            }
            isSceneChanging = false;
        }
    }

    if (InputMgr::GetKeyDown(sf::Keyboard::Escape)) {
        if (back)
        {
            back->StartFadeOut();
        }
        SOUND_MGR.StartFadeOut();
        isSceneChanging = true;
        onSceneChange = [this]() {
            SCENE_MGR.ChangeScene(SceneIds::Title);
            };        
    }

}
void SceneModeSelect::Draw(sf::RenderWindow& window)
{
    Scene::Draw(window);   
    window.draw(text);
    if (back && back->isFadingout)  // 또는 fade 중 조건
    {
        window.draw(back->whiteOverlay);
    }
    //window.draw(stagetext);
 
    // ── Hover‑tooltip -------------------------------------------------
    StageEntry* hovered = nullptr;
    for (auto& s : stages)
    {
        if (s.btn && s.btn->IsMouseOver())
        {
            hovered = &s;
            break;
        }
    }

    if (hovered)
    {
        sf::Vector2f mp =
            window.mapPixelToCoords(InputMgr::GetMousePosition(), uiView);

        sf::Text info;
        info.setFont(FONT_MGR.Get("fonts/BACKTO1982.TTF"));
        info.setCharacterSize(24);
        info.setFillColor(sf::Color::White);
        info.setString("Author : " + hovered->author + "\n"
            "Desc   : " + hovered->desc);

        sf::FloatRect tb = info.getLocalBounds();
        const float PAD = 8.f;

        sf::RectangleShape box(
            { tb.width + PAD * 2.f, tb.height + PAD * 2.f });
        box.setFillColor(sf::Color(0, 0, 0, 200));
        box.setOutlineThickness(1.f);
        box.setOutlineColor(sf::Color::White);
        box.setPosition(mp.x + 15.f, mp.y + 15.f);     // 살짝 옆으로

        info.setPosition(box.getPosition() + sf::Vector2f(PAD, PAD));

        window.draw(box);
        window.draw(info);
    }
    // ────────────────────────────────────────────────────────────────
}