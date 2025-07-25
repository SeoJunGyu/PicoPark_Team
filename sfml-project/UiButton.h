#pragma once
#include "GameObject.h"

class UiButton : public GameObject
{
protected:
	std::function<void()> event;

public:
	sf::Sprite sprite;
	std::string buttonOnTexId;
	std::string buttonOffTexId;
	bool isOn = true;
	sf::Text text;

	Button(const std::string& name = "");
	virtual ~Button() = default;

	void SetCallBack(std::function<void()> callback) { event = callback; }

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float rot) override;
	void SetScale(const sf::Vector2f& s) override;
	void SetOrigin(const sf::Vector2f& o) override;
	void SetOrigin(Origins preset) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};