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


	sf::Vector2f prvPos;
	sf::Vector2f gravity = { 0.f, 500.f };
	sf::Vector2f velocity = { 0.f, 0.f };
	bool isGrounded = true;
	bool isFallen = false; //낙하 테스트 변수
	float speed = 100.f;
	float jumpPower = 150.f;

	HitBox hitBox;
	TileMap* tilemap;

	int index = 0;
	sf::Color tint = sf::Color::White;

	float coyoteCounter = 0.f; //지면을 떠난 뒤 남은 허용 시간
	float jumpBufferCounter = 0.f; //미리 눌러둔 점프의 남은 유효 시간

	float coyoteTime = 0.12f; //120ms
	float jumpBuffer = 0.12f; //120ms`

public:
	Player(int idx, const sf::Color& c, const std::string& name = "Player");
	virtual ~Player() = default;

	const sf::Vector2f& getPrvPos() const { return prvPos; }
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

