#include "stdafx.h"
#include "BackGround.h"

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
}

void BackGround::Update(float dt)
{
}

void BackGround::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}

