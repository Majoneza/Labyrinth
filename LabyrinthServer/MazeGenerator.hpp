#pragma once
#ifndef MAZE_GENERATOR_HPP
#define MAZE_GENERATOR_HPP

#include <random>
#include "dynarray.hpp"

class MazeGenerator
{
public:
	enum Type { Empty, Wall };

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