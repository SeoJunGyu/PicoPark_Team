#pragma once
#include "TileCore.hpp"

class SceneEditor;
class TilePalette {
public:
    void Load(const std::vector<std::string>& files, int tileSize);
    void DrawImGui(SceneEditor& editor);
    int  GetSelected() const { return currentId; }
    void ClearSelected() { currentId = -1; }
    const TileSet& GetTileSet() const { return tileSet; }

private:
    TileSet tileSet;
    int currentId = -1;
};
