#pragma once
#ifndef IMAGE_H
#define IMAGE_H

#include <SFML\Graphics.hpp>

class Image
{
public:
	//Image constructor
	Image();

	//Image constructor
	Image(const sf::String& imageDir);

	//Image constructor
	Image(const sf::Vector2f& position, const sf::Vector2f& size);

	//Image constructor
	Image(const sf::String& imageDir, const sf::Vector2f& position, const sf::Vector2f& size);

	//Image constructor
	void onRender(sf::RenderWindow& win) const;

	//Image onRender handler
	const sf::Vector2f& getPosition() const;

	//Image set repeated
	void setRepeated(const bool& repeated);

	//Image set size
	void setSize(const float& x, const float& y);

	//Image set size
	void setSize(const sf::Vector2f& size);

	//Image set position
	void setPosition(const float& x, const float& y);

	//Image set position
	void setPosition(const sf::Vector2f& position);

	//Image set rotation
	void setRotation(const float& rotation);

	//Image set texture
	bool setTexture(const sf::String& imageDir);

	//Image get size
	sf::Vector2f getSize() const;

	//Image get rotation
	float getRotation() const;

	//Image move
	void move(const sf::Vector2f& offset);

	//Image move
	void move(const float& x, const float& y);
private:
	//Image sprite
	sf::Sprite image;

	//Image texture
	sf::Texture texture;
};

#endif