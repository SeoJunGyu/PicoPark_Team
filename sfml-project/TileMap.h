#pragma once

class TileMap
{
public:
    bool load(const Level& lvl);              
    void draw(sf::RenderWindow& win);         
    bool isSolid(int tx, int ty) const;      
    int  getTile(int tx, int ty) const;       
    void setTile(int tx, int ty, int id);     
private:
    int width, height, tileSize;
    std::vector<int> tiles;                  
    sf::VertexArray vertices;
};

