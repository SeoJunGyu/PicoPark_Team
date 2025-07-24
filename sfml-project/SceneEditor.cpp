#include "stdafx.h"
#include "SceneEditor.h"
#include "JsonSerializer.hpp"

SceneEditor::SceneEditor()
	: Scene(SceneIds::Editor) 
{
}

void SceneEditor::Brush(LevelGrid& grid, int tileId, sf::Vector2i mouse, sf::RenderWindow& win)
{
    sf::Vector2f wp = win.mapPixelToCoords(mouse, worldView);
    int tx = int(wp.x) / grid.tileSize;
    int ty = int(wp.y) / grid.tileSize;

    if (tx < 0 || tx >= grid.width || ty < 0 || ty >= grid.height) return;

    grid.tiles[ty * grid.width + tx] = tileId;
}

void SceneEditor::Init()
{
    fontIds.push_back("fonts/DS-DIGIT.ttf");
	Scene::Init();
}

void SceneEditor::Enter()
{
	Scene::Enter(); 

    palette.Load({ "graphics/Floor.png",
                   "graphics/Spikes.png",
                   "graphics/Door_Button.png",
                   "graphics/Button.png" }, 16);

    constexpr float UI_WIDTH = 1000.f;                // 팔레트 폭
    const auto winSize = FRAMEWORK.GetWindow().getSize();
    const float uiRatio = UI_WIDTH / winSize.x;
    uiStartX = float(winSize.x) - UI_WIDTH;

    worldView.setViewport({ 0.f, 0.f, 1.f - uiRatio, 1.f });
    uiView.setViewport({ 1.f - uiRatio, 0.f, uiRatio, 1.f });

    grid = LevelGrid(10, 6, 16);                     

    worldView.setSize(grid.width * grid.tileSize,
        grid.height * grid.tileSize);
    worldView.setCenter(worldView.getSize() * 0.5f);

    currentTile = 1;       // 첫 색 선택
    currentEntity = "";

    //int tileId = palette.GetSelected();

    //grid.width = 10;  grid.height = 6; grid.tileSize = 16;
    //grid.tiles.assign(grid.width * grid.height, 0);

    //constexpr float UI_WIDTH = 1000.f;            
    //const sf::Vector2u winSize = FRAMEWORK.GetWindow().getSize();
    //float uiRatio = UI_WIDTH / winSize.x;
    //uiStartX = float(winSize.x) - UI_WIDTH;
    //level.gridWidth = 10;
    //level.gridHeight = 6;
    //level.tileSize = 16;


    //worldView.setSize(level.gridWidth * level.tileSize,   // 160
    //    level.gridHeight * level.tileSize);  // 96
    //worldView.setCenter(worldView.getSize() * 0.5f);

    //worldView.setViewport({ 0.f, 0.f, 1.f - uiRatio, 1.f });
    //uiView.setViewport({ 1.f - uiRatio, 0.f, uiRatio, 1.f });


    //level.layers.clear();
    //level.layers.push_back({                    
    //    "Main", Layer::Kind::TILE, true, 1.f,
    //    std::vector<int>(level.gridWidth * level.gridHeight, 0)  
    //    });
}

void SceneEditor::Update(float dt)
{
	Scene::Update(dt);
    if (InputMgr::GetMouseButton(sf::Mouse::Right)) {
        palette.ClearSelected();
        Brush(grid, 0, InputMgr::GetMousePosition(), FRAMEWORK.GetWindow());
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i mp = sf::Mouse::getPosition(FRAMEWORK.GetWindow());
        if (mp.x >= uiStartX) {           
            int localX = mp.x - int(uiStartX);
            int localY = mp.y;

            int col = localX / 36, row = localY / 36;
            if (row < 2 && col < 5) {
                int id = row * 5 + col + 1;     
                palette.ClearSelected();
                currentTile = id;
                currentEntity = "";             
            }

            if (localY >= 200 && localY < 320 && localX < 32) {
                int idx = (localY - 200) / 40;   // 0,1,2
                static const char* n[] = { "Key","Door","PlayerSpawn" };
                currentEntity = n[idx];
            }
        }
        else {
            int imguiSel = palette.GetSelected();     
            if (imguiSel > 0)              
                currentTile = 100 + imguiSel;
            // 월드 캔버스
            sf::Vector2f wp = FRAMEWORK.GetWindow().mapPixelToCoords(mp, worldView);
            int tx = int(wp.x) / grid.tileSize;
            int ty = int(wp.y) / grid.tileSize;

            if (!currentEntity.empty()) {              
                level.entities.push_back({
                        ++nextId,                       
                        StrToType(currentEntity),        
                        tx * grid.tileSize,             
                        ty * grid.tileSize,             
                        16, 16,                          
                        {}                               
                });
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                Brush(grid, currentTile, InputMgr::GetMousePosition(), FRAMEWORK.GetWindow());
            }
        }
    }

    // Ctrl+S : 저장
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        std::cout << "맵 저장 완료." << std::endl;
        SaveAsLevel("levels/stage_tmp.json");
    }

    // F5 : 바로 플레이
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F5)) {
        SCENE_MGR.ChangeScene(SceneIds::Game);
        //SaveTempAndSwitchToGame();  
    }

    // ←↑↓→ : worldView 이동
    float pan = 300 * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  worldView.move(-pan, 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))  worldView.move(pan, 0);
}

void SceneEditor::SaveAsLevel(const std::string& path)
{
    Level lv;
    lv.tileSize = grid.tileSize;
    lv.gridWidth = grid.width;
    lv.gridHeight = grid.height;
    lv.layers.push_back({ "Main",
                          Layer::Kind::TILE,
                          true, 1.f,
                          grid.tiles });      
    std::ofstream(path) << nlohmann::json(lv).dump(2);
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
    //칠해진거 그리는거
    sf::RectangleShape colorTile({ 16,16 });
    sf::Sprite spr;

    for (int y = 0; y < grid.height; ++y)
        for (int x = 0; x < grid.width; ++x)
        {
            int id = grid.tiles[y * grid.width + x];
            if (id == 0) continue;

            if (id <= 9)                                // 1~9 : 단색 타일
            {
                colorTile.setFillColor(Palette[id]);
                colorTile.setPosition(x * 16.f, y * 16.f);
                w.draw(colorTile);
            }
            else                                        // 100+ : 텍스처 타일
            {
                int texIdx = id - 100;                  // 0‑based
                if (texIdx >= palette.GetTileSet().textures.size()) continue;

                spr.setTexture(palette.GetTileSet().textures[texIdx]);
                float s = float(grid.tileSize) /
                    palette.GetTileSet().textures[texIdx].getSize().x;
                spr.setScale(s, s);
                spr.setPosition(x * grid.tileSize, y * grid.tileSize);
                w.draw(spr);
            }
        }

    //그리드 선
    sf::VertexArray gridLines(sf::Lines);
    for (int x = 0; x <= grid.width; ++x) {
        gridLines.append({ {x * 16.f, 0.f             }, sf::Color(180,180,180) });
        gridLines.append({ {x * 16.f, grid.height * 16.f}, sf::Color(180,180,180) });
    }
    for (int y = 0; y <= grid.height; ++y) {
        gridLines.append({ {0.f,             y * 16.f}, sf::Color(180,180,180) });
        gridLines.append({ {grid.width * 16.f, y * 16.f}, sf::Color(180,180,180) });
    }
    w.draw(gridLines);


    w.setView(uiView);
    palette.DrawImGui();
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
