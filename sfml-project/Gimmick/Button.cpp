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
	dontPush = properties.value("dontPush", false);
	scalePlus = properties.value("scalePlus", false);
	scaleMinus = properties.value("scaleMinus", false);
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
	sf::FloatRect box = hitBox.rect.getGlobalBounds();

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

		sf::Vector2f scl = p->GetScale();
		auto clampSigned = [&](float scaleValue)
			{
				float s = (scaleValue < 0.f ? -1.f : 1.f);
				float absScale = std::abs(scaleValue);
				if (absScale < 0.05f)
				{
					absScale = 0.05f;
				}
				else if (absScale > 0.5f)
				{
					absScale = 0.5f;
				}
				return s * absScale;
			};
		sf::Vector2f prvPos = p->GetPosition();

		//if (overlapY < overlapX && p->GetHitBox().GetTop() < hitBox.GetTop())
		if (overlapY < overlapX && contacY <= hitBox.GetTop() + 3.0f)
		{
			detect = true;

			if (dontPush) //자폭버튼일경우
			{
				for (auto* p : Variables::players)
				{
					if (p->isDead)
					{
						continue;
					}
					p->OnDie();
				}
			}

			//스케일 설정 버튼일경우
			
			if (scalePlus && !scaleMinus)
			{
				scl.x *= (1.f + dt);
				scl.y *= (1.f + dt);

				scl.x = clampSigned(scl.x);
				scl.y = clampSigned(scl.y);

				p->pendingScale = scl;

				//scl.x += 0.02f * dt;
				//scl.y += 0.02f * dt;

				//scl.x = Utils::Clamp(scl.x, 0.05f, 0.5f);
				//scl.y = Utils::Clamp(scl.y, 0.05f, 0.5f);
			}
			else if (scaleMinus && !scalePlus)
			{
				scl.x *= (1.f - dt);
				scl.y *= (1.f - dt);

				scl.x = clampSigned(scl.x);
				scl.y = clampSigned(scl.y);

				p->SetScale(scl);

				//scl.x -= 0.02f * dt;
				//scl.y -= 0.02f * dt;

				//scl.x = Utils::Clamp(scl.x, 0.01f, 1.0f);
				//scl.y = Utils::Clamp(scl.y, 0.01f, 1.0f);
			}

			//scl.x = clampSigned(scl.x);
			//scl.y = clampSigned(scl.y);

			//p->pendingScale = scl;
			//p->SetScale(scl);
            //p->SetPosition(p->getPrvPos());
			//p->GetHitBox().UpdateTransform(p->body, p->body.getLocalBounds());
			//std::cout << "sc : " << scl.x << " / " << scl.y << std::endl;
			//std::cout << "after : " << p->GetPosition().x << " / " << p->GetPosition().y << std::endl;
			break;
		}

		std::cout << "before : " << prvPos.x << " / " << prvPos.y << std::endl;
		std::cout << "after : " << p->GetPosition().x << " / " << p->GetPosition().y << std::endl;
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
			SOUND_MGR.PlaySfx("audio/buttonclick.mp3",false);			
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
