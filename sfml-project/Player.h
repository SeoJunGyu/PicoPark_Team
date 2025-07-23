#pragma once
#include "GameObject.h"
#include "Animator.h"

class TileMap;

class Player :
    public GameObject
{
protected:
	sf::Sprite body;
	Animator animator;

	sf::Vector2f gravity = { 0.f, 500.f };
	sf::Vector2f velocity = { 0.f, 0.f };
	bool isGrounded = true;
	float speed = 100.f;

	HitBox hitBox;
	TileMap* tilemap;

	//플레이어 구분 변수
	int index = 0;
	sf::Color tint = sf::Color::White;

public:
	Player(int idx, const sf::Color& c, const std::string& name = "Player");
	virtual ~Player() = default;

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

	sf::FloatRect GetLocalBounds() const override
	{
		return body.getLocalBounds();
	}

	sf::FloatRect GetGlobalBounds() const override
	{
		return body.getGlobalBounds();
	}

	HitBox GetHitBox() const { return hitBox; }

	void SetTileMap(TileMap* tilemap) { this->tilemap = tilemap; }
};

