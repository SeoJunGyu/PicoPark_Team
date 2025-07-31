#include "stdafx.h"
#include "SceneGame.h"
#include "Player.h"
#include "Gimmick.h"
#include "PrefabMgr.h"

std::string SceneGame::pendingStage;
bool SceneGame::isEditor = false;

constexpr float BASE_W = 240.f;   // 15 tile
constexpr float BASE_H = 144.f;   //  9 tile
constexpr float TILE = 16.f;

static sf::Color makeColor(int tileId)
{
    if (tileId == 0)         
        return sf::Color::Transparent;

    static std::array<sf::Color, 6> palette = {
        sf::Color(52,153,126),
        sf::Color(240,189, 72),
        sf::Color(206, 86,207),
        sf::Color(52,128,255),
        sf::Color(255, 96,122),
        sf::Color(128,128,128)
    };

    static std::vector<sf::Color> palette2 = {
        sf::Color::Blue,
        sf::Color(207, 159, 253),
        sf::Color::Green,
        sf::Color::Yellow
    };
    //return palette[(tileId - 1) % palette.size()];
    return palette2[tileId];
}

void SceneGame::LoadStage(const std::string& jsonPath)
{
    ClearStage();
    Button::ClearStates(); //버튼 래치 및 누름상태 초기화
    Variables::ResetStage();
    std::vector<sf::Vector2f> spawnPoints;
    std::ifstream fin(jsonPath);
    nlohmann::json j;
    fin >> j; //파일 파싱 : 필요한 데이터를 가져와서 사용하는 행위?

    for (const auto& entobj : j["entities"])
    {
        std::string tstr = entobj.at("type").get<std::string>();
        /*float ox = level->tileSize * 0.5f;
        float oy = level->tileSize * 0.5f;

        sf::Vector2f pos(entobj.at("x").get<float>() + ox, entobj.at("y").get<float>() + oy);*/


       /* if (Gimmick* g = Gimmick::CreateFromJson(entobj))
        {
            g->Init();
            g->Reset();
            Variables::gimmicks.push_back(g);
            AddGameObject(g);
            if (tstr == "MovingPlatform")
            {
                Variables::platforms.push_back(dynamic_cast<MovingPlatform*>(g));
            }
        }*/

        if (tstr == "PlayerSpawn")
        {
            //int tmp = entobj["properties"].value("playerIndex", 0);
            float x = entobj.at("x").get<float>();
            float y = entobj.at("y").get<float>();
            spawnPoints.emplace_back(x, y);
            continue;          // Gimmick 생성 생략
        }

        sf::Vector2f scl(entobj["scale"][0], entobj["scale"][1]);

        GameObject* g = PrefabMgr::I().Instantiate(
            entobj["type"],
            { entobj["x"], entobj["y"] }, scl,
            entobj.value("properties", nlohmann::json::object()));
        if (g) {
            g->Init();
            g->Reset();
            Variables::gimmicks.push_back((Gimmick*)g);
            AddGameObject(g);
            if (tstr == "MovingPlatform")
            {
                Variables::platforms.push_back(dynamic_cast<MovingPlatform*>(g));
            }
            else if (tstr == "PushBlock")
            {
                Variables::blocks.push_back(dynamic_cast<PushBlock*>(g));
            }
        }
        /*float ox = level->tileSize * 0.5f;
        float oy = level->tileSize * 0.5f;

        sf::Vector2f pos(entobj.at("x").get<float>() + ox, entobj.at("y").get<float>() + oy);*/


        //if (Gimmick* g = Gimmick::CreateFromJson(entobj))
        //{
        //    g->Init();
        //    g->Reset();
        //    Variables::gimmicks.push_back(g);
        //    AddGameObject(g);
        //}

    }

    int idx = 0;
    for (auto& pos : spawnPoints)
    {
        sf::Color col = makeColor(idx);

        Player* p = new Player(idx, col, "Player" + std::to_string(idx));              // Player 클래스 생성자 (index)
        p->setStage(jsonPath);
        p->SetPosition(pos);
        p->SetScale({ 0.1f, 0.09f });
        p->Init();   
        p->Reset();
        p->SetTileMap(tileMap);
        Variables::players.push_back(p);
        //std::cout << p->GetPosition().x << " / " << p->GetPosition().y << std::endl;
        AddGameObject(p);
        ++idx;
    }
}

void SceneGame::ClearStage()
{
    for (auto* obj : gameObjects) 
    {
        delete obj;
    }
    gameObjects.clear();
    Variables::players.clear();
    Variables::gimmicks.clear();
    Variables::platforms.clear();
    Variables::blocks.clear();
}

void SceneGame::updateCamera(float dt)
{
    sf::FloatRect bb;
    bool first = true;
    for (auto* p : Variables::players) {
        sf::Vector2f pos = p->GetPosition();
        if (first) { bb = { pos.x, pos.y, 0, 0 }; first = false; }
        else {
            bb.left = std::min(bb.left, pos.x);
            bb.top = std::min(bb.top, pos.y);
            bb.width = std::max(bb.width, pos.x - bb.left);
            bb.height = std::max(bb.height, pos.y - bb.top);
        }
    }
    float margin = TILE;                
    bb.left -= margin; bb.top -= margin;
    bb.width += margin * 2.f; bb.height += margin * 2.f;

    float zX = bb.width / BASE_W;
    float zY = bb.height / BASE_H;
    float zoom = std::ceil(std::max({ 1.f, zX, zY }));   

    float mapW = level->gridWidth * TILE;
    float mapH = level->gridHeight * TILE;

    float viewW = std::min(BASE_W * zoom, mapW);
    float viewH = std::min(BASE_H * zoom, mapH);


    sf::Vector2f centre(bb.left + bb.width * 0.5f,
        bb.top + bb.height * 0.5f);

    sf::Vector2f half(viewW * 0.5f, viewH * 0.5f);


    float centreX = (mapW > viewW)
        ? Utils::Clamp(centre.x, half.x, mapW - half.x)
        : mapW * 0.5f;
    float centreY = (mapH > viewH)
        ? Utils::Clamp(centre.y, half.y, mapH - half.y)
        : mapH * 0.5f;

    centre = { std::round(centreX), std::round(centreY) };

  /*  centre.x = Utils::Clamp(centre.x, half.x, mapW - half.x);
    centre.y = Utils::Clamp(centre.y, half.y, mapH - half.y);

    centre.x = std::round(centre.x);
    centre.y = std::round(centre.y);*/


    sf::Vector2f cur = worldView.getCenter();
    float smooth = 8.f;
    centre = cur + (centre - cur) * dt * smooth;

    worldView.setCenter(centre);
    worldView.setSize(viewW, viewH);
    applyLetterBox(worldView, FRAMEWORK.GetWindow().getSize());

    //sf::FloatRect playersBB;            // 모든 플레이어의 AABB
    //bool first = true;
    //for (auto* p : Variables::players) {
    //    sf::Vector2f pos = p->GetPosition();
    //    if (first) {
    //        playersBB = { pos.x, pos.y, 0, 0 };
    //        first = false;
    //    }
    //    else {
    //        playersBB.left = std::min(playersBB.left, pos.x);
    //        playersBB.top = std::min(playersBB.top, pos.y);
    //        playersBB.width = std::max(playersBB.width, pos.x - playersBB.left);
    //        playersBB.height = std::max(playersBB.height, pos.y - playersBB.top);
    //    }
    //}

    //bool fits =
    //    playersBB.width <= VIEW_W * 0.9f &&   // 10% 여유
    //    playersBB.height <= VIEW_H * 0.9f;

    //if (fits) {
    //    sf::Vector2f targetCenter(
    //        playersBB.left + playersBB.width * 0.5f,
    //        playersBB.top + playersBB.height * 0.5f);

    //    // 부드러운 이동(옵션) : LERP
    //    sf::Vector2f cur = worldView.getCenter();
    //    float   smooth = 5.f;                 // 값이 클수록 빠르게 따라감
    //    targetCenter = cur + (targetCenter - cur) * dt * smooth;

    //    // 맵 경계 밖으로 나가지 않도록 클램프
    //    sf::Vector2f half = worldView.getSize() * 0.5f;
    //    float mapW = level->gridWidth * level->tileSize;
    //    float mapH = level->gridHeight * level->tileSize;
    //    targetCenter.x = Utils::Clamp(targetCenter.x, half.x, mapW - half.x);
    //    targetCenter.y = Utils::Clamp(targetCenter.y, half.y, mapH - half.y);

    //    worldView.setCenter(targetCenter);
    //}
}

void SceneGame::applyLetterBox(sf::View& v, sf::Vector2u win)
{
    float winR = (float)win.x / win.y;
    float viewR = v.getSize().x / v.getSize().y;
    sf::FloatRect vp;
    if (viewR < winR) {                  
        float pad = (1.f - viewR / winR) * 0.5f;
        vp = { pad, 0.f, 1.f - pad * 2.f, 1.f };
    }
    else {                             
        float pad = (1.f - winR / viewR) * 0.5f;
        vp = { 0.f, pad, 1.f, 1.f - pad * 2.f };
    }
    v.setViewport(vp);
}

//void SceneGame::buildWorld(const Level& lvl)
//{
//    tileMap.init(lvl.gridWidth, lvl.gridHeight, lvl.layers);
//    for (const auto& ent : lvl.entities)
//        gimmicks.push_back(GimmickFactory::build(ent));
//}

SceneGame::SceneGame()
	: Scene(SceneIds::Game)
{
}

SceneGame::~SceneGame()
{
    delete level;
    level = nullptr;
}

void SceneGame::Init()
{
    texIds.push_back("graphics/Characters/Icon/Player0.png");
    texIds.push_back("graphics/Characters/Icon/Player1.png");
    texIds.push_back("graphics/Characters/Icon/Player2.png");
    texIds.push_back("graphics/Characters/Icon/Player3.png");
    texIds.push_back("graphics/Characters/Player_Death.png");
    texIds.push_back("graphics/Characters/Player1_Death.png");
    texIds.push_back("graphics/Characters/Player2_Death.png");
    texIds.push_back("graphics/Characters/Player3_Death.png");
    texIds.push_back("graphics/Item/key.png");
    texIds.push_back("graphics/Item/door.png");
    texIds.push_back("graphics/Item/doorOpen.png");
    texIds.push_back("graphics/Item/Button.png");
    texIds.push_back("graphics/Item/Button_Pressed.png");
    texIds.push_back("graphics/Item/Pad.png");
    texIds.push_back("graphics/Item/jump1.png");
    texIds.push_back("graphics/Item/jump3.png");
    texIds.push_back("graphics/Item/FallingPlatform.png");
    texIds.push_back("graphics/Item/RoundPlatform.png");
    texIds.push_back("graphics/Item/elevator.png");
    texIds.push_back("graphics/Item/WeightBlock.png");
    texIds.push_back("graphics/Item/BlinkerRed.png");
    texIds.push_back("graphics/Item/BlinkerOrange.png");
    texIds.push_back("graphics/Item/Blinkergreen.png");
    //texIds.push_back("graphics/Characters/Pico Player Jump Final.png");
    texIds.push_back("graphics/Characters/Pico_Player_Idle_Final.png");
    texIds.push_back("graphics/Characters/Pico_Player_Jump_Final.png");
    texIds.push_back("graphics/Characters/Pico_Player_walk_right_final.png");
    texIds.push_back("graphics/Characters/player_pushwalk.png");
    texIds.push_back("graphics/Characters/Pico_Player1_Idle_Final.png");
    texIds.push_back("graphics/Characters/Pico_Player1_Jump_Final.png");
    texIds.push_back("graphics/Characters/Pico_Player1_walk_right_final.png");
    texIds.push_back("graphics/Characters/player1_pushwalk.png");
    texIds.push_back("graphics/Characters/Pico_Player2_Idle_Final.png");
    texIds.push_back("graphics/Characters/Pico_Player2_Jump_Final.png");
    texIds.push_back("graphics/Characters/Pico_Player2_walk_right_final.png");
    texIds.push_back("graphics/Characters/player2_pushwalk.png");
    texIds.push_back("graphics/Characters/Pico_Player3_Idle_Final.png");
    texIds.push_back("graphics/Characters/Pico_Player3_Jump_Final.png");
    texIds.push_back("graphics/Characters/Pico_Player3_walk_right_final.png");
    texIds.push_back("graphics/Characters/player3_pushwalk.png");

    bgTex.loadFromFile("graphics/Background.png");
    bgSpr.setTexture(bgTex);
    bgSpr.setScale({ 1.3f, 1.3f });

    fontIds.push_back("fonts/DS-DIGIT.ttf");

    ANI_CLIP_MGR.Load("animations/Pico_Player_Idle_Final.csv");
    ANI_CLIP_MGR.Load("animations/Pico_Player_Jump_Final.csv");
    ANI_CLIP_MGR.Load("animations/Pico_Player_walk_right_final.csv");
    ANI_CLIP_MGR.Load("animations/player_pushwalk.csv");
    ANI_CLIP_MGR.Load("animations/Pico_Player1_Idle_Final.csv");
    ANI_CLIP_MGR.Load("animations/Pico_Player1_Jump_Final.csv");
    ANI_CLIP_MGR.Load("animations/Pico_Player1_walk_right_final.csv");
    ANI_CLIP_MGR.Load("animations/player1_pushwalk.csv");
    ANI_CLIP_MGR.Load("animations/Pico_Player2_Idle_Final.csv");
    ANI_CLIP_MGR.Load("animations/Pico_Player2_Jump_Final.csv");
    ANI_CLIP_MGR.Load("animations/Pico_Player2_walk_right_final.csv");
    ANI_CLIP_MGR.Load("animations/player2_pushwalk.csv");
    ANI_CLIP_MGR.Load("animations/Pico_Player3_Idle_Final.csv");
    ANI_CLIP_MGR.Load("animations/Pico_Player3_Jump_Final.csv");
    ANI_CLIP_MGR.Load("animations/Pico_Player3_walk_right_final.csv");
    ANI_CLIP_MGR.Load("animations/player3_pushwalk.csv");

    level = new Level();
    tileMap = new TileMap();

    Variables::tilemap = tileMap;

    clearFont.loadFromFile("fonts/Galmuri11-Bold.TTF");
    clearTxt.setFont(clearFont);
    clearTxt.setString("C L E A R !");
    clearTxt.setCharacterSize(20);
    clearTxt.setFillColor(sf::Color(255, 134, 77));
    clearTxt.setOutlineColor(sf::Color::White);
    clearTxt.setOutlineThickness(2.f);

    Scene::Init();
}

void SceneGame::StartStageClear()
{
    if (stageClear) return;              
    stageClear = true;
    clearTime = 0.f;

    sf::Vector2u winSize = FRAMEWORK.GetWindow().getSize();
    sf::FloatRect vp = worldView.getViewport();
    sf::Vector2f vpSize(winSize.x * vp.width, winSize.y * vp.height);
    //sf::FloatRect b = clearTxt.getLocalBounds();
    //clearTxt.setOrigin(b.left + b.width * 0.5f,
        //b.top + b.height * 0.5f);

    toPos = { vp.left * winSize.x + vpSize.x * 0.5f,
        vp.top * winSize.y + vpSize.y * 0.5f };
    fromPos = { toPos.x - worldView.getSize().x * 2.f,
               toPos.y };

    clearTxt.setPosition(fromPos);

    for (auto* p : Variables::players) p->SetActive(false);
}

void SceneGame::Enter()
{
    Scene::Enter();   
    std::string path = pendingStage.empty()
        ? "levels/stage_tmp.json"
        : std::move(pendingStage);
    pendingStage.clear();

    if (loadLevel_(path, *level)) {
        std::cout << "맵 로딩" << std::endl;
        std::cout << "엔티티 개수 : " << level->entities.size() << std::endl;
        tileMap->load(*level, 1);

        LoadStage(path);
    }

    //worldView.setSize(VIEW_W, VIEW_H);
    worldView.setSize(BASE_W, BASE_H);
    worldView.setCenter(BASE_W * 0.5f, BASE_H * 0.5f);
    //worldView.setSize(VIEW_W, level->gridHeight * level->tileSize);
    //worldView.setSize(level->gridWidth  * level->tileSize, level->gridHeight * level->tileSize);  
    //worldView.setCenter(worldView.getSize() / 2.f);


//float winRatio  = FRAMEWORK.GetWindow().getSize().x / (float)FRAMEWORK.GetWindow().getSize().y;
//float viewRatio = worldView.getSize().x / worldView.getSize().y;
//
//sf::FloatRect vp;
//if (viewRatio < winRatio) {             // �¿� ����
//    float pad = (1.f - viewRatio / winRatio) * 0.5f;
//    vp = { pad, 0.f, 1.f - pad * 2.f, 1.f };
//} else {                                // ���� ����
//    float pad = (1.f - winRatio / viewRatio) * 0.5f;
//    vp = { 0.f, pad, 1.f, 1.f - pad * 2.f };
//}
//worldView.setViewport(vp);

//FRAMEWORK.GetWindow().setView(worldView);
}

void SceneGame::Update(float dt)
{
	Scene::Update(dt);

    if (stageClear)            
    {
        constexpr float SLIDE_DUR = 1.0f;   // 텍스트 이동 1초
        constexpr float HOLD_DUR = 1.0f;   // 중앙에서 1초 정지
        clearTime += dt;

        if (clearTime < SLIDE_DUR)
        {   
            float t = clearTime / SLIDE_DUR;  
            clearTxt.setPosition(Utils::Lerp(fromPos, toPos, t));
        }
        else if (clearTime >= SLIDE_DUR + HOLD_DUR)
        {   
            SCENE_MGR.ChangeScene(SceneIds::Select);
            stageClear = false;
            return;
        }

        return;     
    }

    if (InputMgr::GetKeyDown(sf::Keyboard::Escape)) {
        SCENE_MGR.ChangeScene(SceneIds::Select);
    }

    if (!Variables::players.empty())        // 플레이어가 하나라도 있으면
        updateCamera(dt);

    if (InputMgr::GetKeyDown(sf::Keyboard::Escape) && isEditor) {
        //isEditor = false;
        SCENE_MGR.ChangeScene(SceneIds::Editor);
    }

    if (InputMgr::GetKeyDown(sf::Keyboard::F11))
    {
        for (Player* p : Variables::players)
        {
            p->OnDie();
        }
    }

    /*
     if (Variables::players[0] != nullptr)
    {
        sf::Vector2f playerPos = Variables::players[0]->GetPosition();

        // 경계 제한
        sf::Vector2f viewSize = worldView.getSize();
        sf::Vector2f halfSize = viewSize * 0.5f;
        sf::Vector2f mapSize = {
         (float)level->gridWidth * level->tileSize,
         (float)level->gridHeight * level->tileSize
        };

        playerPos.x = Utils::Clamp(playerPos.x, 0.f, mapSize.x);
        playerPos.y = Utils::Clamp(playerPos.y, 0.f, mapSize.y);

        int tileX = static_cast<int>(playerPos.x) / level->tileSize;
        int tileY = static_cast<int>(playerPos.y) / level->tileSize;

        if (tileMap->isSolid(tileX, tileY)) {
            //std::cout << "충돌" << std::endl;
            playerPos = Variables::players[0]->getPrvPos();
        }
        playerPos.x = Utils::Clamp(playerPos.x, 0.f, mapSize.x);
        playerPos.y = Utils::Clamp(playerPos.y, 20.f, mapSize.y);
        Variables::players[0]->SetPosition(playerPos);

    }
    */
    

    //Variables::players[0]->SetPosition({ 48.f, 64.f });
    //std::cout << Variables::players[0]->GetPosition().x << " / " << Variables::players[0]->GetPosition().y << std::endl;
}

void SceneGame::Draw(sf::RenderWindow& window)
{
    window.setView(worldView);
    window.draw(bgSpr);
    tileMap->Draw(window);
    Scene::Draw(window);
    if (stageClear)
    {
        sf::FloatRect vp = worldView.getViewport();   

        sf::View uiView = window.getDefaultView();
        uiView.setViewport(vp);

        window.setView(uiView);

        sf::Vector2u winSize = window.getSize();
        sf::Vector2f vpSize(winSize.x * vp.width, winSize.y * vp.height);
        clearTxt.setCharacterSize(std::round(vpSize.y * 0.15f)); // 비율로 글자크기
        sf::FloatRect b = clearTxt.getLocalBounds();
        clearTxt.setOrigin(b.left + b.width * 0.5f,
            b.top + b.height * 0.5f);
        /*clearTxt.setPosition(vp.left * winSize.x + vpSize.x * 0.5f,
            vp.top * winSize.y + vpSize.y * 0.5f);*/

        window.draw(clearTxt);
    }
    //auto activeView = FRAMEWORK.GetWindow().getView();
    //std::cout << "Active view size: "
    //    << activeView.getSize().x << ", "
    //    << activeView.getSize().y << '\n';
    //auto v = window.getView();
    //v.setCenter(std::round(v.getCenter().x),
    //    std::round(v.getCenter().y));
    //window.setView(v);


    /*
    for (auto& e : level->entities)
    {
        sf::RectangleShape box({ float(e.value("w",level->tileSize)),
                                float(e.value("h",level->tileSize)) });
        box.setPosition(float(e["x"]), float(e["y"]));
        box.setFillColor(sf::Color::Transparent);
        box.setOutlineThickness(-1);
        box.setOutlineColor((e["type"] == "Door" &&
            e["properties"].value("locked", false))
            ? sf::Color::Red : sf::Color::Yellow);
        window.draw(box);
    }
    */
    

    /*
    const int ts = level->tileSize;      // 16
    const int gw = level->gridWidth;
    const int gh = level->gridHeight;

    sf::RectangleShape rect(sf::Vector2f(16.f, 16.f)); 

    const auto& data = level->tiles;       // vector<int> ��ü Ÿ��

    for (int y = 0; y < gh; ++y)
    {
        for (int x = 0; x < gw; ++x)
        {
            int idx = y * gw + x;
            int tileId = data[idx];

            if (tileId == 0) continue;   

            rect.setPosition(float(x * ts), float(y * ts));
            rect.setFillColor(makeColor(tileId));
            window.draw(rect);
        }
    }


    for (auto& e : level->entities)
    {
        auto boxW = e.value("w", ts);
        auto boxH = e.value("h", ts);
        sf::RectangleShape box(sf::Vector2f(boxW, boxH));
        box.setPosition(float(e["x"]), float(e["y"]));
        box.setFillColor(sf::Color::Transparent);
        box.setOutlineThickness(-1);
        box.setOutlineColor(sf::Color::Yellow);

        if (e["type"] == "Door") {
            bool locked = e["properties"].value("locked", false);
            box.setOutlineColor(locked ? sf::Color::Red     // ���-��
                : sf::Color::Green); // ����-��
        }
        window.draw(box);
    }
    */


    
}
