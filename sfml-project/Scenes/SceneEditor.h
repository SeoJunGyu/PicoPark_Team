#pragma once
#include "Scene.h"
#include "TileCore.hpp"
#include "TilePalette.h"


class SceneEditor : public Scene
{
public:
    SceneEditor();
    void Brush(LevelGrid& grid, int tileId, sf::Vector2i mouse, sf::RenderWindow& win);
    void Init() override;
    void Enter() override;
    void Update(float dt) override;
    void Draw(sf::RenderWindow&) override;
    void SaveAsLevel(const std::string& path);
    void ResizeGrid(int w, int h);
    void LoadLevelFromJson(const std::string& path);
    bool has(const nlohmann::json& j, const std::string& k)
    {
        return j.contains(k) && !j[k].is_null();
    }
    std::unordered_map<std::string, nlohmann::json> prefabOverrides;
    std::vector<std::string> prefabNames;
    nlohmann::json pendingProps;
    std::vector<sf::Vector2f> penScale;
private:
    Level level;     
    TilePalette palette;
    LevelGrid grid;
    std::unordered_map<std::string, sf::Texture> texArr;
    int currentTile = 1;    
    int nextId = 0;
    int currentPlayerIndex = 0;
    int pIndex = 0;
    float uiStartX = 0.f;
    std::string currentEntity = ""; // "Key"/"Door"/"PlayerSpawn"
    char  lvlName[64] = "stage_tmp";   // �������� �̸�
    char  author[32] = "null";
    char  desc[128] = "desc";

    int   pendingW = 10;                // �׸��� ������
    int   pendingH = 6;

    sf::Vector2i mPos = { 0, 0 };

    std::array<sf::Color, 10> Palette = {
        sf::Color::Transparent,   // ����
        sf::Color(52,153,126),
        sf::Color(240,189, 72),
        sf::Color(206, 86,207),
        sf::Color(52,128,255),
        sf::Color(255, 96,122),
        sf::Color(128,128,128),
        sf::Color(80, 80,200),
        sf::Color(255,133, 77),
        sf::Color(20,180,180)
    };
};

