#include "stdafx.h"
#include "GameStateManager.hpp"

GameStateManager::GameStateManager(Game & game) : 
	gameState(nullptr), 
	game(game)
{
}

void GameStateManager::setState(GameState* gS)
{
	gameState.release();
	gameState = std::unique_ptr<GameState>(gS);
}

void GameStateManager::onEvent(const sf::Event& evn) const
{
	gameState->onEvent(evn);
}

void GameStateManager::onTick() const
{
	gameState->onTick();
}

void GameStateManager::onRender(sf::RenderWindow& win) const
{
	gameState->onRender(win);
}