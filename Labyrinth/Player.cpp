#include "stdafx.h"
#include "Player.hpp"
#include "Map.hpp"

Player::Player() :
	GameObject()
{
}

Player::Player(const sf::String& texture, const sf::Vector2u& mapPosition, const sf::Vector2f& size, const Map* map) :
	GameObject(texture, mapPosition, size, map)
{
}

Player::Player(const sf::String& texture, const sf::Uint32& x, const sf::Uint32& y, const float& sizeX, const float& sizeY, const Map* map) :
	GameObject(texture, x, y, sizeX, sizeY, map)
{
}

void Player::onEvent(const sf::Event& evn)
{
	
}

void Player::onTick()
{
}

void Player::onRender(sf::RenderWindow& win)
{
	sprite.setTexture(texture);
	win.draw(sprite);
}

void Player::moveUp()
{
	if (!map->hasWallAt(getPosition() - sf::Vector2u{ 0U, 1U })) move(0, -1);
	sprite.setRotation(180.f);
}

void Player::moveDown()
{
	if (!map->hasWallAt(getPosition() + sf::Vector2u{ 0U, 1U })) move(0, 1);
	sprite.setRotation(0.f);
}

void Player::moveLeft()
{
	if (!map->hasWallAt(getPosition() - sf::Vector2u{ 1U, 0U })) move(-1, 0);
	sprite.setRotation(90.f);
}

void Player::moveRight()
{
	if (!map->hasWallAt(getPosition() + sf::Vector2u{ 1U, 0U })) move(1, 0);
	sprite.setRotation(270.f);
}