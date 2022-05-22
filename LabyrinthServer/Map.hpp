#pragma once
#ifndef MAP_H
#define MAP_H

#include <SFML\Graphics.hpp>
#include <vector>
#include "Tile.hpp"
#include "dynarray.hpp"
#include "MazeGenerator.hpp"

class Map
{
public:
	//Map constructor
	Map(const sf::Uint32& MAP_WIDTH, const sf::Uint32& MAP_HEIGHT);

	//Map constructor
	Map(const sf::Vector2u& MAP_SIZE);

	//Map onTick handler
	void onTick();

	//Map has wall at
	bool hasWallAt(const sf::Uint32& x, const sf::Uint32& y) const;
	
	//Map has wall at
	bool hasWallAt(const sf::Vector2u& position) const;

	//Map valid pos
	bool validPos(const sf::Uint32& x, const sf::Uint32& y) const;

	//Map valid pos
	bool validPos(const sf::Vector2u& position) const;

	//Map reset all tiles to empty
	void clear();

	//Map set tile at position
	bool setTile(const Tile& tile);

	//Map get tile
	Tile& getTile(const sf::Uint32& x, const sf::Uint32& y);

	//Map get tile
	Tile& getTile(const sf::Vector2u& position);

	//Map get tile
	Tile& getTile(const size_t& index);

	//Map get tile
	const Tile& getTile(const sf::Uint32& x, const sf::Uint32& y) const;

	//Map get tile
	const Tile& getTile(const sf::Vector2u& position) const;

	//Map get tile
	const Tile& getTile(const size_t& index) const;

	//Map generate map
	void createMap();

	//Map returns how to solve the map
	std::vector<std::array<sf::Uint32, 3U>> solve();

	//Map start position
	sf::Vector2u startPos;

	//Map end position
	sf::Vector2u endPos;

	//Map size
	const sf::Vector2u size;
private:
	//Map tiles
	dynarray<Tile> tiles;

	//Map maze generator
	MazeGenerator maze;
};

#endif