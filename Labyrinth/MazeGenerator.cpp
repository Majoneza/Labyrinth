#include "stdafx.h"
#include "MazeGenerator.hpp"

MazeGenerator::MazeGenerator(int width, int height) :
	width(width),
	height(height),
	engine((std::random_device())()),
	randomizer(0, 3),
	nodes(width * height)
{
	reset();
}

void MazeGenerator::generate()
{
	Node *start, *last;

	start = nodes.begin() + 1 + width;
	start->parent = start;
	last = start;

	while ((last = link(last)) != start);
}

void MazeGenerator::reset()
{
	nodes.fill(Node());
	Node* n;

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			n = nodes.begin() + i + j * width;
			if (i * j % 2)
			{
				n->x = i;
				n->y = j;
				n->dirs = 15;
				n->type = Empty;
			}
			else n->type = Wall;
		}
	}
}

MazeGenerator::Node* MazeGenerator::link(Node* n)
{
	int x = 0, y = 0;
	char dir;
	Node* dest;

	if (n == nullptr) return nullptr;

	while (n->dirs)
	{
		dir = (1 << randomizer(engine));

		if (~n->dirs & dir) continue;

		n->dirs &= ~dir;

		switch (dir)
		{
			//Right
		case Right:
			if (n->x + 2 < width)
			{
				x = n->x + 2;
				y = n->y;
			}
			else continue;
			break;
			//Down
		case Down:
			if (n->y + 2 < height)
			{
				x = n->x;
				y = n->y + 2;
			}
			else continue;
			break;
			//Left
		case Left:
			if (n->x - 2 >= 0)
			{
				x = n->x - 2;
				y = n->y;
			}
			else continue;
			break;
			//Up
		case Up:
			if (n->y - 2 >= 0)
			{
				x = n->x;
				y = n->y - 2;
			}
			else continue;
			break;
		default:
			break;
		}
		dest = nodes.begin() + x + y * width;

		if (dest->type == Empty)
		{
			if (dest->parent != nullptr) continue;

			dest->parent = n;

			nodes[n->x + (x - n->x) / 2 + (n->y + (y - n->y) / 2) * width].type = Empty;

			return dest;
		}
	}
	return n->parent;
}