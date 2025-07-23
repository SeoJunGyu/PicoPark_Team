#pragma once
class HitBox
{
public:
	HitBox();

	sf::RectangleShape rect;

	void UpdateTransform(const sf::Transformable& tr, const sf::FloatRect& localBounds);
	void Draw(sf::RenderWindow& window);

	float GetLeft() const { return rect.getLocalBounds().left; }
	float GetTop() const { return rect.getLocalBounds().top; }
	float GetRight() const { auto b = rect.getLocalBounds(); return b.left + b.width; }
	float GetBottom() const { auto b = rect.getLocalBounds(); return b.top + b.height; }
	float GetWidth() const { return rect.getLocalBounds().width; }
	float GetHeight() const { return rect.getLocalBounds().height; }
};

