#pragma once
#include "GameObject.h"
#include "json.hpp"

class Gimmick :
    public GameObject
{

public:
	GimmickType type{ GimmickType::Key }; // 초기화 : Key
	sf::Vector2f velocity{ 0.f, 0.f }; //동적 기믹만 사용

	int pInt[2]{ 0, 0 }; //정수 1 ~ 2개 필요 오브젝트 : WeightPad(인수), ColorPushBlock(색상값), Button - MovingPlatform (채널)
	float pFloat[2]{ 0.f, 0.f }; //실수 1 ~ 2개 필요 오브젝트 : BouncePad(반발 속도 x y), FallPlatform(낙하 딜레이), MovingPlatform(이동, 루프시간)

	static Gimmick* CreateFromJson(const nlohmann::json& j);

protected:
	bool pressed = false; //button, pad
	float timer = 0.f; //fall delay , rope tention(장력)

	int id{};

	sf::Sprite body;
	nlohmann::json properties; //Door.locked, Platform.path 등

	HitBox hitBox;

	bool locked; //이렇게 properties 사용
	bool opened = false;
	bool waColliding = false;

public:
	Gimmick(int id, GimmickType t, const sf::Vector2f& pos, const sf::Vector2f& scl, float rot, nlohmann::json props);
	virtual ~Gimmick() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float rot) override;
	void SetScale(const sf::Vector2f& s) override;
	void SetOrigin(const sf::Vector2f& o) override;
	void SetOrigin(Origins preset) override;

	void Init() override;
	void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	sf::FloatRect GetLocalBounds() const override
	{
		return body.getLocalBounds();
	}

	sf::FloatRect GetGlobalBounds() const override
	{
		return body.getGlobalBounds();
	}

	HitBox GetHitBox() const { return hitBox; }

	//Gimmick function

};
