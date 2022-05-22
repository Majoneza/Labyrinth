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

Tile::Tile(const sf::String& texture, const Type& type, const sf::Vector2u& mapPosition, const sf::Vector2f& size, const Map* map) :
	GameObject(texture, mapPosition, size, map),
	type(type)
{
}

Tile::Tile(const sf::String& texture, const Type& type, const sf::Uint32& x, const sf::Uint32& y, const float& sizeX, const float& sizeY, const Map* map) :
	GameObject(texture, x, y, sizeX, sizeY, map),
	type(type)
{
}

Tile::Tile(const sf::Texture& texture, const Type& type, const sf::Vector2u& mapPosition, const sf::Vector2f& size, const Map* map) :
	GameObject(texture, mapPosition, size, map),
	type(type)
{
}

Tile::Tile(const sf::Texture& texture, const Type& type, const sf::Uint32& x, const sf::Uint32& y, const float& sizeX, const float& sizeY, const Map* map) :
	GameObject(texture, x, y, sizeX, sizeY, map),
	type(type)
{
}

void Tile::onEvent(const sf::Event& evn)
{

}

void Tile::onTick()
{

}

void Tile::onRender(sf::RenderWindow& win)
{
	sprite.setTexture(texture);
	win.draw(sprite);
}