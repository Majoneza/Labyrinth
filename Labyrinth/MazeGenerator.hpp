#pragma once
#ifndef MAZE_GENERATOR_HPP
#define MAZE_GENERATOR_HPP

#include <random>
#include "dynarray.hpp"

class MazeGenerator
{
public:
	enum Type { Empty, Wall };
	enum Direction { Right = 1 << 0, Down = 1 << 1, Left = 1 << 2, Up = 1 << 3 };

	struct Node
	{
		int x, y;
		Type type;
		char dirs;
		Node* parent;
	};

	MazeGenerator(int width, int height);

	void generate();

	void reset();

	dynarray<Node> nodes;

	const int width, height;

	std::mt19937 engine;
	const std::uniform_int_distribution<short> randomizer;
private:
	Node* link(Node* node);
};

#endif