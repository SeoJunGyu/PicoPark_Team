#include "stdafx.h"
#include "Blinker.h"

Blinker::Blinker(nlohmann::json j)
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

void Blinker::Reset()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;

	minTime = properties.value("minTime", 10.f);
	maxTime = properties.value("maxTime", 20.f);
	resetDuration = properties.value("resetDuration", 3.f);
	rnd = Utils::RandomRange(minTime, maxTime);

	texGreen = TEXTURE_MGR.Get("graphics/Item/Blinkergreen.png");
	texYellow = TEXTURE_MGR.Get("graphics/Item/BlinkerOrange.png");
	texRed = TEXTURE_MGR.Get("graphics/Item/BlinkerRed.png");

	totalTime = rnd;
	currentTime = totalTime;
	changeYellow = totalTime * 0.1f;

	state = State::Green;
	body.setTexture(texGreen);

	SetOrigin(Origins::MC);
	SetPosition(GetPosition());
	SetScale(GetScale());
	SetRotation(GetRotation());

	hitBox.UpdateTransform(body, body.getLocalBounds());

	Gimmick::Reset();
}

void Blinker::Update(float dt)
{
	currentTime = std::max(0.f, currentTime - dt);

	State nextState = state;
	State prevState = state;

	if (currentTime <= 0.f)
	{
		nextState = State::Red;
	}
	else if (currentTime <= changeYellow)
	{
		nextState = State::Yellow;
	}
	else
	{
		nextState = State::Green;
	}

	if (nextState != state)
	{
		state = nextState;
		switch (state)
		{
		case Blinker::Green:
			body.setTexture(texGreen);
			break;
		case Blinker::Yellow:
			body.setTexture(texYellow);
			break;
		case Blinker::Red:
			body.setTexture(texRed);
			break;
		default:
			break;
		}
	}

	if (prevState != State::Red && state == State::Red)
	{
		enterRed = true;
		playerKill = false;
		resetTime = 0.f;
	}

	if (state == State::Red)
	{
		if (!playerKill)
		{
			bool anyMoving = false;
			for (auto* p : Variables::players)
			{
				if (p->velocity.x != 0.f || p->velocity.y != 0.f)
				{
					anyMoving = true;
					break;
				}
			}

			if (anyMoving)
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
		}

		resetTime += dt;
		if (resetTime >= resetDuration)
		{
			totalTime = Utils::RandomRange(minTime, maxTime);
			currentTime = totalTime;
			resetTime = 0.f;

			state = State::Green;
			body.setTexture(texGreen);
		}
	}

	if (Variables::players.size())
	{
		SetPosition({ Variables::players[0]->GetPosition().x, Variables::players[0]->GetPosition().y - 30.f });
		hitBox.UpdateTransform(body, body.getLocalBounds());
	}

	std::cout << totalTime << currentTime << std::endl;

	Gimmick::Update(dt);
}
