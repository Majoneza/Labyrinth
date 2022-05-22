#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SFML\System.hpp>

class Map;
class GameObject
{
public:
	//GameObject constructor
	GameObject();

	//GameObject
	GameObject(const Map* map);

	//GameObject constructor
	GameObject(const sf::Vector2u& mapPosition, const Map* m);

	//GameObject constructor
	GameObject(const sf::Uint32& mapPositionX, const sf::Uint32& mapPositionY, const Map* m);

	//GameObject destructor
	virtual ~GameObject() = default;

	//GameObject virtual onTick handler
	virtual void onTick() = 0;

	//GameObject get map position
	sf::Vector2u getPosition() const;

	//GameObject move
	bool move(const sf::Uint32& x, const sf::Uint32& y);

	//GameObject move
	bool move(const sf::Vector2u& offset);

	//GameObject set map position
	bool setPosition(const sf::Vector2u& pos);

	//GameObject set map position
	bool setPosition(const sf::Uint32& x, const sf::Uint32& y);

	//GameObject set Map
	void setMap(const Map* map);

	//GameObject get Map
	const Map* getMap() const;
protected:
	//GameObject map
	const Map* map;

	//GameObject position
	sf::Vector2u position;
};

#endif