#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SFML\Graphics.hpp>

class Map;
class GameObject
{
public:
	//GameObject constructor
	GameObject();

	//GameObject
	GameObject(const Map* map);

	//GameObject constructor
	GameObject(const sf::String& texture, const sf::Vector2u& mapPosition, const sf::Vector2f& size, const Map* m);

	//GameObject constructor
	GameObject(const sf::String& texture, const sf::Uint32& mapPositionX, const sf::Uint32& mapPositionY, const float& sizeX, const float& sizeY, const Map* m);

	//GameObject constructor
	GameObject(const sf::Texture& texture, const sf::Vector2u& mapPosition, const sf::Vector2f& size, const Map* m);

	//GameObject constructor
	GameObject(const sf::Texture& texture, const sf::Uint32& mapPositionX, const sf::Uint32& mapPositionY, const float& sizeX, const float& sizeY, const Map* m);

	//GameObject destructor
	virtual ~GameObject() = default;

	//GameObject virtual onEvent handler
	virtual void onEvent(const sf::Event& evn) = 0;

	//GameObject virtual onTick handler
	virtual void onTick() = 0;

	//GameObject virtual onRender handler
	virtual void onRender(sf::RenderWindow& win) = 0;

	//GameObject set size
	void setSize(const sf::Vector2f& size);

	//GameObject set size
	void setSize(const float& sizeX, const float& sizeY);

	//GameObject get map position
	sf::Vector2u getPosition() const;

	//GameObject move
	bool move(const sf::Int32& x, const sf::Int32& y);

	//GameObject move
	bool move(const sf::Vector2i& offset);

	//GameObject set map position
	bool setPosition(const sf::Vector2u& pos);

	//GameObject set map position
	bool setPosition(const sf::Uint32& x, const sf::Uint32& y);

	//GameObject set map position without checking
	void forceSetPosition(const sf::Vector2u& pos);

	//GameObject set map position without checking
	void forceSetPosition(const sf::Uint32& x, const sf::Uint32& y);

	//GameObject set texture
	bool setTexture(const sf::String& texture);

	//GameObject set texture
	void setTexture(const sf::Texture& texture);

	//GameObject get game position
	const sf::Vector2f& getGamePosition() const;

	//GameObject set Map
	void setMap(const Map* map);

	//GameObject get Map
	const Map* getMap() const;
protected:
	//GameObject map
	const Map* map;

	//GameObject sprite
	sf::Sprite sprite;

	//GameObject texture
	sf::Texture texture;
};

#endif