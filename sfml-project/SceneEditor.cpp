#include "stdafx.h"
#include "SceneEditor.h"
#include "GimmickTypeConv.hpp"

SceneEditor::SceneEditor()
	: Scene(SceneIds::Editor) 
{
}

void SceneEditor::Init()
{
    fontIds.push_back("fonts/DS-DIGIT.ttf");
	Scene::Init();
}

void SceneEditor::Enter()
{
	Scene::Enter(); 
    //constexpr float UI_WIDTH = 1000.f;   
    constexpr float UI_WIDTH = 1000.f;            
    const sf::Vector2u winSize = FRAMEWORK.GetWindow().getSize();
    float uiRatio = UI_WIDTH / winSize.x;
    uiStartX = float(winSize.x) - UI_WIDTH;
    level.gridWidth = 10;
    level.gridHeight = 6;
    level.tileSize = 16;


    worldView.setSize(level.gridWidth * level.tileSize,   // 160
        level.gridHeight * level.tileSize);  // 96
    worldView.setCenter(worldView.getSize() * 0.5f);

    worldView.setViewport({ 0.f, 0.f, 1.f - uiRatio, 1.f });
    uiView.setViewport({ 1.f - uiRatio, 0.f, uiRatio, 1.f });


    //worldView.setSize(winSize.x * (1 - uiRatio), winSize.y - 200);
    //worldView.setCenter(worldView.getSize() * 0.5f);

    //worldView.setSize(level.gridWidth * level.tileSize, level.gridHeight * level.tileSize);
    //worldView.setSize(800, 800);
    //worldView.setCenter(worldView.getSize() * 0.5f);

    //uiView.setViewport({ 1.f - uiRatio, 0.f, uiRatio, 1.f });
    //worldView.setViewport({ 0,0, 1.f - uiRatio, 1.f });

    level.layers.clear();
    level.layers.push_back({                    
        "Main", Layer::Kind::TILE, true, 1.f,
        std::vector<int>(level.gridWidth * level.gridHeight, 0)  
        });
}

void SceneEditor::Update(float dt)
{
	Scene::Update(dt);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i mp = sf::Mouse::getPosition(FRAMEWORK.GetWindow());
        if (mp.x >= uiStartX) {           // UI 영역
            int localX = mp.x - int(uiStartX);
            int localY = mp.y;

            // 팔레트 5×2 슬롯 영역 (여기선 1~9까지만)
            int col = localX / 36, row = localY / 36;
            if (row < 2 && col < 5) {
                int id = row * 5 + col + 1;      // 1~10
                currentTile = id;
                currentEntity = "";              // 타일 모드
            }

            if (localY >= 200 && localY < 320 && localX < 32) {
                int idx = (localY - 200) / 40;   // 0,1,2
                static const char* n[] = { "Key","Door","PlayerSpawn" };
                currentEntity = n[idx];
            }
        }
        else {                      // 월드 캔버스
            sf::Vector2f wp = FRAMEWORK.GetWindow().mapPixelToCoords(mp, worldView);
            int tx = int(wp.x) / level.tileSize;
            int ty = int(wp.y) / level.tileSize;

            if (!currentEntity.empty()) {              // 엔티티 배치
                level.entities.push_back({
                        ++nextId,                        // id
                        StrToType(currentEntity),        // type
                        tx * level.tileSize,             // x  ← int 로 맞춤
                        ty * level.tileSize,             // y
                        16, 16,                          // w, h (기본값이면 생략 가능)
                        {}                               // properties (빈 객체)
                });
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if (tx >= 0 && tx < level.gridWidth && ty >= 0 && ty < level.gridHeight)
                {
                    if (currentEntity.empty()) {
                        level.layers[0].data[ty * level.gridWidth + tx] = currentTile;   // ★ 타일 색칠
                    }
                    /* 엔티티는 기존 코드 유지 */
                }
            }
        }
    }

    // Ctrl+S : 저장
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        //std::ofstream("levels/stage_tmp.json") << nlohmann::json(level).dump(2);
    }

    // F5 : 바로 플레이
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F5)) {
        //SaveTempAndSwitchToGame();   // 아래 6번
    }

    // ←↑↓→ : worldView 이동
    float pan = 300 * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  worldView.move(-pan, 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))  worldView.move(pan, 0);
}

void SceneEditor::Draw(sf::RenderWindow& w) {
    w.setView(w.getDefaultView());
    w.clear(sf::Color(40, 40, 40));


    w.setView(worldView);

    sf::Vector2f worldSize = worldView.getSize();
    sf::Vector2f worldPos = worldView.getCenter() - worldSize * 0.5f;

    sf::RectangleShape worldBG(worldSize);
    worldBG.setPosition(worldPos);
    worldBG.setFillColor(sf::Color::White);
    w.draw(worldBG);

    sf::RectangleShape border(worldSize);
    border.setPosition(worldPos);
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineThickness(2.f);
    border.setOutlineColor(sf::Color::Black);
    w.draw(border);
    /* 4‑1. 이미 칠해진 타일 그리기 */
    sf::RectangleShape tileRect({ 16,16 });
    for (int y = 0; y < level.gridHeight; ++y)
        for (int x = 0; x < level.gridWidth; ++x) {
            int id = level.layers[0].data[y * level.gridWidth + x];
            if (id == 0) continue;
            tileRect.setPosition(x * 16.f, y * 16.f);
            tileRect.setFillColor(Palette[id]);
            w.draw(tileRect);
        }

    /* 4‑2. 그리드 선 그리기 */
    sf::VertexArray gridLines(sf::Lines);
    for (int x = 0; x <= level.gridWidth; ++x) {
        gridLines.append({ {x * 16.f, 0.f             }, sf::Color(180,180,180) });
        gridLines.append({ {x * 16.f, level.gridHeight * 16.f}, sf::Color(180,180,180) });
    }
    for (int y = 0; y <= level.gridHeight; ++y) {
        gridLines.append({ {0.f,             y * 16.f}, sf::Color(180,180,180) });
        gridLines.append({ {level.gridWidth * 16.f, y * 16.f}, sf::Color(180,180,180) });
    }
    w.draw(gridLines);


    w.setView(uiView);
    DrawPaletteAndButtons(w); 
}

void SceneEditor::DrawPaletteAndButtons(sf::RenderWindow& w)
{
    const float margin = 8.f;
    const float slotGap = 36.f;

    sf::RectangleShape slot({ 32,32 });
    slot.setFillColor(sf::Color(50, 50, 50));
    slot.setOutlineThickness(1);
    slot.setOutlineColor(sf::Color::White);

    /* --- 팔레트 색칠 --- */
    for (std::size_t i = 1; i < Palette.size(); ++i) {        // 1~9
        int row = int(i - 1) / 5;
        int col = int(i - 1) % 5;
        slot.setPosition(margin + col * slotGap, margin + row * slotGap);
        w.draw(slot);

        sf::RectangleShape prev({ 24,24 });
        prev.setFillColor(Palette[i]);
        prev.setPosition(slot.getPosition() + sf::Vector2f(4, 4));
        w.draw(prev);
    }

    /* --- 엔티티 버튼 --- */
    static const char* names[] = { "Key","Door","PlayerSpawn" };
    for (int i = 0; i < 3; ++i) {
        slot.setPosition(margin, 200 + i * 40);
        w.draw(slot);
        sf::Text txt(names[i],
            FONT_MGR.Get("fonts/DS-DIGIT.ttf"), 14);
        txt.setPosition(slot.getPosition() + sf::Vector2f(4, 6));
        w.draw(txt);
    }
}
