#include "stdafx.h"
#include "GameObject.hpp"
#include "Map.hpp"

GameObject::GameObject() :
	map(nullptr)
{
}

GameObject::GameObject(const Map* map) :
	map(map)
{
}

GameObject::GameObject(const sf::String& texture, const sf::Vector2u& mapPosition, const sf::Vector2f& size, const Map* m) :
	map(m)
{
	if(!setTexture(texture)) printf("Could not load texture: %s", (static_cast<std::string>(texture)).c_str());
	setSize(size);
	if (!setPosition(mapPosition)) setPosition(1, 1);
}

GameObject::GameObject(const sf::String& texture, const sf::Uint32& mapPositionX, const sf::Uint32& mapPositionY, const float& sizeX, const float& sizeY, const Map* m) :
	map(m)
{
	if(!setTexture(texture)) printf("Could not load texture: %s", (static_cast<std::string>(texture)).c_str());
	setSize(sizeX, sizeY);
	if (!setPosition(mapPositionX, mapPositionY)) setPosition(1, 1);
}

GameObject::GameObject(const sf::Texture& texture, const sf::Vector2u& mapPosition, const sf::Vector2f& size, const Map* m) :
	map(m), texture(texture)
{
	sprite.setTexture(this->texture);
	setSize(size);
	if (!setPosition(mapPosition)) setPosition(1, 1);
}

GameObject::GameObject(const sf::Texture& texture, const sf::Uint32& mapPositionX, const sf::Uint32& mapPositionY, const float& sizeX, const float& sizeY, const Map* m) :
	map(m), texture(texture)
{
	sprite.setTexture(this->texture);
	setSize(sizeX, sizeY);
	if (!setPosition(mapPositionX, mapPositionY)) setPosition(1, 1);
}

bool GameObject::move(const sf::Int32& x, const sf::Int32& y)
{
	const auto position = getPosition();
	return setPosition(position.x + x, position.y + y);
}

bool GameObject::move(const sf::Vector2i& offset)
{
	const auto position = getPosition();
	return setPosition(position.x + offset.x, position.y + offset.y);
}

bool GameObject::setPosition(const sf::Vector2u& pos)
{
	if (map->validPos(pos)) sprite.setPosition(map->getGamePosition(pos));
	else return false;
	return true;
}

bool GameObject::setPosition(const sf::Uint32& x, const sf::Uint32& y)
{
	if (map->validPos(x, y)) sprite.setPosition(map->getGamePosition(x, y));
	else return false;
	return true;
}

bool GameObject::setTexture(const sf::String& texture)
{
	if (!this->texture.loadFromFile(texture)) return false;
	sprite.setOrigin(static_cast<float>(this->texture.getSize().x / 2), static_cast<float>(this->texture.getSize().y / 2));
	return true;
}

void GameObject::setTexture(const sf::Texture& texture)
{
	this->texture = texture;
	sprite.setOrigin(static_cast<float>(this->texture.getSize().x / 2), static_cast<float>(this->texture.getSize().y / 2));
}

void GameObject::setSize(const sf::Vector2f& size)
{
	sprite.setScale(size.x / texture.getSize().x, size.y / texture.getSize().y);
}

void GameObject::setSize(const float& sizeX, const float& sizeY)
{
	sprite.setScale(sizeX / texture.getSize().x, sizeY / texture.getSize().y);
}

sf::Vector2u GameObject::getPosition() const
{
	return map->getMapPosition(sprite.getPosition());
}

void GameObject::forceSetPosition(const sf::Vector2u& pos)
{
	sprite.setPosition(map->getGamePosition(pos));
}

void GameObject::forceSetPosition(const sf::Uint32& x, const sf::Uint32& y)
{
	sprite.setPosition(map->getGamePosition(x, y));
}

const sf::Vector2f& GameObject::getGamePosition() const
{
	return sprite.getPosition();
}

void GameObject::setMap(const Map* map)
{
	this->map = map;
}

const Map* GameObject::getMap() const
{
	return map;
}