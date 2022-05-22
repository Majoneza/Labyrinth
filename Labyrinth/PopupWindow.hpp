#pragma once
#ifndef POPUPWINDOW_H
#define POPUPWINDOW_H

#include <SFML\Graphics.hpp>

class PlusShape : public sf::Drawable
{
public:
	PlusShape(const float& x, const float& y, const float& sX, const float& sY);

	PlusShape(const sf::Vector2f& position, const sf::Vector2f& size);

	void setFillColor(const sf::Color& color);

	void setOutlineColor(const sf::Color& color);

	void setOutlineThickness(float thickness);

	void setTexture(const sf::Texture* texture, bool resetRect = false);

	void setTextureRect(const sf::IntRect& rect);

	const sf::Texture* getTexture() const;

	const sf::IntRect& getTextureRect() const;

	const sf::Color& getFillColor() const;

	const sf::Color& getOutlineColor() const;

	float getOutlineThickness() const;

	sf::FloatRect getLocalBounds() const;

	sf::FloatRect getGlobalBounds() const;

	void setPosition(float x, float y);

	void setPosition(const sf::Vector2f& position);

	void setRotation(float angle);

	void setScale(float factorX, float factorY);

	void setScale(const sf::Vector2f& factors);

	void setSize(float sX, float sY);

	void setSize(const sf::Vector2f& size);

	void move(float offsetX, float offsetY);

	void move(const sf::Vector2f& offset);

	void rotate(float angle);

	void scale(float factorX, float factorY);

	void scale(const sf::Vector2f& factor);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
	sf::ConvexShape first;
	sf::ConvexShape second;
	sf::Transform transform;
};

class PopUpWindow
{
public:
	PopUpWindow();

	void onEvent(const sf::Event& evn);

	void onTick();

	void onRender(sf::RenderWindow& win);
private:
	sf::RectangleShape shape;
};

#endif