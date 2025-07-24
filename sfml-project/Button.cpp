#include "stdafx.h"
#include "Button.h"
#include "Player.h"

Button::Button(nlohmann::json j)
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

void Button::Reset()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;

	body.setTexture(TEXTURE_MGR.Get("graphics/Item/Button.png"));
	pressed = false;

	SetOrigin(Origins::MC);
	SetPosition(GetPosition());
	SetScale(GetScale());
	SetRotation(GetRotation());

	hitBox.UpdateTransform(body, body.getLocalBounds());
	

	Gimmick::Reset();
}

void Button::Update(float dt)
{
	bool wasPressed = pressed;
	pressed = false;

	for (auto* p : Variables::players)
	{
		if (!Utils::CheckCollision(hitBox.rect, p->GetHitBox().rect))
		{
			continue;
		}

		//겹침 검사
		float overlapX = std::min(hitBox.GetLeft() + hitBox.GetWidth(), p->GetHitBox().GetLeft() + p->GetHitBox().GetWidth()) - std::max(hitBox.GetLeft(), p->GetHitBox().GetLeft());
		float overlapY = std::min(hitBox.GetTop() + hitBox.GetHeight(), p->GetHitBox().GetTop() + p->GetHitBox().GetHeight()) - std::max(hitBox.GetTop(), p->GetHitBox().GetTop());

		if (overlapY < overlapX && p->GetHitBox().GetTop() < hitBox.GetTop())
		{
			pressed = true;
			break;
		}
	}

	//상태가 바뀌었을때만 텍스처 및 신호 업데이트
	if (pressed != wasPressed)
	{
		if (pressed)
		{
			body.setTexture(TEXTURE_MGR.Get("graphics/Item/Button_Pressed.png"));
			
		}
		else
		{
			body.setTexture(TEXTURE_MGR.Get("graphics/Item/Button.png"));
		}
	}

	Gimmick::Update(dt);
}
