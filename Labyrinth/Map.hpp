#pragma once
#ifndef MAP_H
#define MAP_H

#include <SFML\Graphics.hpp>
#include "Tile.hpp"
#include "dynarray.hpp"
#include "MazeGenerator.hpp"

class Map
{
public:
	//Map constructor
	Map(const sf::Uint32& MAP_WIDTH, const sf::Uint32& MAP_HEIGHT, const float& PIXEL_WIDTH, const float& PIXEL_HEIGHT);

	//Map constructor
	Map(const sf::Vector2u& MAP_SIZE, const sf::Vector2f& PIXEL_SIZE);

	//Map onEvent handler
	void onEvent(const sf::Event& evn);

	//Map onTick handler
	void onTick();

	//Map onRender handler
	void onRender(sf::RenderWindow& win);

	//Map has wall at
	bool hasWallAt(const sf::Uint32& x, const sf::Uint32& y) const;
	
	//Map has wall at
	bool hasWallAt(const sf::Vector2u& position) const;

	//Map valid pos
	bool validPos(const sf::Uint32& x, const sf::Uint32& y) const;

	//Map valid pos
	bool validPos(const sf::Vector2u& position) const;

	//Map set in game size
	void setTileSize(const float& pixel_width, const float& pixel_height);

	//Map set in game size
	void setTileSize(const sf::Vector2f& pixel_size);

	//Map get game position
	sf::Vector2f getGamePosition(const sf::Uint32& x, const sf::Uint32& y) const;

	//Map get game position
	sf::Vector2f getGamePosition(const sf::Vector2u& position) const;

	//Map get map position
	sf::Vector2u getMapPosition(const float& x, const float& y) const;

	//Map get map position
	sf::Vector2u getMapPosition(const sf::Vector2f& position) const;

	//Map reset all tiles to empty
	void clear();

	//Map get tile size
	const sf::Vector2f& getTileSize() const;

	//Map get map size
	const sf::Vector2u& getSize() const;

	//Map get vector size
	size_t getVectorSize() const;

	//Map set tile at position
	bool setTile(const Tile& tile);

	//Map get tile
	Tile& getTile(const sf::Uint32& x, const sf::Uint32& y);

	//Map get tile
	Tile& getTile(const sf::Vector2u& position);

	//Map get tile
	Tile& getTile(const size_t& index);

	//Map generate map
	void createMap(const sf::Texture& startSprite, const sf::Texture& endSprite, const sf::Texture& wallSprite);

	//Map returns how to solve the map
	std::vector<std::array<sf::Uint32, 3U>> solve();

	//Map start position
	sf::Vector2u startPos;

	//Map end position
	sf::Vector2u endPos;
private:
	//Map tiles
	dynarray<Tile> tiles;

	//Map maze generator
	MazeGenerator maze;

	//Map size
	const sf::Vector2u size;

	//Map tile size
	sf::Vector2f tileSize;
};

#endif