#include "stdafx.h"
#include "GameObject.hpp"
#include "Map.hpp"

GameObject::GameObject() :
	map(nullptr), position(0, 0)
{
}

GameObject::GameObject(const Map* map) :
	map(map), position(0, 0)
{
}

GameObject::GameObject(const sf::Vector2u& mapPosition, const Map* m) :
	map(m), position(mapPosition)
{
	if (!setPosition(position)) setPosition(1, 1);
}

GameObject::GameObject(const sf::Uint32& mapPositionX, const sf::Uint32& mapPositionY, const Map* m) :
	map(m), position(mapPositionX, mapPositionY)
{
	if (!setPosition(position)) setPosition(1, 1);
}

sf::Vector2u GameObject::getPosition() const
{
	return position;
}

bool GameObject::move(const sf::Uint32& x, const sf::Uint32& y)
{
	return setPosition(getPosition() + sf::Vector2u(x, y));
}

bool GameObject::move(const sf::Vector2u& offset)
{
	return setPosition(getPosition() + offset);
}

bool GameObject::setPosition(const sf::Vector2u& pos)
{
	if (map->validPos(pos)) position = pos;
	else return false;
	return true;
}

bool GameObject::setPosition(const sf::Uint32& x, const sf::Uint32& y)
{
	if (map->validPos(x, y)) position = { x, y };
	else return false;
	return true;
}

void GameObject::setMap(const Map* map)
{
	this->map = map;
}

const Map* GameObject::getMap() const
{
	return map;
}