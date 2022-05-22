#include "stdafx.h"
#include "Map.hpp"
#include <array>
#include <vector>
#include "MazeGenerator.hpp"

Map::Map(const sf::Uint32& MAP_WIDTH, const sf::Uint32& MAP_HEIGHT, const float& PIXEL_WIDTH, const float& PIXEL_HEIGHT) :
	tileSize(PIXEL_WIDTH / MAP_WIDTH, PIXEL_HEIGHT / MAP_HEIGHT),
	size(MAP_WIDTH, MAP_HEIGHT),
	tiles(MAP_WIDTH * MAP_HEIGHT),
	startPos(0U, 0U),
	endPos(0U, 0U),
	maze(MAP_WIDTH + 1, MAP_HEIGHT + 1)
{
}

Map::Map(const sf::Vector2u& MAP_SIZE, const sf::Vector2f& PIXEL_SIZE) :
	tileSize(PIXEL_SIZE.x / static_cast<float>(MAP_SIZE.x), static_cast<float>(PIXEL_SIZE.y / MAP_SIZE.y)),
	size(MAP_SIZE.x, MAP_SIZE.y),
	tiles(MAP_SIZE.x * MAP_SIZE.y),
	startPos(0U, 0U),
	endPos(0U, 0U),
	maze(MAP_SIZE.x + 1, MAP_SIZE.y + 1)
{
}

void Map::onEvent(const sf::Event& evn)
{
	for (size_t i = 0; i < size.x * size.y; i++)
	{
		tiles[i].onEvent(evn);
	}
}

void Map::onTick()
{
	for (size_t i = 0; i < size.x * size.y; i++)
	{
		tiles[i].onTick();
	}
}

void Map::onRender(sf::RenderWindow& win)
{
	for (size_t i = 0; i < size.x * size.y; i++)
	{
		tiles[i].onRender(win);
	}
}

bool Map::hasWallAt(const sf::Uint32& x, const sf::Uint32& y) const
{
	return validPos(x, y) && tiles[(y - 1U) * size.x + (x - 1U)].type == Tile::Type::Wall;
}

bool Map::hasWallAt(const sf::Vector2u& position) const
{
	return validPos(position) && tiles[(position.y - 1U) * size.x + (position.x - 1U)].type == Tile::Type::Wall;
}

bool Map::validPos(const sf::Uint32& x, const sf::Uint32& y) const
{
	return size.x >= x && x > 0U && size.y >= y && y > 0U;
}

bool Map::validPos(const sf::Vector2u& position) const
{
	return size.x >= position.x && position.x > 0U && size.y >= position.y && position.y > 0U;
}

void Map::setTileSize(const float& pixel_width, const float& pixel_height)
{
	tileSize = { pixel_width / size.x, pixel_height / size.y };
	for (size_t i = 0; i < size.x * size.y; i++)
	{
		tiles[i].setSize(tileSize);
	}
}

void Map::setTileSize(const sf::Vector2f& pixel_size)
{
	tileSize = { pixel_size.x / size.x, pixel_size.y / size.y };
	for (size_t i = 0; i < size.x * size.y; i++)
	{
		tiles[i].setSize(tileSize);
	}
}

sf::Vector2f Map::getGamePosition(const sf::Uint32& x, const sf::Uint32& y) const
{
	return { x * tileSize.x - (tileSize.x / 2.f), y * tileSize.y - (tileSize.y / 2.f) };
}

sf::Vector2f Map::getGamePosition(const sf::Vector2u& position) const
{
	return { position.x * tileSize.x - (tileSize.x / 2.f), position.y * tileSize.y - (tileSize.y / 2.f) };
}

sf::Vector2u Map::getMapPosition(const float& x, const float& y) const
{
	return { static_cast<unsigned int>(round((x + (tileSize.x / 2.f)) / tileSize.x)), static_cast<unsigned int>(round((y + (tileSize.y / 2.f)) / tileSize.y)) };
}

sf::Vector2u Map::getMapPosition(const sf::Vector2f& position) const
{
	return { static_cast<unsigned int>(round((position.x + (tileSize.x / 2.f)) / tileSize.x)), static_cast<unsigned int>(round((position.y + (tileSize.y / 2.f)) / tileSize.y)) };
}

void Map::clear()
{
	tiles.fill(Tile(Tile::Type::Empty, this));
}

const sf::Vector2f& Map::getTileSize() const
{
	return tileSize;
}

const sf::Vector2u& Map::getSize() const
{
	return size;
}

size_t Map::getVectorSize() const
{
	return tiles.size();
}

bool Map::setTile(const Tile& tile)
{
	if (validPos(tile.getPosition())) tiles[(tile.getPosition().y - 1) * size.x + (tile.getPosition().x - 1)] = tile;
	else return false;
	return true;
}

Tile& Map::getTile(const sf::Uint32& x, const sf::Uint32& y)
{
	return tiles[(y - 1U) * size.x + (x - 1U)];
}

Tile& Map::getTile(const sf::Vector2u& position)
{
	return tiles[(position.y - 1U) * size.x + (position.x - 1U)];
}

Tile& Map::getTile(const size_t& index)
{
	return tiles[index];
}

void Map::createMap(const sf::Texture& startSprite, const sf::Texture& endSprite, const sf::Texture& wallSprite)
{
	clear();
	maze.reset();
	maze.generate();
	startPos = { 1, 1 };
	endPos = size.x % 2 ? sf::Vector2u{ size.x, size.y } : sf::Vector2u{ size.x - 1, size.y - 1 };
	getTile(startPos).setTexture(startSprite);
	getTile(endPos).setTexture(endSprite);
	for (int x = 1; x < maze.width; x++)
	{
		for (int y = 1; y < maze.height; y++)
		{
			MazeGenerator::Node& node = maze.nodes[x + y * maze.width];
			Tile& tile = getTile(x, y);
			if (node.type == MazeGenerator::Type::Wall)
			{
				tile.setTexture(wallSprite);
				tile.type = Tile::Type::Wall;
			}
			tile.setPosition(x, y);
			tile.setSize(tileSize);
		}
	}
}

//Ovo radi tako sto ostavi uzorak koji direktno vodi do cilja
/*
clear();
startPos = { positionRandomizer(engine), positionRandomizer(engine) };
endPos = { positionRandomizer(engine), positionRandomizer(engine) };
for (size_t height = 1; height <= size.y; height++)
{
	for (size_t width = 1; width <= size.x; width++)
	{
		if (width != startPos.x && width != endPos.x && height != startPos.y && height != endPos.y)
		{
			if (wallRandomizer(engine) <= wallPercent) setTile(Tile(wallSprite, Tile::Type::Wall, sf::Vector2u(width, height), tileSize, this));
			else setTile(Tile(sf::Texture(), Tile::Type::Empty, sf::Vector2u(width, height), tileSize, this));
		}
		else if (!(width == startPos.x && width == endPos.x && height == startPos.y && height == endPos.y)) setTile(Tile(sf::Texture(), Tile::Type::Empty, sf::Vector2u(width, height), tileSize, this));
	}
}
setTile(Tile(startSprite, Tile::Type::Empty, startPos, tileSize, this));
setTile(Tile(endSprite, Tile::Type::Empty, endPos, tileSize, this));*/

std::vector<std::array<sf::Uint32, 3U>> Map::solve()
{
	//STEPS: START = 0, LEFT = 1, RIGHT = 2, DOWN = 3, UP = 4
	std::vector<std::array<sf::Uint32, 3U>> steps;
	std::vector<std::array<sf::Uint32, 3U>> invalid;
	steps.push_back({ startPos.x, startPos.y, 0U });
	while (true)
	{
		std::array<sf::Uint32, 3U>& position = steps.back();
		//DOWN
		if (position[2U] != 4U &&
			validPos(position[0U], position[1U] + 1U) &&
			getTile(position[0U], position[1U] + 1U).type != Tile::Type::Wall &&
			std::find(invalid.cbegin(), invalid.cend(), std::array<sf::Uint32, 3U>({ position[0U], position[1U] + 1U, 3U })) == invalid.cend())
			steps.push_back({ position[0U], position[1U] + 1U, 3U });
		//RIGHT
		else if (position[2U] != 1U &&
			validPos(position[0U] + 1U, position[1U]) &&
			getTile(position[0U] + 1U, position[1U]).type != Tile::Type::Wall &&
			std::find(invalid.cbegin(), invalid.cend(), std::array<sf::Uint32, 3U>({ position[0U] + 1U, position[1U], 2U })) == invalid.cend())
			steps.push_back({ position[0U] + 1, position[1U], 2U });
		//UP
		else if (position[2U] != 3U &&
			validPos(position[0U], position[1U] - 1U) &&
			getTile(position[0U], position[1U] - 1U).type != Tile::Type::Wall &&
			std::find(invalid.cbegin(), invalid.cend(), std::array<sf::Uint32, 3U>({ position[0U], position[1U] - 1U, 4U })) == invalid.cend())
			steps.push_back({ position[0U], position[1U] - 1U, 4U });
		//LEFT
		else if (position[2U] != 2U &&
			validPos(position[0U] - 1U, position[1U]) &&
			getTile(position[0U] - 1U, position[1U]).type != Tile::Type::Wall &&
			std::find(invalid.cbegin(), invalid.cend(), std::array<sf::Uint32, 3U>({ position[0U] - 1U, position[1U], 1U })) == invalid.cend())
			steps.push_back({ position[0U] - 1U, position[1U], 1U });
		else if (steps.size() > 1)
		{
			steps.pop_back();
			auto r = std::find(invalid.cbegin(), invalid.cend(), steps.back());
			if (r != invalid.cend())
				invalid.erase(r);
		}
		else return {};
		if (endPos.x == position[0] && endPos.y == position[1]) return steps;
		invalid.push_back(position);
	}
}