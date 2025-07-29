#include "stdafx.h"
#include "PushBlock.h"
#include "Player.h"
#include "TileMap.h"

static sf::Color makeColor(int tileId)
{
    static std::vector<sf::Color> palette2 = {
        sf::Color::Blue,
        sf::Color(207, 159, 253),
        sf::Color::Green,
        sf::Color::Yellow
    };

    return palette2[tileId];
}

PushBlock::PushBlock(nlohmann::json j)
	: Gimmick(
		j.value("id", 0),
		StrToType(j.at("type").get<std::string>()),
		{ j.at("x").get<float>() , j.at("y").get<float>() },
		Variables::CalScl(j),
		j.value("rotation", 0.f),
		j.value("properties", nlohmann::json::object())
	)
{
}

void PushBlock::Reset()
{
    sortingLayer = SortingLayers::Foreground;
    sortingOrder = 0;

    body.setTexture(TEXTURE_MGR.Get("graphics/Item/WeightBlock.png"));

    player = nullptr;

    requireCount = properties.value("requireCount", 1);
    colorIndex = properties.value("color", -1);
    if (colorIndex >= 0)
    {
        blockColor = makeColor(colorIndex);
        body.setColor(blockColor);
        colorPush = true;
    }
    else
    {
        colorPush = false;
    }

    velocity = { 0.f, 0.f };
    isGrounded = false;

    tilemap = Variables::tilemap;

    standing.type = StandType::None;
    standing.ptr = nullptr;

    deltaPos = { 0.f, 0.f };

    SetOrigin(Origins::MC);
    SetPosition(GetPosition());
    SetScale(GetScale());
    SetRotation(GetRotation());

    hitBox.UpdateTransform(body, body.getLocalBounds());

    Gimmick::Reset();
}

void PushBlock::Update(float dt)
{
    sf::Vector2f before = GetPosition(); //프레임간 위치를 파악하기위해 이전 프레임 좌표 저장

    sf::FloatRect bBox = hitBox.rect.getGlobalBounds();
    hitBox.UpdateTransform(body, body.getLocalBounds());

    //미는 인원 확인
    int leftPushers = 0, rightPushers = 0;
    for (Player* p : Variables::players) 
    {
        sf::FloatRect pBox = p->GetHitBox().rect.getGlobalBounds();
        CollisionInfo info = Utils::GetAABBCollision(pBox, bBox);

        // 겹치지는 않았거나, 옆면 충돌이 아니면 무시
        if (info.depth <= 0.f || std::abs(info.normal.x) <= 0.5f)
        {
            continue;
        }

        // 플레이어가 블럭 안으로 파고들지 않게 분리
        float sepX = info.normal.x * info.depth;
        p->SetPosition({ p->GetPosition().x + sepX, p->GetPosition().y });
        p->velocity.x = 0.f;

        if (colorPush)
        {
            if (p->GetIndex() != colorIndex)
            {
                continue;
            }
        }

        // 왼쪽에서 밀면 info.normal.x < 0
        if (info.normal.x < 0.f)
        {
            ++leftPushers;
        }
        else
        {
            ++rightPushers;
        }

        
    }

    const float pushSpeed = 80.f;
    if (leftPushers >= requireCount && rightPushers < requireCount) 
    {
        velocity.x = +pushSpeed; // 오른쪽으로 이동
    }
    else if (rightPushers >= requireCount && leftPushers < requireCount) 
    {
        velocity.x = -pushSpeed; // 왼쪽으로 이동
    }
    else 
    {
        velocity.x = 0.f; // 그 외에는 정지
    }

    ApplySupport();

    //중력
    if (!isGrounded)
    {
        velocity.y += gravity.y * dt;
        //standingPlatform = nullptr; //딛고 선 플랫폼 초기화
        standing.clear();
    }

    //수평 이동
    position.x += velocity.x * dt;
    SetPosition(position);
    hitBox.UpdateTransform(body, body.getLocalBounds());

    //수평 충돌
    bool collidedX = false;

    //블럭 수평 충돌
    SetPosition(position);
    hitBox.UpdateTransform(body, body.getLocalBounds());

    for (auto* b : Variables::blocks)
    {
        if (b == this || !Utils::CheckCollision(hitBox.rect, b->GetHitBox().rect))
        {
            continue;
        }

        auto blockRect = hitBox.rect.getGlobalBounds();
        auto platRect = b->GetHitBox().rect.getGlobalBounds();
        CollisionInfo info = Utils::GetAABBCollision(blockRect, platRect);

        if (info.depth > 0.f && std::abs(info.normal.x) > 0.5f)
        {
            position.x += info.normal.x * info.depth;
            SetPosition(position);
            hitBox.UpdateTransform(body, body.getLocalBounds());
            velocity.x = 0.f;
            collidedX = true;
            break;
        }
    }

    //플랫폼 수평 충돌
    for (auto* plat : Variables::platforms)
    {
        if (!Utils::CheckCollision(hitBox.rect, plat->GetHitBox().rect))
        {
            continue;
        }

        auto blockRect = hitBox.rect.getGlobalBounds();
        auto platRect = plat->GetHitBox().rect.getGlobalBounds();
        CollisionInfo info = Utils::GetAABBCollision(blockRect, platRect);

        if (info.depth > 0.f && std::abs(info.normal.x) > 0.5f)
        {
            position.x += info.normal.x * info.depth;
            SetPosition(position);
            hitBox.UpdateTransform(body, body.getLocalBounds());
            velocity.x = 0.f;
            collidedX = true;
            break;
        }
    }

    //타일맵 수평 충돌
    if (!collidedX)
    {
        sf::Vector2f mapSize = tilemap->GetTileMapSize();
        int cols = tilemap->width, rows = tilemap->height;
        float tileW = mapSize.x / cols, tileH = mapSize.y / rows;
        auto blockRect = hitBox.rect.getGlobalBounds();

        int y0 = std::max(0, int(blockRect.top / tileH));
        int y1 = std::min(rows - 1, int((blockRect.top + blockRect.height) / tileH));
        int x0 = std::max(0, int(blockRect.left / tileW));
        int x1 = std::min(cols - 1, int((blockRect.left + blockRect.width) / tileW));

        for (int ty = y0; ty <= y1 && !collidedX; ++ty)
        {
            for (int tx = x0; tx <= x1; ++tx)
            {
                if (!tilemap->isSolid(tx, ty))
                {
                    continue;
                }

                sf::FloatRect tileRect{ tx * tileW, ty * tileH, tileW, tileH };
                CollisionInfo info = Utils::GetAABBCollision(blockRect, tileRect);

                if (info.depth > 0.f && std::abs(info.normal.x) > 0.5f)
                {
                    position.x += info.normal.x * info.depth;
                    SetPosition(position);
                    hitBox.UpdateTransform(body, body.getLocalBounds());
                    velocity.x = 0.f;
                    collidedX = true;
                    break;
                }
            }
        }
    }

    //수직 이동
    position.y += velocity.y * dt;
    SetPosition(position);
    hitBox.UpdateTransform(body, body.getLocalBounds());
    isGrounded = false;

    //수직 충돌
    bool collidedY = false;

    //블럭 수직 충돌
    SetPosition(position);
    hitBox.UpdateTransform(body, body.getLocalBounds());

    for (auto* b : Variables::blocks)
    {
        if (b == this || !Utils::CheckCollision(hitBox.rect, b->GetHitBox().rect))
        {
            continue;
        }

        auto blockRect = hitBox.rect.getGlobalBounds();
        auto platRect = b->GetHitBox().rect.getGlobalBounds();
        CollisionInfo info = Utils::GetAABBCollision(blockRect, platRect);

        if (info.depth > 0.f && std::abs(info.normal.y) > 0.5f)
        {
            position.y += info.normal.y * info.depth;
            SetPosition(position);
            hitBox.UpdateTransform(body, body.getLocalBounds());

            if (info.normal.y < 0.f)
            {
                isGrounded = true;
                velocity.y = 0.f;

                standing.type = StandType::PushBlock;
                standing.ptr = b;
            }
            collidedY = true;
            break;
        }
    }

    //플랫폼 수직 충돌
    for (auto* plat : Variables::platforms)
    {
        if (!Utils::CheckCollision(hitBox.rect, plat->GetHitBox().rect))
        {
            continue;
        }

        auto blockRect = hitBox.rect.getGlobalBounds();
        auto platRect = plat->GetHitBox().rect.getGlobalBounds();
        CollisionInfo info = Utils::GetAABBCollision(blockRect, platRect);

        if (info.depth > 0.f && std::abs(info.normal.y) > 0.5f)
        {
            position.y += info.normal.y * info.depth;
            SetPosition(position);
            hitBox.UpdateTransform(body, body.getLocalBounds());

            if (info.normal.y < 0.f)
            {
                isGrounded = true;
                velocity.y = 0.f;

                standing.type = StandType::Platform;
                standing.ptr = plat;
            }
            collidedY = true;
            break;
        }
    }

    //타일맵 수직 충돌
    if (!collidedY)
    {
        sf::Vector2f mapSize = tilemap->GetTileMapSize();
        int cols = tilemap->width, rows = tilemap->height;
        float tileW = mapSize.x / cols, tileH = mapSize.y / rows;
        auto blockRect = hitBox.rect.getGlobalBounds();

        int y0 = std::max(0, int(blockRect.top / tileH));
        int y1 = std::min(rows - 1, int((blockRect.top + blockRect.height) / tileH));
        int x0 = std::max(0, int(blockRect.left / tileW));
        int x1 = std::min(cols - 1, int((blockRect.left + blockRect.width) / tileW));

        for (int ty = y0; ty <= y1 && !collidedY; ++ty)
        {
            for (int tx = x0; tx <= x1; ++tx)
            {
                if (!tilemap->isSolid(tx, ty))
                {
                    continue;
                }

                sf::FloatRect tileRect{ tx * tileW, ty * tileH, tileW, tileH };
                CollisionInfo info = Utils::GetAABBCollision(blockRect, tileRect);

                if (info.depth > 0.f && std::abs(info.normal.y) > 0.5f)
                {
                    position.y += info.normal.y * info.depth;
                    SetPosition(position);
                    hitBox.UpdateTransform(body, body.getLocalBounds());

                    if (info.normal.y < 0.f)
                    {
                        isGrounded = true;
                        velocity.y = 0.f;
                    }
                    collidedY = true;
                    break;
                }
            }
        }
    }

    //플레이어 충돌
    sf::FloatRect blockBox = hitBox.rect.getGlobalBounds();
    hitBox.UpdateTransform(body, body.getLocalBounds());

    for (Player* p : Variables::players)
    {
        sf::FloatRect pBox = p->GetHitBox().rect.getGlobalBounds();

        CollisionInfo info = Utils::GetAABBCollision(blockBox, pBox);
        if (info.depth <= 0.f)
        {
            continue;
        }

        if (std::abs(info.normal.x) > 0.5f)
            continue;

        if (info.normal.y > 0.5f && p->velocity.y > 0.f)
        {
            float separationY = info.normal.y * info.depth;
            p->SetPosition({ p->GetPosition().x, p->GetPosition().y - separationY });
            p->velocity.y = 0.f;
            p->isGrounded = true;

            continue;
        }

        if (info.normal.y < -0.5f && p->velocity.y >= 0.f)
        {
            float separationY = info.normal.y * info.depth;
            //p->SetPosition({ p->GetPosition().x, p->GetPosition().y + separationY });

            p->velocity.y += 50.f;

            position.y += separationY;
            velocity.y = 0.f;
            isGrounded = true;
            standing.type = StandType::Player;
            standing.ptr = p;

            break;
        }
    }

    SetPosition(position);
    hitBox.UpdateTransform(body, body.getLocalBounds());

	if (isGrounded)
	{
		velocity.y = 0.f;
	}

    sf::Vector2f after = GetPosition(); //프레임간 위치를 파악하기위해 최종 프레임 좌표 저장
    deltaPos = after - before; //실제 이동한 차이만 저장된다.

	Gimmick::Update(dt);
}

sf::Vector2f PushBlock::GetSupportDelta()
{
    switch (standing.type)
    {
    case StandType::None:
        return { 0.f, 0.f };
    case StandType::Platform:
        return standing.asPlatform()->GetDeltaPos();
    case StandType::Player:
        return standing.asPlayer()->GetDeltaPos();
    case StandType::PushBlock:
        return standing.asPushBlock()->GetDeltaPos();
    }

    return { 0.f, 0.f };
}

void PushBlock::ApplySupport()
{
    position += GetSupportDelta(); //지지물 이동량 재귀적으로 누적
}
