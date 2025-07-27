#include "stdafx.h"
#include "Button.h"
#include "Player.h"

std::vector<int> Button::momentaryCount;
std::vector<bool> Button::latchState;

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
	channel = properties.value("channel", 0);
	latch = properties.value("latch", false);
	//Variables::signals[channel] = false;

	EnsureSize(channel);

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

	bool detect = false; //충돌로 눌림 감지

	for (auto* p : Variables::players)
	{
		if (!Utils::CheckCollision(hitBox.rect, p->GetHitBox().rect))
		{
			continue;
		}

		//겹침 검사
		float overlapX = std::min(hitBox.GetLeft() + hitBox.GetWidth(), p->GetHitBox().GetLeft() + p->GetHitBox().GetWidth()) - std::max(hitBox.GetLeft(), p->GetHitBox().GetLeft());
		float overlapY = std::min(hitBox.GetTop() + hitBox.GetHeight(), p->GetHitBox().GetTop() + p->GetHitBox().GetHeight()) - std::max(hitBox.GetTop(), p->GetHitBox().GetTop());

		float contacY = p->GetHitBox().GetTop() + p->GetHitBox().GetHeight() - overlapY;

		//if (overlapY < overlapX && p->GetHitBox().GetTop() < hitBox.GetTop())
		if (overlapY < overlapX && contacY <= hitBox.GetTop() + 1.0f)
		{
			detect = true;
			break;
		}
	}

	if (latch)
	{
		if (detect && !wasPressed) //새로 눌렀을때만 토글
		{
			latchState[channel] = !latchState[channel];
		}

		pressed = wasPressed || detect;
	}
	else
	{
		pressed = detect;

		if (pressed && !wasPressed)
		{
			momentaryCount[channel]++;
		}
		if (!pressed && wasPressed)
		{
			momentaryCount[channel]--;
		}
	}
	momentaryCount[channel] = std::max(0, momentaryCount[channel]);

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

		//Variables::signals[channel] = pressed;
	}

	hitBox.UpdateTransform(body, body.getLocalBounds());
	//std::cout << channel << " : " << Variables::signals[channel] << std::endl;

	Gimmick::Update(dt);
}

void Button::EnsureSize(int channel)
{
	if (channel < 0)
	{
		return;
	}

	if (momentaryCount.size() <= channel)
	{
		momentaryCount.resize(channel + 1, 0);
		latchState.resize(channel + 1, false);
	}
}

bool Button::IsActive(int channel)
{
	if (channel < 0)                   
		return true;

	if (channel >= (int)latchState.size())
		return false;                  

	return latchState[channel] || (momentaryCount[channel] > 0);
}

void Button::ClearStates()
{
	momentaryCount.assign(momentaryCount.size(), 0);
	latchState.assign(latchState.size(), false);
}
