#include "stdafx.h"
#include "UiButton.h"
#include "Scene.h"

UiButton::UiButton(const std::string& name)
	: GameObject(name)
{
}

void UiButton::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	sprite.setPosition(pos);
	text.setPosition(pos);
}

void UiButton::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	sprite.setRotation(rot);
	text.setRotation(rot);
}

void UiButton::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	sprite.setScale(s);
	text.setScale(s);
}

void UiButton::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	sprite.setOrigin(o);
	text.setOrigin(o);
}

void UiButton::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	if (preset != Origins::Custom)
	{
		Utils::SetOrigin(sprite, preset);
		Utils::SetOrigin(text, preset);
	}
}

void UiButton::SetColor(const sf::Color& color)
{
	text.setFillColor(color);
	sprite.setColor(color);
}

void UiButton::SetText(const std::string& t, const std::string& fontid, int size)
{
	text.setString(t);
	text.setFont(FONT_MGR.Get(fontid));
	text.setCharacterSize(size);

	Utils::SetOrigin(text, Origins::MC);
}

void UiButton::SetTextstyle(const ButtonStyle& style)
{
	text.setString(style.text);
	text.setFont(FONT_MGR.Get(style.fontid));
	text.setCharacterSize(style.fontSize);
}

void UiButton::SetSprit(const std::string& texID)
{
	sprite.setTexture(TEXTURE_MGR.Get(texID));

	Utils::SetOrigin(text, Origins::MC);
}

void UiButton::Effect(bool on)
{
	sf::Color textcolor = text.getFillColor();
	on ? textcolor.a = 255 : textcolor.a = 0;
	text.setFillColor(textcolor);

	sf::Color spritecolor = sprite.getColor();
	on ? spritecolor.a = 255 : spritecolor.a = 0;
	sprite.setColor(spritecolor);
}

void UiButton::DrawEffect(float dt)
{
	sf::FloatRect bounds;
	
	if (!text.getString().isEmpty())
	{
		bounds = text.getGlobalBounds();		
	}
	else
	{
		bounds = sprite.getGlobalBounds();		
	}

	sf::Vector2f center = {
	bounds.left + bounds.width * 0.5f,
	bounds.top + bounds.height * 0.5f
	};
		

	animTime += dt;

	float scaleOffset = std::sin(animTime * speed) * amplitude;
	float scale = 1.f + scaleOffset;

	outline.setSize({ bounds.width + 20.f, bounds.height + 20.f });	
	outline.setOutlineThickness(7.f);
	outline.setOutlineColor(sf::Color(255, 127, 80, 255));	
	outline.setFillColor(sf::Color::Transparent);
	Utils::SetOrigin(outline, Origins::MC);
	outline.setScale({ scale + 0.08f, scale + 0.08f });
	outline.setPosition(center.x+5.f,center.y+5.f);
}


void UiButton::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 1;

	SetOrigin(Origins::MC);

	SOUNDBUFFER_MGR.Load("audio/click.mp3");
	SOUND_MGR.SetBgmVolume(50);

}

void UiButton::Release()
{
}

void UiButton::Reset()
{
}

void UiButton::Update(float dt)
{
	effectdrawon = false;

	if (enterLock)
	{
		if (!InputMgr::GetMouseButtonUp(sf::Mouse::Left))
			enterLock = false;
		/*yesnolock = false;*/
		return;
	}

	bool hovered = sprite.getGlobalBounds().contains((sf::Vector2f)InputMgr::GetMousePosition()) ||
		text.getGlobalBounds().contains((sf::Vector2f)InputMgr::GetMousePosition());

	isOn = InputMgr::GetMouseButton(sf::Mouse::Left) && hovered;

	if (hovered)
	{
		effectdrawon = true;
				
		if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
		{
			if (event)
				SOUND_MGR.PlaySfx("audio/click.mp3",false);
				event();
			enterLock = true;
		}
	}
	//else if (InputMgr::GetKeyDown(sf::Keyboard::Enter))
	//{
	//	//if (event)
	//		//event();
	//	enterLock = true;
	//	/*yesnolock = true;*/
	//}
	

	if (useeffect && effectdrawon)
	{
		this->DrawEffect(dt);
	}

	if (flashTime > 0.f)
	{
		flashTime -= dt;
		isOn = true;
	}
}

void UiButton::Draw(sf::RenderWindow& window)
{
	if (useeffect && effectdrawon)
	{
		window.draw(outline);
	}
	window.draw(sprite);
	window.draw(text);
	if (innerText.getString() != "")
	{
		innerText.setPosition(sprite.getPosition());
		innerText.setFont(FONT_MGR.Get("fonts/BACKTO1982.TTF"));
		window.draw(innerText);
	}
}

void UiButton::SetInnerText(const sf::Text& t)
{
	innerText = t;                       
	Utils::SetOrigin(innerText, Origins::MC);
}

bool UiButton::IsMouseOver() const
{
	bool hovered = sprite.getGlobalBounds().contains((sf::Vector2f)InputMgr::GetMousePosition()) ||
		text.getGlobalBounds().contains((sf::Vector2f)InputMgr::GetMousePosition());
	return hovered;
}
