#include "stdafx.h"
#include "BackGround.h"

bool BackGround::isFadingout = false;

BackGround::BackGround(const std::string& name)
	:GameObject(name)
{
}

void BackGround::Settext(const std::string& textid)
{
	sprite.setTexture(TEXTURE_MGR.Get(textid));
}

void BackGround::Init()
{
	sortingLayer = SortingLayers::Background;
	sortingOrder = 1;
}

void BackGround::Release()
{
}

void BackGround::Reset()
{
	isFadingout = false;
	fadeAlpha = 0.f;
	fadeComplete = false;

	whiteOverlay.setSize(FRAMEWORK.GetWindowSizeF());	
	whiteOverlay.setFillColor(sf::Color(255, 255, 255, 0));
	whiteOverlay.setPosition(0.f, 0.f);
}

void BackGround::Update(float dt)
{	

	if (isFadingout)
	{
		fadeAlpha += fadeSpeed * dt;
		std::cout << fadeAlpha << std::endl;
		if (fadeAlpha >= 255)
		{
			fadeAlpha = 255;			
			fadeComplete = true;
		}
		whiteOverlay.setFillColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(fadeAlpha)));	
	}
}

void BackGround::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);	
}

void BackGround::StartFadeOut()
{	
	isFadingout = true;
	fadeAlpha = 0.f;
	fadeComplete = false;
}
bool BackGround::IsFadeOutComplete() const
{
	return fadeComplete;
}

