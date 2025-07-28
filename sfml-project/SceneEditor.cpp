#include "stdafx.h"
#include "SceneEditor.h"
#include "JsonSerializer.hpp"
#include "PrefabMgr.h"
#include "SceneGame.h"

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
        grid.entitiesScale[idx] = { 0.f, 0.f };
        grid.entitiesType[idx].clear();
        grid.entitiesProps[idx].clear();
    }
    else if (tileId <= 9) {           
        if (grid.entitiesType[idx] == "PlayerSpawn") {
            currentPlayerIndex--;
            std::cout << "남은 플레이어 수 : " << currentPlayerIndex << std::endl;
        }
        grid.tiles[idx] = tileId;
        grid.entities[idx] = 0;
        grid.entitiesScale[idx] = { 0.f, 0.f };
        grid.entitiesType[idx].clear();
        grid.entitiesProps[idx].clear();
    }
    else {                                           
        grid.tiles[idx] = 0;
        grid.entities[idx] = tileId;
        int pid = palette.GetSelected();
        grid.entitiesScale[idx] = penScale[pid];
        grid.entitiesType[idx] = currentEntity;
        grid.entitiesProps[idx] = pendingProps;
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

    if (SceneGame::GetEditor()) {
        SceneGame::SetEditor(false);
        return;
    }

    pendingProps = nlohmann::json::object();
    std::vector<std::string> texFiles;
    prefabNames.clear();                     // 같은 인덱스로 이름 저장
    texArr.clear();
    penScale.clear();
    for (auto it = PrefabMgr::I().Table().begin(); it != PrefabMgr::I().Table().end(); ++it)
    {
        texFiles.push_back(it->second.sprite);
        sf::Texture t;
        t.loadFromFile(it->second.sprite);
        texArr[it->second.sprite] = t;
        prefabNames.push_back(it->first);
    }
    palette.Load(texFiles, 16);

    penScale.assign(prefabNames.size(), { -1.f, -1.f });

 /*   palette.Load({ "graphics/Characters/Icon/Player0.png",
                   "graphics/Item/key.png",
                   "graphics/Item/door.png"}, 16);*/

    constexpr float UI_WIDTH = 500.f;                // 팔레트 폭
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
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
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

            //if (localY >= 200 && localY < 320 && localX < 32) {
            //    int idx = (localY - 200) / 40;   // 0,1,2
            //    static const char* n[] = { "Key","Door","PlayerSpawn" };
            //    currentEntity = n[idx];
            //}
        }
        else {
            // 월드 캔버스
            sf::Vector2f wp = FRAMEWORK.GetWindow().mapPixelToCoords(mp, worldView);
            int tx = int(wp.x) / grid.tileSize;
            int ty = int(wp.y) / grid.tileSize;

            if (palette.GetSelected() != -1) {
                //static const char* n[] = { "PlayerSpawn","Key","Door" };
                //currentEntity = n[palette.GetSelected()];
                int imguiSel = palette.GetSelected();     
                currentEntity = (imguiSel >= 0) ? prefabNames[imguiSel] : "";

                //std::cout << "선택 : " << currentEntity << std::endl;
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if (!ImGui::GetIO().WantCaptureMouse)
                {
                    Brush(grid,
                        (palette.GetSelected() == -1 ? currentTile : palette.GetSelected() + 100),
                        InputMgr::GetMousePosition(),
                        FRAMEWORK.GetWindow());
                }
                /*if (!currentEntity.empty() && grid.entitiesType[ty * grid.width + tx] != currentEntity) {
                    Brush(grid, palette.GetSelected() + 100, InputMgr::GetMousePosition(), FRAMEWORK.GetWindow());
                }
                if (palette.GetSelected() == -1) Brush(grid, currentTile, InputMgr::GetMousePosition(), FRAMEWORK.GetWindow());*/
            }
        }
    }

    // Ctrl+S : 저장
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && InputMgr::GetKeyDown(sf::Keyboard::S)){
        std::cout << "맵 저장 완료." << std::endl;
        SaveAsLevel("levels/" + std::string(lvlName) + ".json");
    }

    if (InputMgr::GetKeyDown(sf::Keyboard::Escape)) {
        SCENE_MGR.ChangeScene(SceneIds::Title);
    }

    // F5 : 바로 플레이
    if (InputMgr::GetKeyDown(sf::Keyboard::F5)) {
        std::string file = "levels/" + std::string(lvlName) + ".json";
        SceneGame::SetPendingStage(file);
        SceneGame::SetEditor(true);
        SCENE_MGR.ChangeScene(SceneIds::Game); 
    }

    // ←↑↓→ : worldView 이동
    /*float pan = 300 * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  worldView.move(-pan, 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))  worldView.move(pan, 0);*/

    //마우스 관련 조작
    static sf::Vector2i lastPix{ -1, -1 };

    bool dragging = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
        sf::Mouse::isButtonPressed(sf::Mouse::Left);

    if (dragging)
    {
        sf::Vector2i pix = InputMgr::GetMousePosition();
        if (lastPix.x != -1)
        {
            sf::Vector2f delta = worldView.getCenter() -
                FRAMEWORK.GetWindow().mapPixelToCoords(lastPix, worldView);
            sf::Vector2f deltaNow = worldView.getCenter() -
                FRAMEWORK.GetWindow().mapPixelToCoords(pix, worldView);
            worldView.move(deltaNow - delta);
        }
        lastPix = pix;                     
    }
    else
    {
        lastPix = { -1, -1 };               
    }

    int d = InputMgr::PopWheelDelta();
    if (d != 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
    {
        const float STEP = 1.25f;            
        const float MIN_ZOOM = 0.2f;
        const float MAX_ZOOM = 5.f;

        float factor = (d > 0) ? 1.f / STEP : STEP;
        factor = std::pow(factor, std::abs(d));

        sf::Vector2i pix = InputMgr::GetMousePosition();
        sf::Vector2f before = FRAMEWORK.GetWindow().mapPixelToCoords(pix, worldView);

        float cur = worldView.getSize().x;        
        float next = cur * factor;
        if (next < grid.width * grid.tileSize * MIN_ZOOM) factor = 1.f;
        if (next > grid.width * grid.tileSize * MAX_ZOOM) factor = 1.f;


        worldView.zoom(factor);

        sf::Vector2f after = FRAMEWORK.GetWindow().mapPixelToCoords(pix, worldView);
        worldView.move(before - after);
    }

}

void SceneEditor::SaveAsLevel(const std::string& path)
{
    //Level lv;
    level.tileSize = grid.tileSize;
    level.gridWidth = grid.width;
    level.gridHeight = grid.height;
    level.author = author;
    level.description = desc;
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
            const PrefabAsset* PreA = PrefabMgr::I().Get(type);
            if (!PreA) continue;

            nlohmann::json ent;
            nlohmann::json props = grid.entitiesProps[y * grid.width + x];
            //nlohmann::json props = PreA->defaultProps;

        /*  auto ovIt = prefabOverrides.find(type);
            if (ovIt != prefabOverrides.end())
            {
                for (auto& kv : ovIt->second.items())   
                {
                    const std::string& key = kv.key();  
                    const nlohmann::json& val = kv.value(); 
                    props[key] = val;                    
                }
            }*/

            int dx = 0;
            int dy = 0;

            if (props.contains("path") &&
                props["path"].is_array() && props["path"].size() == 2 &&
                props["path"][0].is_array() && props["path"][1].is_array() &&
                props["path"][0].size() == 2 && props["path"][1].size() == 2)
            {
                const auto& p0 = props["path"][0];
                const auto& p1 = props["path"][1];

                dx = p1[0].get<int>() - p0[0].get<int>();
                dy = p1[1].get<int>() - p0[1].get<int>();
            }

            std::vector<sf::Vector2i> editPath;
            if (type == "MovingPlatform") {
                editPath.push_back({ x * grid.tileSize, y * grid.tileSize });
                editPath.push_back({ x * grid.tileSize + dx, y * grid.tileSize + dy});
                nlohmann::json jPath = nlohmann::json::array();
                for (const auto& p : editPath)
                    jPath.push_back({ p.x, p.y });        // 두 원소짜리 작은 배열

                props["path"] = jPath;
            }

            sf::Vector2f sc = PreA->scale;                       // 기본값
            auto scIt = grid.entitiesScale[y * grid.width + x];
            if (scIt.x != 0.f) sc = scIt;

            ent["id"] = nextId++;
            ent["type"] = type;
            ent["x"] = x * grid.tileSize;
            ent["y"] = y * grid.tileSize;
            ent["scale"] = sc;
            ent["properties"] = props;

            level.entities.push_back(ent);

            /*std::unordered_map<std::string, int> props;
            if (type == "PlayerSpawn") {
                ww = 60, hh = 128; 
                props["playerIndex"] = pIndex;
                pIndex++;
            }
            if (type == "Door") { sx = 0.1f; sy = 0.1f; }
            if (type == "Key") { sx = 0.1f; sy = 0.1f; }*/

            //level.entities.push_back({
            //    nextId++,                      // id
            //    StrToType(type),               // type
            //    x * grid.tileSize,             // x
            //    y * grid.tileSize,             // y
            //    ww, hh,                        // w, h
            //    { sx, sy },                    // scale
            //    props                             // properties
            //    });
        }
    std::ofstream(path) << nlohmann::json(level).dump(2);
}

void SceneEditor::ResizeGrid(int w, int h)
{
    std::vector<int> oldTiles = grid.tiles;
    std::vector<int> oldEntities = grid.entities;
    std::vector<std::string> oldEt = grid.entitiesType;
    std::vector<sf::Vector2f> oldScale = grid.entitiesScale;
    int oldW = grid.width, oldH = grid.height;

    grid.width = w;
    grid.height = h;
    grid.tiles.assign(w * h, 0);
    grid.entities.assign(w * h, 0);
    grid.entitiesType.assign(w * h, {});
    grid.entitiesProps.assign(w * h, nlohmann::json::object());
    grid.entitiesScale.assign(w * h, { 0.f, 0.f });

    int copyW = std::min(oldW, w);
    int copyH = std::min(oldH, h);
    for (int y = 0; y < copyH; ++y)
        for (int x = 0; x < copyW; ++x)
        {
            int oIdx = y * oldW + x;
            int nIdx = y * w + x;
            grid.tiles[nIdx] = oldTiles[oIdx];
            grid.entities[nIdx] = oldEntities[oIdx];
            grid.entitiesType[nIdx] = oldEt[oIdx];
            grid.entitiesScale[nIdx] = oldScale[oIdx];
        }

    worldView.setSize(w * grid.tileSize, h * grid.tileSize);
    worldView.setCenter(worldView.getSize() * 0.5f);
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
            std::string eName = grid.entitiesType[y * grid.width + x];
            //if (id <= 0 && Entityid <= 0) continue;
            if (id <= 0 && Entityid <= 0) continue;

            if (id <= 9)                                // 1~9 : 단색 타일
            {
                colorTile.setFillColor(Palette[id]);
                colorTile.setPosition(x * 16.f, y * 16.f);
                w.draw(colorTile);
            }
            //if (Entityid >= 100)                                     // 100+ : 텍스처 타일
            if(!eName.empty())
            {
                //int texIdx = Entityid - 100;
                const PrefabAsset* a = PrefabMgr::I().Get(eName);
                if (!a) continue;                

                //sf::Sprite s(TEXTURE_MGR.Get(a->sprite));
                sf::Sprite s(texArr[a->sprite]);
                sf::Vector2u ts = s.getTexture()->getSize();
                float k = float(grid.tileSize) / std::max(ts.x, ts.y);
                //s.setScale(k * a->scale.x, k * a->scale.y);
                s.setScale(k, k);
                s.setOrigin(ts.x * 0.5f, ts.y * 0.5f);
                s.setPosition((x + 0.5f) * grid.tileSize, (y + 0.5f) * grid.tileSize);
                w.draw(s);


                //int texIdx = Entityid - 100;                  // 0‑based
                //if (texIdx >= palette.GetTileSet().textures.size()) continue;

                //sf::Sprite ent;
                //ent.setTexture(palette.GetTileSet().textures[texIdx]);

                //sf::Vector2u texSz = palette.GetTileSet().textures[texIdx].getSize();

                ////float sx = float(grid.tileSize) / texSz.x;   
                ////float sy = float(grid.tileSize) / texSz.y;  
                ////spr.setScale(sx, sy);

                //float s = float(grid.tileSize) / std::max(texSz.x, texSz.y);
                //ent.setScale(s, s);
                //ent.setOrigin(texSz.x * 0.5f, texSz.y * 0.5f);
                //ent.setPosition((x + 0.5f) * grid.tileSize,
                //    (y + 0.5f) * grid.tileSize);
                ////spr.setPosition(x * grid.tileSize, y * grid.tileSize);
                //w.draw(ent);
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
    ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(1100, 100), ImGuiCond_FirstUseEver);
    ImGui::Begin("Level Settings");

    ImGui::Separator();
    //ImGui::Text("Tile palette");

    const int COLS = 5;
    const ImVec2 BTN_SZ(24, 24);

    for (int i = 1; i < (int)Palette.size(); ++i)        // 색 1~9
    {
        ImGui::PushID(i);

        ImVec4 col(
            Palette[i].r / 255.f,
            Palette[i].g / 255.f,
            Palette[i].b / 255.f,
            Palette[i].a / 255.f);
        bool pressed = ImGui::ColorButton("##c", col,
            ImGuiColorEditFlags_NoTooltip, BTN_SZ);

        if (pressed)
        {
            currentTile = i;       
            palette.ClearSelected();   
            currentEntity.clear();
        }

        if (currentTile == i && palette.GetSelected() == -1)
        {
            const float pad = 2.f;
            ImVec2 pmin = ImGui::GetItemRectMin();
            ImVec2 pmax = ImGui::GetItemRectMax();
            ImGui::GetWindowDrawList()->AddRect(
                ImVec2(pmin.x - pad, pmin.y - pad),
                ImVec2(pmax.x + pad, pmax.y + pad),
                IM_COL32(255, 255, 0, 255), 0.f, 0, 2.f);
        }

        ImGui::PopID();

        int colIdx = (i - 1) % COLS;
        if (colIdx + 1 < COLS) ImGui::SameLine();
    }
    ImGui::Separator();

    ImGui::InputText("Level file name", lvlName, IM_ARRAYSIZE(lvlName));
    ImGui::InputText("Author", author, IM_ARRAYSIZE(author));
    ImGui::InputTextMultiline("Description", desc,
        IM_ARRAYSIZE(desc),
        ImVec2(-FLT_MIN, 60));

    ImGui::InputInt("Width", &pendingW);
    ImGui::InputInt("Height", &pendingH);
    pendingW = Utils::Clamp(pendingW, 1, 100);   // 100 칸 정도로 제한
    pendingH = Utils::Clamp(pendingH, 1, 100);

    if (ImGui::Button("Apply grid size"))
    {
        if (pendingW != grid.width || pendingH != grid.height)
            ResizeGrid(pendingW, pendingH);     
    }

    if (ImGui::Button("Save"))
    {
        std::string file = "levels/" + std::string(lvlName) + ".json";
        SaveAsLevel(file);                       
    }
    ImGui::End();
    palette.DrawImGui(*this);
}
