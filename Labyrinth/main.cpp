// Labyrinth.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Game.hpp"

int main()
{
	//SIGNAL HANDLING FOR TASK MANAGER
	//Start game
	
	Game game("Labyrinth", sf::Style::Default, sf::ContextSettings(24, 8, 16), "game/config.json");

	//TREBA POPRAVITI INPUT HANDLER I INPUT LISTENER PREVISE POSLA NAPRAVI ZA SVAKI KLIK

	return 0;
}