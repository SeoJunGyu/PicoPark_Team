#include "stdafx.h"
#include "SceneGame.h"
#include "Player.h"
#include "Gimmick.h"
#include "PrefabMgr.h"

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


        if (Gimmick* g = Gimmick::CreateFromJson(entobj))
        {
            g->Init();
            g->Reset();
            Variables::gimmicks.push_back(g);
            AddGameObject(g);
            if (tstr == "MovingPlatform")
            {
                Variables::platforms.push_back(dynamic_cast<MovingPlatform*>(g));
            }
        }

        if (tstr == "PlayerSpawn")
        {
            int tmp = entobj["properties"].value("playerIndex", 0);
            float x = entobj.at("x").get<float>();
            float y = entobj.at("y").get<float>();
            spawnPoints.emplace_back(x, y);
            continue;          // Gimmick 생성 생략
        }

        GameObject* g = PrefabMgr::I().Instantiate(
            entobj["type"],
            { entobj["x"], entobj["y"] },
            entobj.value("properties", nlohmann::json::object()));
        if (g) {
            g->Init();
            g->Reset();
            Variables::gimmicks.push_back((Gimmick*)g);
            AddGameObject(g);
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
        p->SetPosition(pos);
        p->SetScale({ 0.1f, 0.1f });
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
}

void SceneGame::updateCamera(float dt)
{
    sf::FloatRect playersBB;            // 모든 플레이어의 AABB
    bool first = true;
    for (auto* p : Variables::players) {
        sf::Vector2f pos = p->GetPosition();
        if (first) {
            playersBB = { pos.x, pos.y, 0, 0 };
            first = false;
        }
        else {
            playersBB.left = std::min(playersBB.left, pos.x);
            playersBB.top = std::min(playersBB.top, pos.y);
            playersBB.width = std::max(playersBB.width, pos.x - playersBB.left);
            playersBB.height = std::max(playersBB.height, pos.y - playersBB.top);
        }
    }

    bool fits =
        playersBB.width <= VIEW_W * 0.9f &&   // 10% 여유
        playersBB.height <= VIEW_H * 0.9f;

    if (fits) {
        sf::Vector2f targetCenter(
            playersBB.left + playersBB.width * 0.5f,
            playersBB.top + playersBB.height * 0.5f);

        // 부드러운 이동(옵션) : LERP
        sf::Vector2f cur = worldView.getCenter();
        float   smooth = 5.f;                 // 값이 클수록 빠르게 따라감
        targetCenter = cur + (targetCenter - cur) * dt * smooth;

        // 맵 경계 밖으로 나가지 않도록 클램프
        sf::Vector2f half = worldView.getSize() * 0.5f;
        float mapW = level->gridWidth * level->tileSize;
        float mapH = level->gridHeight * level->tileSize;
        targetCenter.x = Utils::Clamp(targetCenter.x, half.x, mapW - half.x);
        targetCenter.y = Utils::Clamp(targetCenter.y, half.y, mapH - half.y);

        worldView.setCenter(targetCenter);
    }
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
    texIds.push_back("graphics/Item/key.png");
    texIds.push_back("graphics/Item/door.png");
    texIds.push_back("graphics/Item/doorOpen.png");
    texIds.push_back("graphics/Item/Button.png");
    texIds.push_back("graphics/Item/Button_Pressed.png");
    texIds.push_back("graphics/Item/Pad.png");
    texIds.push_back("graphics/Item/jump1.png");
    texIds.push_back("graphics/Item/jump3.png");
    texIds.push_back("graphics/Item/FallingPlatform.png");
    texIds.push_back("graphics/Item/elevator.png");
    texIds.push_back("graphics/Item/WeightBlock.png");
    bgTex.loadFromFile("graphics/Background.png");
    bgSpr.setTexture(bgTex);
    bgSpr.setScale({ 1.3f, 1.3f });

    fontIds.push_back("fonts/DS-DIGIT.ttf");

    level = new Level();
    tileMap = new TileMap();

    Variables::tilemap = tileMap;

    Scene::Init();
}

void SceneGame::Enter()
{
    Scene::Enter();
    if (loadLevel_("levels/stageTest.json", *level)) {
        std::cout << "맵 로딩" << std::endl;
        std::cout << "엔티티 개수 : " << level->entities.size() << std::endl;
        tileMap->load(*level, 1);

        LoadStage("levels/stageTest.json");
    }

    worldView.setSize(VIEW_W, VIEW_H);

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

    if (!Variables::players.empty())        // 플레이어가 하나라도 있으면
        updateCamera(dt);

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
    window.draw(bgSpr);
    tileMap->Draw(window);
    Scene::Draw(window);
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
