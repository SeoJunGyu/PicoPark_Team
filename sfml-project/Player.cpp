#include "stdafx.h"
#include "Player.h"
#include "TileMap.h"

Player::Player(int idx, const sf::Color& c, const std::string& name)
	: GameObject(name), index(idx), tint(c)
{
}

void Player::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	body.setPosition(pos);
}

void Player::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	body.setRotation(rot);
}

void Player::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	body.setScale(s);
}

void Player::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	body.setOrigin(o);
}

void Player::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	if (preset != Origins::Custom)
	{
		Utils::SetOrigin(body, preset);
	}
}

void Player::Init()
{
	animator.SetTarget(&body);

	animator.AddEvent("Idle", 0,
		[]()
		{
			std::cout << "!!" << std::endl;
		}
	);

	animator.AddEvent("Idle", 0,
		[]()
		{
			std::cout << "??" << std::endl;
		}
	);

	isGrounded = false;
}

void Player::Release()
{
}

void Player::Reset()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 100;

	switch (index)
	{
	case 0:
		body.setTexture(TEXTURE_MGR.Get("graphics/Characters/Icon/Player0.png"));
		break;
	case 1:
		body.setTexture(TEXTURE_MGR.Get("graphics/Characters/Icon/Player1.png"));
		break;
	case 2:
		body.setTexture(TEXTURE_MGR.Get("graphics/Characters/Icon/Player2.png"));
		break;
	case 3:
		body.setTexture(TEXTURE_MGR.Get("graphics/Characters/Icon/Player3.png"));
		break;
	}

	float ox = 16.f * 0.5f;
	float oy = 16.f * 0.5f;

	float realH = body.getTexture()->getSize().y * body.getScale().y;
	float oy_fix = std::abs((realH - 16.f) * 0.5f);

	sf::Vector2f pos(GetPosition().x + ox, GetPosition().y + oy + oy_fix);
	SetPosition(pos);
	
	SetOrigin(Origins::BC);
	hitBox.UpdateTransform(body, body.getLocalBounds());
}

void Player::Update(float dt)
{
	animator.Update(dt);

	hasRider = false; //플레이어가 내 머리 위에 있는지 확인

	ScanHeadStack(); //머리 위 스캔 - 독립적으로 플래그 확정
	ApplySupport(); //이동물 탑승시 이동량 헬퍼

	if (InputMgr::GetJump(index))
	{

		jumpBufferCounter = jumpBuffer; //점프 입력 기록
	}

	//코요테 타임 갱신
	if (isGrounded)
	{
		coyoteCounter = coyoteTime; //지면에 붙어 있으면 리필
	}
	else
	{
		coyoteCounter -= dt; //공중이면 감소
	}

	//중력 적용
	jumpBufferCounter -= dt; //시간이 흐르면 버퍼 소멸

	float h = InputMgr::GetAxis(index, Axis::Horizontal);
	velocity.x = h * speed;

	if (!isGrounded)
	{
		velocity.y += gravity.y * dt;
		//standingPlatform = nullptr; //딛고 선 플랫폼 초기화
		standing.clear();
	}
	
	sf::FloatRect prevRect = hitBox.rect.getGlobalBounds();
	prvPos = position;

	//position += velocity * dt;
	position.x += velocity.x * dt;

	//Collision
	// 타일 가로
	SetPosition(position);
	hitBox.UpdateTransform(body, body.getLocalBounds());
	int ts = tilemap->GetTileSize();
	int leftTx = int(hitBox.GetLeft() / ts);
	int rightTx = int((hitBox.GetRight() + 0.2f) / ts);
	int topTy = int((hitBox.GetTop() + 0.2f) / ts);
	int botTy = int((hitBox.GetBottom() + 0.2f) / ts);

	bool hitRight = tilemap->isSolid(rightTx, topTy) || tilemap->isSolid(rightTx, botTy);
	bool hitLeft = tilemap->isSolid(leftTx, topTy) || tilemap->isSolid(leftTx, botTy);
	
	for (int tx : { leftTx, rightTx })
	{
		if (!tilemap->isSolid(tx, topTy) && !tilemap->isSolid(tx, botTy))
		{
			continue;
		}

		//ts, ts * 2 -> 두 줄(Top~Bottom) 커버
		sf::FloatRect tileRect(tx * ts, topTy * ts, ts, ts * 2);

		CollisionInfo info = Utils::GetAABBCollision(hitBox.rect.getGlobalBounds(), tileRect);

		if (info.depth > 0.f && std::abs(info.normal.x) > 0.5f)
		{
			position.x += info.normal.x * info.depth; // 최소 깊이만큼 밀어냄
			velocity.x = 0.f;

			//겹침이 해소됐으니 더 볼 필요 없음
			break;
		}
	}

	// 플랫폼 가로
	SetPosition(position);
	hitBox.UpdateTransform(body, body.getLocalBounds());
	for (auto* plat : Variables::platforms)
	{
		if (!Utils::CheckCollision(hitBox.rect, plat->GetHitBox().rect))
		{
			continue;
		}

		float prevBottom = prevRect.top + prevRect.height;

		sf::FloatRect playerRect = hitBox.rect.getGlobalBounds();
		sf::FloatRect platRect = plat->GetHitBox().rect.getGlobalBounds();

		if (velocity.y > 0.f && prevBottom <= platRect.top + 0.1f)
		{
			//낙하중이고, 이전 발바닥이 플래폼 윗면보다 위였으면 -> ㅊ옆면 처리 스킵 (착지 후보니까)
			continue;
		}

		if (velocity.y < 0.f && prevRect.top >= prevRect.top + platRect.height - 0.1f)
		{
			continue;
		}

		//진짜 옆면만 처리
		CollisionInfo info = Utils::GetAABBCollision(playerRect, platRect);

		if (info.depth > 0.f && std::abs(info.normal.x) > 0.5f)
		{
			position.x += info.normal.x * info.depth;
			velocity.x = 0.f;
			SetPosition(position);
			hitBox.UpdateTransform(body, body.getLocalBounds());
			break;
		}
	}

	//수직 이동 갱신
	position.y += velocity.y * dt;

	// 세로
	SetPosition(position);
	hitBox.UpdateTransform(body, body.getLocalBounds());

	leftTx = int(hitBox.GetLeft() / ts);
	rightTx = int((hitBox.GetRight() - 0.2f) / ts);
	topTy = int((hitBox.GetTop() + 0.2f) / ts);
	botTy = int((hitBox.GetBottom() + 0.2f) / ts);

	float footMargin = 0.5f; //벽타일 제외용 여유
	int footLeftTx = int((hitBox.GetLeft() + footMargin) / ts);
	int footRightTx = int((hitBox.GetRight() - footMargin) / ts);

	bool hitBottom = tilemap->isSolid(footLeftTx, botTy) || tilemap->isSolid(footRightTx, botTy);
	bool hitTop = tilemap->isSolid(leftTx, topTy) || tilemap->isSolid(rightTx, topTy);

	bool wasGround = isGrounded;

	if (velocity.y >= 0.f)
	{
		if (hitBottom)
		{
			position.y = prvPos.y;
			//position.y = std::floor(botTy * ts - hitBox.GetHeight());
			velocity.y = 0.f;
			isGrounded = true;  // 착지
			//isFallen = false;
		}
		else
		{
			if (wasGround)
			{
				isGrounded = false;
				//isFallen = true;
			}
		}
	}
	else if (velocity.y < 0.f && hitTop)
	{
		position.y = prvPos.y;
		//position.y = std::floor((topTy + 1) * ts);
		velocity.y = 0.f;
		//isGrounded = true;
	}

	//플랫폼 세로 충돌
	SetPosition(position);
	hitBox.UpdateTransform(body, body.getLocalBounds());
	for (auto* plat : Variables::platforms)
	{
		if (!Utils::CheckCollision(hitBox.rect, plat->GetHitBox().rect))
		{
			continue;
		}

		sf::FloatRect playerRect = hitBox.rect.getGlobalBounds();
		sf::FloatRect platRect = plat->GetHitBox().rect.getGlobalBounds();
		CollisionInfo info = Utils::GetAABBCollision(playerRect, platRect);

		if (info.depth <= 0.f || std::abs(info.normal.y) < 0.5f)
		{
			continue;
		}

		if (info.depth > 0.f && std::abs(info.normal.y) > 0.5f)
		{
			position.y += info.normal.y * info.depth;
			SetPosition(position);
			hitBox.UpdateTransform(body, body.getLocalBounds());

			if (info.normal.y < 0.f) //플랫폼 착지
			{
				isGrounded = true;
				//standingPlatform = plat;
				standing.type = StandType::Platform;
				standing.ptr = plat;
				velocity.y = 0.f;
			}
			else if(info.normal.y > 0.f) //플랫폼 하단 충돌
			{
				//velocity.y = std::min(velocity.y, 0.f);
				velocity.y = 0.f;
			}
			break;
		}
		
	}

	// 플레이어 세로 충돌
	SetPosition(position);
	hitBox.UpdateTransform(body, body.getLocalBounds());

	for (auto* other : Variables::players)
	{
		if (other == this)
		{
			continue;
		}

		if (!Utils::CheckCollision(hitBox.rect, other->GetHitBox().rect))
		{
			continue;
		}

		//겹침 검사
		CollisionInfo info = Utils::GetAABBCollision(hitBox.rect.getGlobalBounds(), other->GetHitBox().rect.getGlobalBounds());

		if (info.depth <= 0.f)
		{
			continue;
		}

		if (info.normal.y < -0.5f && velocity.y >= 0.f) //내가 위이고 낙하중일경우 -> 착지
		{
			position.y += info.normal.y * info.depth;
			velocity.y = 0.f;
			isGrounded = true;
			standing.type = StandType::Player;
			standing.ptr = other;

			other->hasRider = true; //너 머리에 플레이어 있다고 알림
			//standingPlatform = other;
		}
		else if (info.normal.y > 0.5f) //내가 아래
		{
			hasRider = true;
			velocity.y = 50.f;
		}
		
	}
	
	// 플레이어 가로 충돌
	SetPosition(position);
	hitBox.UpdateTransform(body, body.getLocalBounds());
	
	for (auto* other : Variables::players)
	{
		if (other == this)
		{
			continue;
		}

		if (!Utils::CheckCollision(hitBox.rect, other->GetHitBox().rect))
		{
			continue;
		}

		if (standing.type == StandType::Player && standing.ptr != other)
		{
			continue;
		}

		//내가 다른 플레이어 위에 올라탄 경우
		if (standing.type == StandType::Player && standing.ptr == other)
		{
			continue; //머리 위에 서 있는 대상이면 옆면 충돌 처리를 스킵한다.
		}
		if (other->standing.type == StandType::Player && other->standing.ptr == this)
		{
			continue;
		}

		//겹침 검사
		CollisionInfo info = Utils::GetAABBCollision(hitBox.rect.getGlobalBounds(), other->GetHitBox().rect.getGlobalBounds());

		if (info.depth <= 0.f)
		{
			continue;
		}

		//x축만 확인
		if (std::abs(info.normal.x) > 0.5f) //normal.x가 -1(왼) 또는 1(오른)
		{
			position.x += info.normal.x * info.depth;
			velocity.x = 0.f;
		}
	}

	//점프 수행 조건
	if (coyoteCounter > 0.f && jumpBufferCounter > 0.f && !hasRider)
	{
		isGrounded = false;
		velocity.y = -jumpPower;
		coyoteCounter = 0.f; //소모
		jumpBufferCounter = 0.f;
	}

	if (h != 0.f)
	{
		sf::Vector2f s = GetScale();
		s.x = std::abs(s.x) * (h > 0.f ? 1.f : -1.f);
		SetScale(s);
	}

	//화면 경계 검사
	float halfW = hitBox.GetWidth() * 0.5f;

	position.x = Utils::Clamp(position.x, 0.f + halfW, tilemap->GetTileMapSize().x - halfW);

	OutWindow();

	//최종 위치 설정
	SetPosition(position);
	hitBox.UpdateTransform(body, body.getLocalBounds());
	
	//std::cout << hitBottom << " / " << hitTop << " / " << velocity.y << " / " << isGrounded << std::endl;
	// Ani
}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(body);

	if (Variables::isDrawHitBox)
	{
		hitBox.Draw(window);
	}
	
}

sf::Vector2f Player::GetSupportDelta()
{
	switch (standing.type)
	{
		case StandType::None:
			return { 0.f, 0.f };
		case StandType::Platform:
			return standing.asPlatform()->GetDeltaPos();
		case StandType::Player:
		{
			Player* base = standing.asPlayer();
			return (base->position - base->prvPos) + base->GetSupportDelta();
		}
	}

	return { 0.f, 0.f };
}

void Player::ApplySupport()
{
	position += GetSupportDelta(); //지지물 이동량 재귀적으로 누적
}

void Player::ScanHeadStack()
{
	hasRider = false;
	isOnPlayer = false;
	if (standing.type == StandType::Player)
	{
		standing.clear(); //서있는곳이 플레이어 위면 지지물 컨테이너를 잠시 비운다.
	}

	sf::FloatRect myHitBox = hitBox.rect.getGlobalBounds();
	float myHeadY = myHitBox.top;
	float myFeetY = myHitBox.top + myHitBox.height;
	float ocha = 1.0f; //오차 허용

	for (Player* other : Variables::players)
	{
		if (other == this)
		{
			continue;
		}

		sf::FloatRect otherBox = other->GetHitBox().rect.getGlobalBounds();
		bool horizOverlap = (myHitBox.left < otherBox.left + otherBox.width) && (myHitBox.left + myHitBox.width > otherBox.left);

		if (!horizOverlap)
		{
			continue;
		}

		float overlap = std::min(myHitBox.left + myHitBox.width, otherBox.left + otherBox.width) - std::max(myHitBox.left, otherBox.left);

		if (overlap < 0.1f)        // 0.1픽셀 미만이면 스택 판정 무시
		{
			continue;
		}

		float otherHeadY = otherBox.top;
		float otherFeetY = otherBox.top + otherBox.height;

		if (std::abs(otherFeetY - myHeadY) < ocha) //다른 플레이어가 나를 밟고있음
		{
			hasRider = true;
			other->isOnPlayer = true;

			other->standing.type = StandType::Player;
			other->standing.ptr = this;
		}
		else if (std::abs(myFeetY - otherHeadY) < ocha) //내가 다른 플레이어 밟고있음
		{
			isOnPlayer = true;
			standing.type = StandType::Player;
			standing.ptr = other;
		}
	}
}

void Player::OutWindow()
{
	sf::Vector2f windowSize = tilemap->GetTileMapSize();
	sf::Vector2f pos = position; //현재 위치

	if (pos.y > windowSize.y)
	{
		pos.x = pos.x - 30.f;
		pos.y = 0.f;
	}

	position = pos;
}
