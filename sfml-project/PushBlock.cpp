#include "stdafx.h"
#include "PushBlock.h"
#include "Player.h"
#include "TileMap.h"

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
	blockColor = sf::Color(properties["color"][0].get<std::uint8_t>(), properties["color"][1].get<std::uint8_t>(), properties["color"][2].get<std::uint8_t>(), properties["color"][3].get<std::uint8_t>());

	velocity = { 0.f, 0.f };
	isGrounded = false;

	tilemap = Variables::tilemap;

	SetOrigin(Origins::BC);
	SetPosition(GetPosition());
	SetScale(GetScale());
	SetRotation(GetRotation());

	hitBox.UpdateTransform(body, body.getLocalBounds());

	Gimmick::Reset();
}

void PushBlock::Update(float dt)
{
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

        // 왼쪽에서 밀면 info.normal.x < 0
        if (info.normal.x < 0.f)
        {
            ++leftPushers;
        }
        else
        {
            ++rightPushers;
        }

        // 플레이어가 블럭 안으로 파고들지 않게 분리
        float sepX = info.normal.x * info.depth;
        p->SetPosition({ p->GetPosition().x + sepX, p->GetPosition().y });
        p->velocity.x = 0.f;
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

    //중력
    velocity.y += gravity.y * dt;

    //수평 이동
    position.x += velocity.x * dt;
    SetPosition(position);
    hitBox.UpdateTransform(body, body.getLocalBounds());

    //수평 충돌
    bool collidedX = false;

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

    int count = 0;

    for (Player* p : Variables::players)
    {
        sf::FloatRect pBox = p->GetHitBox().rect.getGlobalBounds();

        CollisionInfo info = Utils::GetAABBCollision(pBox, blockBox);
        if (info.depth <= 0.f)
        {
            continue;
        }

        // 옆면 충돌
        if (std::abs(info.normal.x) > 0.5f)
        {
            float separationX = info.normal.x * info.depth;
            p->SetPosition({ p->GetPosition().x + separationX, p->GetPosition().y });
            p->velocity.x = 0.f;
            continue;
        }

        // 아랫면 충돌
        if (info.normal.y > 0.5f && p->velocity.y < 0.f)
        {
            float separationY = info.normal.y * info.depth;
            p->SetPosition({ p->GetPosition().x, p->GetPosition().y + separationY });

            p->SetPosition({ p->GetPosition().x, p->GetPosition().y + separationY });
            p->velocity.y = 0.f;
            continue;
        }

        // 윗면 충돌
        if (info.normal.y < -0.5f)
        {
            float separationY = info.normal.y * info.depth;
            p->SetPosition({ p->GetPosition().x, p->GetPosition().y + separationY });

            // 착지 처리
            p->velocity.y = 0.f;
            p->isGrounded = true;

            continue;
        }
    }

    hitBox.UpdateTransform(body, body.getLocalBounds());

	if (isGrounded)
	{
		velocity.y = 0.f;
	}

	Gimmick::Update(dt);
}
