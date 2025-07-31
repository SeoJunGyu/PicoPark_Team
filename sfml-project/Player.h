#pragma once
#include "GameObject.h"
#include "Animator.h"

struct StandMethod
{
	StandType type = StandType::None;
	void* ptr = nullptr;

	void clear()
	{
		type = StandType::None;
		ptr = nullptr;
	}

	MovingPlatform* asPlatform()
	{
		return type == StandType::Platform ? static_cast<MovingPlatform*>(ptr) : nullptr;
	}

	Player* asPlayer()
	{
		return type == StandType::Player ? static_cast<Player*>(ptr) : nullptr;
	}

	PushBlock* asPushBlock()
	{
		return type == StandType::PushBlock ? static_cast<PushBlock*>(ptr) : nullptr;
	}
};

class TileMap;

class Player :
    public GameObject
{
public:
	sf::Sprite body;
	sf::Vector2f velocity = { 0.f, 0.f };
	bool isGrounded = true;
	bool hasRider = false; //내 머리 위에 누가 올라탔음
	bool isOnPlayer = false; //내가 다른사람 밟고 있음

	StandMethod standing;

	bool hasKey = false;

	sf::Vector2f lastPushingPosition;	

	bool shouldPlayPushwalk = false;
	bool shouldPlayWalk = false;
	bool shouldPlayIdle = false;

	bool isDead = false;

	sf::FloatRect prevRect{};
	sf::Vector2f pendingScale{ -1.f, -1.f };

	int ghostDirection = 1;

protected:
	Animator animator;
	std::string currentStage;

	sf::Vector2f prvPos;
	sf::Vector2f deltaPos; //외부에서 현재 오브젝트 움직임을 파악해야할때
	sf::Vector2f gravity = { 0.f, 500.f };

	bool sideBlocked = false;
	bool isPushingSide = false;
	
	bool isFallen = false; //낙하 테스트 변수
	float speed = 50.f;
	float jumpPower = 150.f;

	HitBox hitBox;
	TileMap* tilemap;
	MovingPlatform* standingPlatform = nullptr;

	int index = 0;
	sf::Color tint = sf::Color::White;

	float coyoteCounter = 0.f; //지면을 떠난 뒤 남은 허용 시간
	float jumpBufferCounter = 0.f; //미리 눌러둔 점프의 남은 유효 시간

	float coyoteTime = 0.12f; //120ms
	float jumpBuffer = 0.12f; //120ms

	sf::Vector2f mapSize;

	float prevAxis = 0.f;

	//사망 관련 변수
	float deathTimer = 0.f;
	float deathDuration = 2.f; //2초 후 장면전환

public:
	Player(int idx, const sf::Color& c, const std::string& name = "Player");
	virtual ~Player() = default;

	void ApplyPendingScale();
	const sf::Vector2f& getPrvPos() const { return prvPos; }
	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float rot) override;
	void SetScale(const sf::Vector2f& s) override;
	void SetOrigin(const sf::Vector2f& o) override;
	void SetOrigin(Origins preset) override;
	void PlayLandAnim();
	void UpdateGroundAnim(bool sideBlocked);

	const std::string& getStage() const { return currentStage; }
	void setStage(const std::string& b) { currentStage = b; }

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

	int GetIndex() const { return index; }

	void SetTileMap(TileMap* tilemap) { this->tilemap = tilemap; }

	//같이 이동하기 위한 헬퍼
	sf::Vector2f GetSupportDelta();
	void ApplySupport();
	void ScanHeadStack();

	void OutWindow();

	void playeraniinit();
	void playeraniwalk();
	void playeranipushwalk();
	


	sf::Vector2f GetDeltaPos() const { return deltaPos; }

	void OnDie();
 std::string Getanimator() { return animator.GetCurrentClipId(); }
};

