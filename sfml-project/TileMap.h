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
	//std::vector<HitBox> solidHB; //�浹�� ���� �簢��
	std::vector<sf::RectangleShape> solids;     // �浹�� �ڽ�(���� ��ǥ)

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
	bool isSolid(int tx, int ty) const;         // �浹
	int  getTile(int tx, int ty) const;         // ID ��ȸ
	void setTile(int tx, int ty, int id);       // �����Ϳ� ����
};

