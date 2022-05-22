#pragma once
#ifndef GAMESTATEMANAGER_H
#define GAMESTATEMANAGER_H

#include <memory>
#include "GameState.hpp"

class Server;
class GameStateManager
{
public:
	//Create GameStateManager object
	GameStateManager(Server & server);

	//GameStateManager destructor
	~GameStateManager() = default;

	//Set GameStateManager state
	void setState(GameState* gS);

	//GameStateManager onTick handler
	void onTick() const;
	
	//GameStateManager game
	Server & server;
private:
	//GameStateManager current state
	std::unique_ptr<GameState> gameState;
};

#endif