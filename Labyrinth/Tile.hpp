#pragma once
#ifndef TILE_H
#define TILE_H

#include "GameObject.hpp"

class Tile : public GameObject
{
public:
	//Tile types
	enum Type { Empty, Wall };

	//Tile constructor
	Tile();

	//Tile constructor
	Tile(const Type& type, const Map* map);

	//Tile constructor
	Tile(const sf::String& texture, const Type& type, const sf::Vector2u& mapPosition, const sf::Vector2f& size, const Map* map);

	//Tile constructor
	Tile(const sf::String& texture, const Type& type, const sf::Uint32& x, const sf::Uint32& y, const float& sizeX, const float& sizeY, const Map* map);

	//Tile constructor
	Tile(const sf::Texture& texture, const Type& type, const sf::Vector2u& mapPosition, const sf::Vector2f& size, const Map* map);

	//Tile constructor
	Tile(const sf::Texture& texture, const Type& type, const sf::Uint32& x, const sf::Uint32& y, const float& sizeX, const float& sizeY, const Map* map);

	//Tile onEvent handler
	void onEvent(const sf::Event& evn) override;

	//Tile onTick handler
	void onTick() override;

	//Tile onRender handler
	void onRender(sf::RenderWindow& win) override;

	//Tile type
	Type type;
};

#endif