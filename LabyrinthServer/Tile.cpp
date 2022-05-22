#include "stdafx.h"
#include "Tile.hpp"

Tile::Tile() :
	type(Empty)
{
}

Tile::Tile(const Type& type, const Map* map) :
	GameObject(map),
	type(type)
{
}

Tile::Tile(const Type& type, const sf::Vector2u& mapPosition, const Map* map) :
	GameObject(mapPosition, map),
	type(type)
{
}

Tile::Tile(const Type& type, const sf::Uint32& x, const sf::Uint32& y, const Map* map) :
	GameObject(x, y, map),
	type(type)
{
}

void Tile::onTick()
{
}