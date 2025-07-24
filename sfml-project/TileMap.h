#pragma once
#include "GameObject.h"
#include "LevelLoader.h"

class TileMap :
    public GameObject
{
protected:
	int width = 0, height = 0, tileSize = 16, firstSolid = 1;
	std::vector<int> tiles;
	//sf::VertexArray vertices; 
	//std::vector<HitBox> solidHB;
	std::vector<sf::RectangleShape> solids;

	sf::Texture* texture = nullptr;
	sf::Transform transform;

public:
	TileMap(const std::string& name = "");
	virtual ~TileMap() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float rot) override;
	void SetScale(const sf::Vector2f& s) override;
	void SetOrigin(const sf::Vector2f& o) override;
	void SetOrigin(Origins preset) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	bool load(const Level& lvl, int solidStart); //Json -> TileMap
	bool isSolid(int tx, int ty) const;
	int  getTile(int tx, int ty) const;
	void setTile(int tx, int ty, int id);

	int GetTileSize() const { return tileSize; }
	sf::Vector2f GetTileMapSize() const { return { (float)width * tileSize, (float)height * tileSize }; }
};

