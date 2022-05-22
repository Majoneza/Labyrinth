#include "stdafx.h"
#include "Image.hpp"

Image::Image() :
	image(texture)
{
}

Image::Image(const sf::String& imageDir) :
	image(texture)
{
	setTexture(imageDir);
}

Image::Image(const sf::Vector2f& position, const sf::Vector2f& size) :
	image(texture)
{
	setPosition(position);
	setSize(size);
}

Image::Image(const sf::String& imageDir, const sf::Vector2f& position, const sf::Vector2f& size) :
	image(texture)
{
	setTexture(imageDir);
	setPosition(position);
	setSize(size);
}

void Image::onRender(sf::RenderWindow& win) const
{
	win.draw(image);
}

const sf::Vector2f& Image::getPosition() const
{
	return image.getPosition();
}

void Image::setRepeated(const bool& repeated)
{
	texture.setRepeated(repeated);
}

void Image::setSize(const float& x, const float& y)
{
	image.setScale(x / texture.getSize().x, y / texture.getSize().y);
	image.setOrigin(getSize().x / 2.f, getSize().y / 2.f);
}

void Image::setSize(const sf::Vector2f& size)
{
	image.setScale(size.x / texture.getSize().x, size.y / texture.getSize().y);
	image.setOrigin(getSize().x / 2.f, getSize().y / 2.f);
}

void Image::setPosition(const float& x, const float& y)
{
	image.setPosition(x, y);
}

void Image::setPosition(const sf::Vector2f& position)
{
	image.setPosition(position);
}

void Image::setRotation(const float& rotation)
{
	image.setRotation(rotation);
}

bool Image::setTexture(const sf::String& imageDir)
{
	if(!texture.loadFromFile(imageDir)) return false;
	image.setOrigin(getSize().x / 2.f, getSize().y / 2.f);
	return true;
}

sf::Vector2f Image::getSize() const
{
	return sf::Vector2f(image.getScale().x * texture.getSize().x, image.getScale().y * texture.getSize().y);
}

float Image::getRotation() const
{
	return image.getRotation();
}

void Image::move(const sf::Vector2f& offset)
{
	image.move(offset);
}

void Image::move(const float& x, const float& y)
{
	image.move(x, y);
}