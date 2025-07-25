#pragma once
#include "GameObject.h"

class UiButton : public GameObject
{
protected:
	std::function<void()> event;

public:
	sf::Text text;
	sf::Sprite sprite;
	sf::Color colorpull;
	bool isOn = true;
	

	UiButton(const std::string& name = "");
	virtual ~UiButton() = default;

	void SetCallBack(std::function<void()> callback) { event = callback; }

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float rot) override;
	void SetScale(const sf::Vector2f& s) override;
	void SetOrigin(const sf::Vector2f& o) override;
	void SetOrigin(Origins preset) override;
	void SetColor(const sf::Color& color);	
	void SetText(const std::string& str, const std::string& fontids, unsigned int size);
	void SetSprit(const std::string& texID);
	void Effect(bool on);

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