#include "stdafx.h"
#include "YesNoPopupUI.h"
#include "UiButton.h"

void YesNoPopupUI::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	sprite.setPosition(pos);
	text.setPosition(pos);
}
void YesNoPopupUI::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	sprite.setRotation(rot);
	text.setRotation(rot);
}
void YesNoPopupUI::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	sprite.setScale(s);
	text.setScale(s);
}
void YesNoPopupUI::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	sprite.setOrigin(o);
	text.setOrigin(o);
}
void YesNoPopupUI::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	if (preset != Origins::Custom)
	{
		Utils::SetOrigin(sprite, preset);
		Utils::SetOrigin(text, preset);
	}
}

void YesNoPopupUI::SetText(const std::string& t, std::string& fontid, int size)
{
	text.setString(t);
	text.setFont(FONT_MGR.Get(fontid));
	text.setCharacterSize(size);
}

void YesNoPopupUI::Choiceevent(const UiButton* t, float dt)
{
	sf::Vector2f btnSize = t->GettextSize();             
	sf::Vector2f btnPos = t->GetPosition();          
	sf::Vector2f center = btnPos + btnSize * 0.5f;  

	animTime += dt;                                  

	float scaleOffset = std::sin(animTime * speed) * amplitude; 
	float scale = 1.f + scaleOffset;

	outline.setPosition(center);
	outline.setOrigin(outline.getSize() * 0.5f);  
	outline.setScale({ scale, scale });

}

void YesNoPopupUI::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 1;
	SetOrigin(Origins::MC);

	yesbut = new UiButton("Yes");
	nobut = new UiButton("No");

	outline.setOutlineColor(sf::Color::Black);
	outline.setOutlineThickness(3.f);
		
}

void YesNoPopupUI::Release()
{
	delete yesbut;
	delete nobut;
}

void YesNoPopupUI::Reset()
{
	sf::Vector2f winSize = FRAMEWORK.GetWindowSizeF();
	sprite.setTexture(TEXTURE_MGR.Get("graphics/MainMenuButton.png"));

	spr = sprite.getGlobalBounds();

	float sprx = spr.left + spr.width * 0.5f;
	float spry = spr.top + spr.height * 0.5f;

	sprite.setPosition(winSize.x * 0.5f - sprx, winSize.y * 0.5f);

	sf::Vector2f sprpos = sprite.getPosition();

	text.setPosition(sprpos.x + spr.width * 0.5f, sprpos.y + 40.f);

	yesbut->SetText("YES", "fonts/BACKTO1982.TTF", 40);	
	yesbut->SetColor(sf::Color::Black);
	yesbut->GetGlobalBounds();
	yesbut->SetPosition({ sprpos.x + 100.f,sprpos.y + spr.height * 0.6f });	

	nobut->SetText("NO", "fonts/BACKTO1982.TTF", 40);	
	nobut->SetColor(sf::Color::Black);
	nobut->GetGlobalBounds();
	nobut->SetPosition({ sprpos.x + spr.width - 100.f,sprpos.y + spr.height * 0.6f });

	closebut->SetText("X", "fonts/BACKTO1982.TTF", 20);	
	closebut->SetColor(sf::Color::Black);
	closebut->GetGlobalBounds();
	closebut->SetPosition({ sprpos.x + spr.width - 35.f,sprpos.y + 20.f });

}

void YesNoPopupUI::Update(float dt)
{
	yesbut->Update(dt);
	nobut->Update(dt);
	closebut->Update(dt);

	sf::Vector2f mousepos = (sf::Vector2f)InputMgr::GetMousePosition();

	sf::FloatRect yesbutbounds = yesbut->GetGlobalBounds();
	sf::FloatRect nobutbounds = yesbut->GetGlobalBounds();

	if (yesbutbounds.contains(mousepos))
	{
		Choiceevent(yesbut,dt);
	}
	else if (nobutbounds.contains(mousepos))
	{
		Choiceevent(nobut, dt);
	}



}

void YesNoPopupUI::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);
	window.draw(text);
	yesbut->Draw(window);
	nobut->Draw(window);
	closebut->Draw(window);
}
