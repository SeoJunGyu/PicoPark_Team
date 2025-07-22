#pragma once
#include "GameObject.h"
#include "json.hpp"

class Gimmick :
    public GameObject
{

public:
	GimmickType type{ GimmickType::NormalBlock }; // �ʱ�ȭ : Key
	sf::Vector2f velocity{ 0.f, 0.f }; //���� ��͸� ���

	int pInt[2]{ 0, 0 }; //���� 1 ~ 2�� �ʿ� ������Ʈ : WeightPad(�μ�), ColorPushBlock(����), Button - MovingPlatform (ä��)
	float pFloat[2]{ 0.f, 0.f }; //�Ǽ� 1 ~ 2�� �ʿ� ������Ʈ : BouncePad(�ݹ� �ӵ� x y), FallPlatform(���� ������), MovingPlatform(�̵�, �����ð�)

	static Gimmick* CreateFromJson(const nlohmann::json& j);

protected:
	bool pressed = false; //button, pad
	float timer = 0.f; //fall delay , rope tention(���)

	int id{};

	sf::Sprite body;
	nlohmann::json properties; //Door.locked, Platform.path ��

	HitBox hitBox;

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

};

