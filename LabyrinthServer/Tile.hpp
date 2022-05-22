#pragma once
#ifndef TILE_H
#define TILE_H

#include "GameObject.hpp"

class Tile : public GameObject
{
public:
	//Tile types
	enum Type { Empty, Wall, MoreTime };

	//Tile constructor
	Tile();

	//Tile constructor
	Tile(const Type& type, const Map* map);

	//Tile constructor
	Tile(const Type& type, const sf::Vector2u& mapPosition, const Map* map);

	//Tile constructor
	Tile(const Type& type, const sf::Uint32& x, const sf::Uint32& y, const Map* map);

	//Tile onTick handler
	void onTick() override;

	//Tile type
	Type type;
};

#endif