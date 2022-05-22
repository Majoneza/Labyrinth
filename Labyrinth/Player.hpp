#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.hpp"

class Player : public GameObject
{
public:
	//Player constructor
	Player();

	//Player constructor
	Player(const sf::String& texture, const sf::Vector2u& mapPosition, const sf::Vector2f& size, const Map* map);

	//Player constructor
	Player(const sf::String& texture, const sf::Uint32& x, const sf::Uint32& y, const float& sizeX, const float& sizeY, const Map* map);

	//Player onEvent handler
	void onEvent(const sf::Event& evn) override;

	//Player onTick handler
	void onTick() override;

	//Player onRender handler
	void onRender(sf::RenderWindow& win) override;

	//Player move up
	void moveUp();

	//Player move down
	void moveDown();

	//Player move left
	void moveLeft();

	//Player move right
	void moveRight();
};

#endif