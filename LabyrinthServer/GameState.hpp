#pragma once
#ifndef GAMESTATE_H
#define GAMESTATE_H

class Game;
class Server;
class GameStateManager;
class GameState
{
public:
	//GameState constructor
	GameState(GameStateManager& gsm);

	//GameState destructor
	virtual ~GameState() = default;

	//GameState onTick handler
	virtual void onTick() = 0;
protected:
	//GameState GameStateManager
	GameStateManager& gsm;

	//GameState server
	Server& server;
};

#endif