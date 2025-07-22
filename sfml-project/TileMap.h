#pragma once
#include "GameObject.h"
class TileMap :
    public GameObject
{
protected:
	int tileW, tileH, gridW, gridH, firstSolidId;
	std::vector<int> tiles;
	sf::VertexArray verts; //렌더용
	std::vector<HitBox> solidHB; //충돌만 갖는 사각형

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

	void SetTile(int gx, int gy, int id);
};

