#include "stdafx.h"
#include "GameStateManager.hpp"

GameStateManager::GameStateManager(Server & server) : 
	gameState(nullptr), 
	server(server)
{
}

void GameStateManager::setState(GameState* gS)
{
	gameState.release();
	gameState = std::unique_ptr<GameState>(gS);
}

void GameStateManager::onTick() const
{
	gameState->onTick();
}