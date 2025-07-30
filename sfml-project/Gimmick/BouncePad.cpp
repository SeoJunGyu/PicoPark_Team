#include "stdafx.h"
#include "BouncePad.h"
#include "Player.h"

BouncePad::BouncePad(nlohmann::json j)
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

void BouncePad::Reset()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;

	body.setTexture(TEXTURE_MGR.Get("graphics/Item/jump1.png"));

	player = nullptr;

	bounce = -(properties.value("Bounce", 0.f));

	SetOrigin(Origins::BC);
	SetPosition(GetPosition());
	SetScale(GetScale());
	SetRotation(GetRotation());

	hitBox.UpdateTransform(body, body.getLocalBounds());

	preScale = GetScale();

	Gimmick::Reset();
}

void BouncePad::Update(float dt)
{
	if (isPressed)
	{
		pressTimer += dt;
		if (pressTimer >= pressDuration)
		{
			SetScale(preScale);
			isPressed = false;
		}
	}

	sf::FloatRect padBox = hitBox.rect.getGlobalBounds();

	for (Player* p : Variables::players)
	{
		sf::FloatRect pBox = p->GetHitBox().rect.getGlobalBounds();

		CollisionInfo info = Utils::GetAABBCollision(pBox, padBox);
		if (info.depth <= 0.f)
		{
			continue;
		}

		// ¿·¸é Ãæµ¹
		if (std::abs(info.normal.x) > 0.5f)
		{
			float separationX = info.normal.x * info.depth;
			p->SetPosition({ p->GetPosition().x + separationX, p->GetPosition().y });
			p->velocity.x = 0.f;
			continue;
		}

		// À­¸é Ãæµ¹
		if (info.normal.y < -0.5f && p->velocity.y > 0.f)
		{
			p->velocity.y = bounce;
			p->isGrounded = false;
			SetScale({ GetScale().x, GetScale().y * 0.5f});
			isPressed = true;
			pressTimer = 0.f;
		}
	}

	SetOrigin(Origins::BC);
	hitBox.UpdateTransform(body, body.getLocalBounds());
	Gimmick::Update(dt);
}
