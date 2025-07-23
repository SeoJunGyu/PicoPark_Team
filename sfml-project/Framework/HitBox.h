#pragma once
class HitBox
{
public:
	HitBox();

	sf::RectangleShape rect;

	void UpdateTransform(const sf::Transformable& tr, const sf::FloatRect& localBounds);
	void Draw(sf::RenderWindow& window);

	float GetLeft() const { return rect.getGlobalBounds().left; }
	float GetTop() const { return rect.getGlobalBounds().top; }
	float GetRight() const { auto b = rect.getGlobalBounds(); return b.left + b.width; }
	float GetBottom() const { auto b = rect.getGlobalBounds(); return b.top + b.height; }
	float GetWidth() const { return rect.getGlobalBounds().width; }
	float GetHeight() const { return rect.getGlobalBounds().height; }
};

