#include "stdafx.h"
#include "SceneGame.h"
#include "Player.h"
#include "Gimmick.h"

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
    return palette[(tileId - 1) % palette.size()];
}

void SceneGame::LoadStage(const std::string& jsonPath)
{
    Variables::ResetStage();

    std::ifstream fin(jsonPath);
    nlohmann::json j;
    fin >> j;

    for (const auto& entobj : j["entities"])
    {
        Gimmick* g = Gimmick::CreateFromJson(entobj);
        g->Init();
        AddGameObject(g);
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
    texIds.push_back("graphics/Item/key.png");
    texIds.push_back("graphics/Item/door.png");
    texIds.push_back("graphics/Item/doorOpen.png");
    texIds.push_back("graphics/Item/Button.png");
    texIds.push_back("graphics/Item/WeightBlock.png");

    fontIds.push_back("fonts/DS-DIGIT.ttf");

    level = new Level();
	if (loadLevel_("levels/stage00.json", *level)) {
		std::cout << "�� �ε� �Ϸ�" << std::endl;
		std::cout << "��ƼƼ ���� : " << level->entities.size() << std::endl;
        tileMap.load(*level, 1);

        LoadStage("levels/stage00.json");
	}

    AddGameObject(new Player());

    Scene::Init();
}

void SceneGame::Enter()
{
Scene::Enter();
worldView.setSize(level->gridWidth  * level->tileSize,   
                  level->gridHeight * level->tileSize);  
worldView.setCenter(worldView.getSize() / 2.f);

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
}

void SceneGame::Draw(sf::RenderWindow& window)
{
    Scene::Draw(window);
    //auto activeView = FRAMEWORK.GetWindow().getView();   // �� **�׸��� ���� ȣ��**
    //std::cout << "Active view size: "
    //    << activeView.getSize().x << ", "
    //    << activeView.getSize().y << '\n';
    //auto v = window.getView();
    //v.setCenter(std::round(v.getCenter().x),
    //    std::round(v.getCenter().y));
    //window.setView(v);

    tileMap.Draw(window);

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
