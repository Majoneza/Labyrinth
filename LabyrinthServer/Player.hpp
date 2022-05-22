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
	Player(const sf::Vector2u& mapPosition, const Map* map);

	//Player constructor
	Player(const sf::Uint32& x, const sf::Uint32& y, const Map* map);

	//Player onTick handler
	void onTick();

	//Player time left
	class {
	public:
		void set(float time)
		{
			timeLeft = time;
		}
		float get() const
		{
			return timeLeft - timer.getElapsedTime().asSeconds() < 0.f ? 0.f : timeLeft - timer.getElapsedTime().asSeconds();
		}
		void add(float time)
		{
			timeLeft += time;
		}
		void update()
		{
			timeLeft = timeLeft - timer.getElapsedTime().asSeconds();
			timer.restart();
		}
		void reset()
		{
			timer.restart();
		}
		operator float() const
		{
			return timeLeft;
		}
	private:
		float timeLeft;
		sf::Clock  timer;
	} timeLeft;

	//Player map number (starts with 0) == score
	sf::Uint16 mapNumber;
};

#endif