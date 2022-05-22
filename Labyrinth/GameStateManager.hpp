#pragma once
#ifndef GAMESTATEMANAGER_H
#define GAMESTATEMANAGER_H

#include <memory>
#include <SFML\Graphics.hpp>
#include "GameState.hpp"

class Game;
class GameStateManager
{
public:
	//Create GameStateManager object
	GameStateManager(Game & game);

	//GameStateManager destructor
	~GameStateManager() = default;

	//Set GameStateManager state
	void setState(GameState* gS);

	//GameStateManager onEvent handler
	void onEvent(const sf::Event& evn) const;

	//GameStateManager onTick handler
	void onTick() const;

	//GameStateManager onRender handler
	void onRender(sf::RenderWindow& win) const;
	
	//GameStateManager game
	Game & game;
private:
	//GameStateManager current state
	std::unique_ptr<GameState> gameState;
};

#endif