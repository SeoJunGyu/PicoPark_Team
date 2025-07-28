#pragma once
#include "GameObject.h"

struct ButtonStyle
{
	std::string text;
	std::string fontid;
	int fontSize;
};

class UiButton : public GameObject
{
protected:
	std::function<void()> event;

	sf::Text text;
	sf::Sprite sprite;
	sf::Color colorpull;

	int Size;	
	bool drawon = false;

	sf::RectangleShape outline;
	
	float animTime = 0.f;
	float speed = 5.f;
	float amplitude = 0.05f;

	bool drawon = false;
public:
	bool isOn = false;
	bool useeffect = false;

	UiButton(const std::string& name = "");
	virtual ~UiButton() = default;

	void SetCallBack(std::function<void()> callback) { event = callback; }

	
	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float rot) override;
	void SetScale(const sf::Vector2f& s) override;
	void SetOrigin(const sf::Vector2f& o) override;
	void SetOrigin(Origins preset) override;
	void SetColor(const sf::Color& color);	
	void SetText(const std::string& t, const std::string& fontid, int size);
	void SetTextstyle(const ButtonStyle& style);
	void SetSprit(const std::string& texID);
	sf::Vector2f GettextSize() const
	{ return sf::Vector2f(text.getGlobalBounds().width, text.getGlobalBounds().height); };
	void Effect(bool on);
	void DrawEffect(float dt);

	sf::FloatRect GetLocalBounds() const override
	{
		if (sprite.getTexture())
			return sprite.getLocalBounds();
		else
			return text.getLocalBounds();
	}

	sf::FloatRect GetGlobalBounds() const override
	{
		if (sprite.getTexture())
			return sprite.getGlobalBounds();
		else
			return text.getGlobalBounds();
	}



	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};