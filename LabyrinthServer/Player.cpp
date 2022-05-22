#include "stdafx.h"
#include "Player.hpp"

Player::Player() :
	timeLeft(),
	mapNumber(0U)
{
}

Player::Player(const sf::Vector2u& mapPosition, const Map* map) :
	GameObject(mapPosition, map),
	timeLeft(),
	mapNumber(0U)
{
}

Player::Player(const sf::Uint32& x, const sf::Uint32& y, const Map* map) :
	GameObject(x, y, map),
	timeLeft(),
	mapNumber(0U)
{
}

void Player::onTick()
{
}