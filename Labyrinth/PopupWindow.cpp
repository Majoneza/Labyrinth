#include "stdafx.h"
#include "PopupWindow.hpp"

PlusShape::PlusShape(const float& x, const float& y, const float& sX, const float& sY) :
	first(2),
	second(2),
	transform()
{
	setPosition(x, y);
	setSize(sX, sY);
	sf::Transformable t;
	
}

PlusShape::PlusShape(const sf::Vector2f& position, const sf::Vector2f& size) :
	first(2),
	second(2)
{

}

void PlusShape::setFillColor(const sf::Color& color)
{
	first.setFillColor(color);
	second.setFillColor(color);
}

void PlusShape::setOutlineColor(const sf::Color& color)
{
	first.setOutlineColor(color);
	second.setOutlineColor(color);
}

void PlusShape::setOutlineThickness(float thickness)
{
	first.setOutlineThickness(thickness);
	second.setOutlineThickness(thickness);
}

void PlusShape::setTexture(const sf::Texture* texture, bool resetRect)
{
	first.setTexture(texture, resetRect);
	second.setTexture(texture, resetRect);
}

void PlusShape::setTextureRect(const sf::IntRect& rect)
{
	first.setTextureRect(rect);
	second.setTextureRect(rect);
}

const sf::Texture* PlusShape::getTexture() const
{
	return first.getTexture();
}

const sf::IntRect& PlusShape::getTextureRect() const
{
	return first.getTextureRect();
}

const sf::Color& PlusShape::getFillColor() const
{
	return first.getFillColor();
}

const sf::Color& PlusShape::getOutlineColor() const
{
	return first.getOutlineColor();
}

float PlusShape::getOutlineThickness() const
{
	return first.getOutlineThickness();
}

sf::FloatRect PlusShape::getLocalBounds() const
{
	return sf::FloatRect();
}

sf::FloatRect PlusShape::getGlobalBounds() const
{
	return sf::FloatRect();
}

void PlusShape::setPosition(float x, float y)
{

}

void PlusShape::setPosition(const sf::Vector2f& position)
{

}

void PlusShape::setRotation(float angle)
{

}

void PlusShape::setScale(float factorX, float factorY)
{

}

void PlusShape::setScale(const sf::Vector2f& factors)
{

}

void PlusShape::setSize(float sX, float sY)
{

}

void PlusShape::setSize(const sf::Vector2f& factors)
{

}

void PlusShape::move(float offsetX, float offsetY)
{

}

void PlusShape::move(const sf::Vector2f& offset)
{

}

void PlusShape::rotate(float angle)
{

}

void PlusShape::scale(float factorX, float factorY)
{

}

void PlusShape::scale(const sf::Vector2f& factor)
{

}

void PlusShape::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(first, states);
	target.draw(second, states);
}

PopUpWindow::PopUpWindow() :
	shape()
{
	
}

void PopUpWindow::onEvent(const sf::Event& evn)
{

}

void PopUpWindow::onTick()
{

}

void PopUpWindow::onRender(sf::RenderWindow& win)
{

}