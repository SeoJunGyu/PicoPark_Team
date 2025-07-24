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

    int idx = ty * grid.width + tx;

    if (currentEntity == "PlayerSpawn" && currentPlayerIndex > 3) {
        return;
    }
    if (currentEntity == "PlayerSpawn" && grid.entitiesType[idx] != "PlayerSpawn" && currentPlayerIndex < 4) {
        ++currentPlayerIndex;
        std::cout << "플레이어 수 : " << currentPlayerIndex << std::endl;
    }

    if (tileId == 0) {                 
        if (grid.entitiesType[idx] == "PlayerSpawn") {
            currentPlayerIndex--;
            std::cout << "남은 플레이어 수 : " << currentPlayerIndex << std::endl;
        }
        grid.tiles[idx] = 0;
        grid.entities[idx] = 0;
        grid.entitiesType[idx].clear();
    }
    else if (tileId <= 9) {           
        if (grid.entitiesType[idx] == "PlayerSpawn") {
            currentPlayerIndex--;
            std::cout << "남은 플레이어 수 : " << currentPlayerIndex << std::endl;
        }
        grid.tiles[idx] = tileId;
        grid.entities[idx] = 0;
        grid.entitiesType[idx].clear();
    }
    else {                                           
        grid.tiles[idx] = 0;
        grid.entities[idx] = tileId;
        grid.entitiesType[idx] = currentEntity;
    }
}

void SceneEditor::Init()
{
    fontIds.push_back("fonts/DS-DIGIT.ttf");
	Scene::Init();
}

void SceneEditor::Enter()
{
	Scene::Enter(); 

    palette.Load({ "graphics/Characters/Icon/Player0.png",
                   "graphics/Item/key.png",
                   "graphics/Item/door.png"}, 16);

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
        currentEntity = "";
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
           /* int imguiSel = palette.GetSelected();     
            if (imguiSel > 0)              
                currentTile = 100 + imguiSel;*/
            // 월드 캔버스
            sf::Vector2f wp = FRAMEWORK.GetWindow().mapPixelToCoords(mp, worldView);
            int tx = int(wp.x) / grid.tileSize;
            int ty = int(wp.y) / grid.tileSize;

            if (palette.GetSelected() != -1) {
                static const char* n[] = { "PlayerSpawn","Key","Door" };
                currentEntity = n[palette.GetSelected()];
                //std::cout << "선택 : " << currentEntity << std::endl;
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if (!currentEntity.empty() && grid.entitiesType[ty * grid.width + tx] != currentEntity) {
                    Brush(grid, palette.GetSelected() + 100, InputMgr::GetMousePosition(), FRAMEWORK.GetWindow());
                }
                if (palette.GetSelected() == -1) Brush(grid, currentTile, InputMgr::GetMousePosition(), FRAMEWORK.GetWindow());
            }
        }
    }

    // Ctrl+S : 저장
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && InputMgr::GetKeyDown(sf::Keyboard::S)){
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
    //Level lv;
    level.tileSize = grid.tileSize;
    level.gridWidth = grid.width;
    level.gridHeight = grid.height;
    level.layers.clear();
    level.layers.push_back({ "Main",
                          Layer::Kind::TILE,
                          true, 1.f,
                          grid.tiles });   
    level.entities.clear();
    for (int y = 0; y < grid.height; ++y)
        for (int x = 0; x < grid.width; ++x)
        {
            int eId = grid.entities[y * grid.width + x];
            if (eId < 100) continue;         

            std::string type = grid.entitiesType[y * grid.width + x];
            if (type.empty()) continue;        

            float sx = 1.f, sy = 1.f;
            int ww = 16, hh = 16;
            std::unordered_map<std::string, int> props;
            if (type == "PlayerSpawn") {
                ww = 60, hh = 128; 
                props["playerIndex"] = pIndex;
                pIndex++;
            }
            if (type == "Door") { sx = 0.1f; sy = 0.1f; }
            if (type == "Key") { sx = 0.1f; sy = 0.1f; }

            level.entities.push_back({
                nextId++,                      // id
                StrToType(type),               // type
                x * grid.tileSize,             // x
                y * grid.tileSize,             // y
                ww, hh,                        // w, h
                { sx, sy },                    // scale
                props                             // properties
                });
        }
    std::ofstream(path) << nlohmann::json(level).dump(2);
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
            int Entityid = grid.entities[y * grid.width + x];
            if (id == 0 && Entityid == 0) continue;

            if (id <= 9)                                // 1~9 : 단색 타일
            {
                colorTile.setFillColor(Palette[id]);
                colorTile.setPosition(x * 16.f, y * 16.f);
                w.draw(colorTile);
            }
            if (Entityid >= 100)                                     // 100+ : 텍스처 타일
            {
                int texIdx = Entityid - 100;                  // 0‑based
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

    ///* --- 엔티티 버튼 --- */
    //static const char* names[] = { "Key","Door","PlayerSpawn" };
    //for (int i = 0; i < 3; ++i) {
    //    slot.setPosition(margin, 200 + i * 40);
    //    w.draw(slot);
    //    sf::Text txt(names[i],
    //        FONT_MGR.Get("fonts/DS-DIGIT.ttf"), 14);
    //    txt.setPosition(slot.getPosition() + sf::Vector2f(4, 6));
    //    w.draw(txt);
    //}
}
